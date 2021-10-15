/**
 * File              : http_connector.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.10.2021
 * Last Modified Date: 15.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "http_connector.h"
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
#include <netdb.h>
#endif

int parse_http_string(char *http_string, HTTP_GET *http_get){
	
	char buf[2*BUFSIZ], protocol_string[256], hostname_string[256], request_string[BUFSIZ];
	int i = 0, buf_len = 0;
	bool protocol_is_set = false, hostname_is_set = false;
	
	while (http_string[i] != '\0'){
		if (!protocol_is_set) {		
			if (http_string[i] == ':' && http_string[i + 1] == '/' && http_string[i + 2] == '/') {
				strncpy(protocol_string, buf, buf_len);
				memset(buf, 0, buf_len);
				buf_len = 0;
				protocol_is_set = true;
				i++; i++; i++;
			}	
		}

		if (protocol_is_set && !hostname_is_set) {
			if (http_string[i] == '/' || http_string[i + 1] == '\0') {
				strncpy(hostname_string, buf, buf_len);				
				memset(buf, 0, buf_len);
				buf_len = 0;
				hostname_is_set = true;
				i++;				
			}			
		}
		if (hostname_is_set) {
			if (http_string[i + 1] == '\0') {
				strncpy(request_string, buf, buf_len);				
			}			
		}		

		buf[buf_len] = http_string[i];
		i++; buf_len++;
	}

	printf("PROTO_STRING: %s\n", protocol_string);
	printf("HOSTNAME_STRING: %s\n", hostname_string);
	printf("REQUEST_STRING: %s\n", request_string);
	
	if (strcmp(protocol_string, "http") == 0) {
		http_get->protocol = HTTP;
		http_get->port = 80;
	}
	else if (strcmp(protocol_string, "https") == 0){
		http_get->protocol = HTTPS;
		http_get->port = 443;
	}
	//else {
	//	fprintf(stderr, "Error. Cant parse connection protocol from string: %s\n", http_string);
	//	return HTTP_CONNECTOR_ERROR_PROTOCOL;
	//}

	//if (strlen(hostname_string) > 0) {
		strncpy(http_get->hostname, hostname_string, 256);
		http_get->hostname[255] = '\0';
	//}
	//else {
	//	fprintf(stderr, "Error. Cant parse hostname from string: %s\n", http_string);
	//	return HTTP_CONNECTOR_ERROR_HOSTNAME;	
	//}

	if (strlen(request_string) > 0) {
		strncpy(http_get->request, hostname_string, BUFSIZ);
		http_get->request[BUFSIZ - 1] = '\0';		
	}	

	printf("HOSTNAME: %s\n", http_get->hostname);

	return 0;
}

void _handle_with_ssl_error(SSL *ssl, int retval){
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

int http_connector(char *http_string, void *data, int (*callback)(char*,int,int*,void*)){
	//parse HTTP string
	HTTP_GET *http_get = malloc(sizeof(HTTP_GET));
	parse_http_string(http_string, http_get);
	//if (parse_http_string(http_string, &http_get)){
		//fprintf(stderr, "Error to parse http_string: %s\n", http_string);	
		//return -1;
	//}
	printf("HOSTNAME: %s\n", http_get->hostname);

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
 
	if ( (host = gethostbyname(http_get->hostname)) == NULL ){
		fprintf(stderr, "Error. Can't get host ip address with hostname: %s\n", http_get->hostname);	
		return -1;
	} 
	int sd = socket(PF_INET, SOCK_STREAM, 0); //init socket
	//bzero(&addr, sizeof(addr));
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(http_get->port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);
	if ( connect(sd, (struct sockaddr*)&addr, sizeof addr)){ //connect socket
		fprintf(stderr, "Error. Can't connect to socket with address: %s:%d\n", host->h_addr, http_get->port);	
		return -1;
	} 
	
	SSL *ssl = SSL_new(ctx); //create ssl structure
	SSL_set_fd(ssl, sd); //connect SSL to socket 
	if ( SSL_connect(ssl) == -1 ){
		fprintf(stderr, "Error. Can't connect SSL to socket with address: %s:%d\n", host->h_addr, http_get->port);	
		return -1;
	}  

	//generage HTTP REQUEST MESSAGE
	char write_buf[2*BUFSIZ];
	sprintf(write_buf, "GET");
	if (snprintf(write_buf, BUFSIZ, "%s %s HTTP/1.1\r\n", write_buf, http_get->request) == -1){
		fprintf(stderr, "Error. Can't merge http request for host: %s with http request: GET %s HTTP/1.1\n", http_get->hostname, http_get->request);	
		return -1;		
	}

	//SSL WRITE
	int retval;
	retval = SSL_write(ssl, write_buf, strlen(write_buf));

	free(write_buf); //no need any more

	if (retval <= 0 ){ //handle with error
		_handle_with_ssl_error(ssl, retval);
		fprintf(stderr, "Error while SSL_write\n");
		return retval;			
	}

	//SSL READ
	int count = 0;
	char buf[1024];
	long bytes;
	while ((bytes = SSL_read(ssl, buf, sizeof buf)) >0 ) {
		buf[bytes] = 0;
		
		printf("%s", buf); //print for debug
		
		//if (callback) {
			//int c = callback(buf, bytes, &count, data); //run callback
			//if (c != 0) { //stop function if callback returned non zero
				//fprintf(stderr, "Stop SSL_read - callback returned: %d\n", c);
				//break;
			//}
			//count++; //we need count to know how many times callback was called
		//}
	}
	if (bytes < 0 ){ //hendle with error
		_handle_with_ssl_error(ssl, bytes);
		fprintf(stderr, "Error while SSL_read\n");
		return retval;			
	}	

	//Close SSL
	SSL_free(ssl);   
	close(sd);      
	SSL_CTX_free(ctx);   


	return 0;
}

