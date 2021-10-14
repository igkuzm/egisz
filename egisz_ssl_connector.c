/**
 * File              : egisz_ssl_connector.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 12.10.2021
 * Last Modified Date: 14.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/**
 * ssl_sonnector.c
 * Copyright (c) 2021 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "egisz_ssl_connector.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <unistd.h>
#if defined _WIN32 || defined _WIN64
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <netdb.h>

char *http_msg_with_args(const char *hostname, const char *http_method, int argc, char *argv[]){
	
	char write_args[BUFSIZ];
	sprintf(write_args, "");
	strcat(write_args, http_method);
	if (argc) {
		strcat(write_args, "?");
		strcat(write_args, argv[0]);
	}
	int i;
	for (i = 1; i < argc; ++i) {
		strcat(write_args, "&");
		strcat(write_args, argv[i]);
	}	
 
	char http_request[BUFSIZ];
	sprintf(http_request, "%s HTTP/1.1\r\n", write_args);	

	printf("HTTP REQUEST: %s\n", http_request);

	char write_host[256];
	sprintf(write_host, "Host: %s\r\n", hostname);	

	
	char *write_buf = calloc(BUFSIZ, sizeof(char));
	sprintf(write_buf, "");
	strcat(write_buf, http_request);
	strcat(write_buf, write_host);
	strcat(write_buf, "Accept: application/json\r\n");
	strcat(write_buf, "Connection: close\r\n");
	strcat(write_buf, "\r\n");

	return write_buf;
}

void handle_with_ssl_error(SSL *ssl, int retval){
	switch (SSL_get_error (ssl, retval)){
		case SSL_ERROR_WANT_READ:
			fprintf (stderr, "Wait for the socket to be readable, then call this function again.\n");
			break;
		case SSL_ERROR_WANT_X509_LOOKUP:
			break;
		case SSL_ERROR_WANT_WRITE:
			fprintf (stderr, "Wait for the socket to be writeable, then call this function again.\n");
			break;
		case SSL_ERROR_SSL:
			break;
		case SSL_ERROR_SYSCALL:
			break;
	}
}

int egisz_ssl_connector_socket(const char *hostname, int port, const char *apikey, const char *http_method, int argc, char *argv[], void *data, int (*callback)(char*,int,int*,void*)){

	//init SSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *method = SSLv23_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if ( ctx == NULL ){
		fprintf(stderr, "Error. Can't init SSL_CTX\n");	
		return -1;
	} 
 
    struct hostent *host;
    struct sockaddr_in addr;
 
    if ( (host = gethostbyname(hostname)) == NULL ){
		fprintf(stderr, "Error. Can't get host ip address with hostname: %s\n", hostname);	
		return -1;
	} 
    int sd = socket(PF_INET, SOCK_STREAM, 0); //init socket
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof addr)){ //connect socket
		fprintf(stderr, "Error. Can't connect to socket with address: %s:%d\n", host->h_addr, port);	
		return -1;
	} 
    
    SSL *ssl = SSL_new(ctx); //create ssl structure
    SSL_set_fd(ssl, sd); //connect SSL to socket 
    if ( SSL_connect(ssl) == -1 ){
		fprintf(stderr, "Error. Can't connect SSL to socket with address: %s:%d\n", host->h_addr, port);	
		return -1;
	}  

	//generage HTTP REQUEST MESSAGE
	char *write_buf = http_msg_with_args(hostname, http_method, argc, argv);
	if (write_buf == NULL){
		char argv_string[BUFSIZ];
		int i;
		for (i = 0; i < argc; ++i) {
			strcat(argv_string, argv[i]);	
			strcat(argv_string, " ");	
		}
		fprintf(stderr, "Error. Can't merge http request for host: %s with http method: %s, arguments: %s \n", hostname, http_method, argv_string);	
		return -1;		
	}

	//SSL WRITE
	int retval;
	retval = SSL_write(ssl, write_buf, strlen(write_buf));

	free(write_buf); //no need any more

	if (retval <= 0 ){ //handle with error
		handle_with_ssl_error(ssl, retval);
		fprintf(stderr, "Error while SSL_write\n");
		return retval;			
	}

    //SSL READ
	int count = 0;
    char buf[1024];
	long bytes;
    while ((bytes = SSL_read(ssl, buf, sizeof buf)) >0 ) {
		buf[bytes] = 0;
		
		//printf("%s", buf); //print for debug
		
		int c = callback(buf, bytes, &count, data); //run callback
		if (c != 0) { //stop function if callback returned non zero
			fprintf(stderr, "Stop SSL_read - callback returned: %d\n", c);
			break;
		}
		count++; //we need count to know how many times callback was called
    }
	if (bytes < 0 ){ //hendle with error
		handle_with_ssl_error(ssl, bytes);
		fprintf(stderr, "Error while SSL_read\n");
		return retval;			
	}	

	//Close SSL
    SSL_free(ssl);   
    close(sd);      
    SSL_CTX_free(ctx);   
	

	return 0;
}


EgiszSSLConnectorAnswer *egisz_ssl_connector_answer_new(){
	EgiszSSLConnectorAnswer *answer = malloc(sizeof(EgiszSSLConnectorAnswer));
	if (answer == NULL) {
		fprintf(stderr, "Cannot allocate memory for EgiszSSLConnectorAnswer\n");
		exit(ENOMEM);
	}
	answer->body = malloc(BUFSIZ*sizeof(char));
	if (answer->body == NULL) {
		fprintf(stderr, "Cannot allocate memory for EgiszSSLConnectorAnswer->body\n");
		exit(ENOMEM);
	}	
	return answer;
}

void egisz_ssl_connector_answer_free(EgiszSSLConnectorAnswer *answer){
	free(answer->body);
	free(answer);
}

int egisz_ssl_connector_answer_callback(char *str, int len, int *count, void *_answer){
	EgiszSSLConnectorAnswer *answer = _answer;
	
	if (*count == 0) { //find header
		int len_header = 0, len_line = 0;
		int i;
		char line[BUFSIZ];
		for (i = 0; i < len; ++i) {
			if (str[i] == '\n' && str[i+1] == '\r') {//new line with \r is header stop
				len_header = i;
			}
			if (str[i] == '\n') { //new line in header
				//find header arguments
				sscanf(line, " Content-Type: %s", answer->content_type);
				sscanf(line, " Content-Length: %d", &answer->content_length);
				sscanf(line, " Transfer-Encoding: %s", answer->transfer_encoding);
				len_line = 0;
				line[0] = '\0';
			}			
			else {
				line[len_line] = str[i];
				len_line++;	
			}
		}
		answer->len_header = len_header;
		strncpy(answer->header, str, len_header);

		//remove header, and continue to parce answer
		answer->len_body = len - len_header;
		for (i = 0; i < len_header + 1; ++i) {
			memmove(&str[0],&str[1],len-1);
			len--;
		}
	}

	////////////////////////////
	if (answer->content_length > 0) { //OK - we have a standart content
		if (answer->content_length > answer->len_body) {
			answer->body = realloc(answer->body, answer->content_length * sizeof(char)); //realoc body string to match content length
			if (answer->body == NULL) {
				fprintf(stderr, "Cannot reallocate memory for EgiszSSLConnectorAnswer->body\n");
				exit(ENOMEM);
			}		
		}
		answer->len_body = answer->content_length;
		strncat(answer->body, str, len); //cat string to body
	}

	////////////////////////////
	if (strcmp(answer->transfer_encoding, "chunked") == 0) { //We have a chunked content
		//lets find size of chunked
		bool we_have_chunked_size = false, chunked_size_is_zero = false;
		int start_of_chunked = 0;
		int i;
		for (i = 0; i < len; ++i) {
			if (str[i] == '\r' && str[i+1] == '\n') { //chunked size goes after \r\n
				char chunked_size_str[5];
				chunked_size_str[0] = str[i+2];
				chunked_size_str[1] = str[i+3];
				chunked_size_str[2] = str[i+4];
				chunked_size_str[3] = str[i+5];
				chunked_size_str[4] = '\0';

				//get size integer
				int chunked_size;
				if (sscanf(chunked_size_str,"%x",&chunked_size) != 1) {
					fprintf(stderr, "Can't get chunked_size from string: %s\n", chunked_size_str);
				} 
				else { //good - we have a chunked size
					//printf("CHUNKED SIZE: %d\n", chunked_size);
					answer->len_body += chunked_size;
					answer->body = realloc(answer->body, answer->len_body * sizeof(char)); //realoc body string to add chunked_size
					if (answer->body == NULL) {
						fprintf(stderr, "Cannot reallocate memory for EgiszSSLConnectorAnswer->body\n");
						exit(ENOMEM);
					}		
					we_have_chunked_size = 1;
					start_of_chunked = i;
					i = len; //stop FOR cicle
					if (chunked_size == 0) {
						chunked_size_is_zero = true;	
					}
				}
			}
			else { //no chunked size this callback
			
			}
		}
		if (we_have_chunked_size) {
		//remove chunked_size string from str
			for (i = 0; i < 8; ++i) {
				memmove(&str[start_of_chunked],&str[start_of_chunked + 1],len-1);
				len--;
			}
		}
		if (chunked_size_is_zero) {
			len++;
		}
		//
		strncat(answer->body, str, len); //cat string to body			
		//
	}	



	return 0;
}
