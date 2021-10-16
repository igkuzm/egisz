/**
 * File              : http_connector.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.10.2021
 * Last Modified Date: 16.10.2021
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

const char *HTTPHeaderItemKey(HTTP_HEADER_ITEM_KEY key)
{
    switch (key)
    {
		case HTTP_HEADER_ITEM_KEY_Content_Length: return "Content-Length";
		case HTTP_HEADER_ITEM_KEY_Content_Type: return "Content-Type";
		case HTTP_HEADER_ITEM_KEY_Authorization: return "Authorization";
		case HTTP_HEADER_ITEM_KEY_SOAPAction: return "SOAPAction";
		case HTTP_HEADER_ITEM_KEY_Connection: return "Connection";
		case HTTP_HEADER_ITEM_KEY_Host: return "Host";
		case HTTP_HEADER_ITEM_KEY_Proxy_Authenticate: return "Proxy-Authenticate";
		case HTTP_HEADER_ITEM_KEY_Proxy_Authorization: return "Proxy-Authorization";
		case HTTP_HEADER_ITEM_KEY_WWW_Authenticate: return "WWW-Authenticate";
    }
}

URLRequest *url_request_new(){
	URLRequest *request = malloc(sizeof(URLRequest));
	if (request == NULL) {
		fprintf(stderr, "Cannot allocate memory for URLRequest\n");
		return NULL;
	}
	HTTPHeaderItemList *headerItemList = malloc(sizeof(HTTPHeaderItemList));
	if (headerItemList == NULL) {
		fprintf(stderr, "Cannot allocate memory for HTTPHeaderItemList\n");
		return NULL;
	}
	
	headerItemList->prev = headerItemList->next = NULL;
	request->headerItemList = headerItemList;
	strcpy(request->httpMethod, "GET");

	return request;
}


URLRequest *url_request_new_with_string(const char *url_string){

	URLRequest *request = url_request_new();
	
	char buf[2*BUFSIZ], protocol_string[256], hostname_string[256], request_string[BUFSIZ];
	memset(protocol_string, 0, 256);
	memset(hostname_string, 0, 256);
	memset(request_string, 0, BUFSIZ);

	int i = 0, buf_len = 0;
	bool protocol_is_set = false, hostname_is_set = false;
	
	while (url_string[i] != '\0'){
		if (!protocol_is_set) {		
			if (url_string[i] == ':' && url_string[i + 1] == '/' && url_string[i + 2] == '/') {
				strncpy(protocol_string, buf, buf_len);
				memset(buf, 0, buf_len);
				buf_len = 0;
				protocol_is_set = true;
				i++; i++; i++;
			}	
		}

		if (protocol_is_set && !hostname_is_set) {
			if (url_string[i] == '/') {
				strncpy(hostname_string, buf, buf_len);				
				memset(buf, 0, buf_len);
				buf_len = 0;
				hostname_is_set = true;
				i++;				
			}			
		}
		if (hostname_is_set) {
			if (url_string[i + 1] == '\0') {
				buf[buf_len] = url_string[i];
				i++; buf_len++;
				strncpy(request_string, buf, buf_len);				
			}			
		}		

		buf[buf_len] = url_string[i];
		i++; buf_len++;
	}

	if (strncmp(protocol_string, "http", 4) == 0) {
		request->protocol = URL_CONNECTION_PROTOCOL_HTTP;
		request->port = 80;
	}
	if (strncmp(protocol_string, "https", 5) == 0){
		request->protocol = URL_CONNECTION_PROTOCOL_HTTPS;
		request->port = 443;
	}
	if (!request->port){
		fprintf(stderr, "Error. Cant parse connection protocol from string: %s\n", url_string);
		return NULL;
	}

	if (strlen(hostname_string) > 0) {
		strncpy(request->hostname, hostname_string, 256);
		request->hostname[255] = '\0';
	}
	else {
		fprintf(stderr, "Error. Cant parse hostname from string: %s\n", url_string);
		return NULL;	
	}

	if (strlen(request_string) > 0) {
		strncpy(request->requestString, request_string, BUFSIZ);
		request->requestString[BUFSIZ - 1] = '\0';		
	}	

	return request;
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

int socket_for_url_request(URLRequest *request){
	struct hostent *host;
	struct sockaddr_in addr;
 
	if ( (host = gethostbyname(request->hostname)) == NULL ){
		fprintf(stderr, "Error. Can't get host ip address with hostname: %s\n", request->hostname);	
		return -1;
	} 
	int sd = socket(PF_INET, SOCK_STREAM, 0); //init socket
	//bzero(&addr, sizeof(addr));
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(request->port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);
	if ( connect(sd, (struct sockaddr*)&addr, sizeof addr)){ //connect socket
		fprintf(stderr, "Error. Can't connect to socket with address: %s:%d\n", host->h_addr, request->port);	
		return -1;
	} 

	return sd;
}

char *message_for_url_request(URLRequest *request){
	char *write_buf = malloc(BUFSIZ * sizeof(char));
	if (write_buf == NULL) {
		fprintf(stderr, "Cannot allocate memory for message_for_url_request\n");
		return NULL;
	}	
	int ret;
	
	ret = sprintf(write_buf, "%s", request->httpMethod);
	if (ret == -1) {
		fprintf(stderr, "Error. Can't merge http request with http method: %s\n", request->httpMethod);	
		return NULL;
	}
	
	ret = sprintf(write_buf, "%s %s HTTP/1.1\r\n", write_buf, request->requestString);
	if (ret == -1) {
		fprintf(stderr, "Error. Can't merge http request: %s\n", request->requestString);	
		return NULL;
	}	

	HTTPHeaderItemList *headerItemList = request->headerItemList;
	while (headerItemList->prev != NULL) {
		HTTPHeaderItem *item = headerItemList->headerItem;
		ret = sprintf(write_buf, "%s%s: %s\r\n", write_buf, HTTPHeaderItemKey(item->key), item->value);
		if (ret == -1) {
			fprintf(stderr, "Error. Can't merge http request with header item: %s: %s\n", HTTPHeaderItemKey(item->key), item->value);	
			return NULL;
		}	
	}
	
	ret = sprintf(write_buf, "%s\r\n", write_buf);
	if (ret == -1) {
		fprintf(stderr, "Error. Can't merge http request: %s\n", write_buf);	
		return NULL;
	}		

	if (request->httpBody) {
		ret = sprintf(write_buf, "%s%s\r\n", write_buf, request->httpBody);
		if (ret == -1) {
			fprintf(stderr, "Error. Can't merge http request with http body\n");	
			return NULL;
		}		
	}

	return write_buf;
}

SSL *ssl_init(){
	//init SSL
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	const SSL_METHOD *method = SSLv23_client_method();
	SSL_CTX *ctx = SSL_CTX_new(method);
	if ( ctx == NULL ){
		fprintf(stderr, "Error. Can't init SSL_CTX\n");	
		return NULL;
	} 
	
	SSL *ssl = SSL_new(ctx); //create ssl structure
	SSL_set_fd(ssl, sd); //connect SSL to socket 
	if ( SSL_connect(ssl) == -1 ){
		fprintf(stderr, "Error. Can't connect SSL to socket with address: %s:%d\n", host->h_addr, http_get->port);	
		return NULL;
	}  
	
	//SSL WRITE
	int retval;
	retval = SSL_write(ssl, write_buf, strlen(write_buf));

	if (retval <= 0 ){ //handle with error
		_handle_with_ssl_error(ssl, retval);
		fprintf(stderr, "Error while SSL_write\n");
		return retval;			
	}

	return ssl;
}

int url_connection_send_request(URLRequest *request, void *data, int (*callback)(char*,int,int*,void*)){
	//int socket
	int sd = socket_for_url_request(request);
	if (sd == -1) {
		perror("Error in function socket_for_url_request()");
		return -1;
	}
	 
	
	//generage HTTP REQUEST MESSAGE
	char *write_buf = message_for_url_request(request);	
	if (write_buf == NULL) {
		perror("Error in function socket_for_url_request()");
		return -1;
	}
	printf("REQUEST MESSAGE: %s\n", write_buf);
	

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
			return bytes;			
		}	

		//Close SSL
		SSL_free(ssl);   
		close(sd);      
		SSL_CTX_free(ctx);   
	}
	
	else { //NO SSL
		//WRITE
		int retval;
		retval = write(sd, write_buf, strlen(write_buf));

		if (retval <= 0 ){ //handle with error
			return retval;			
		}

		//READ
		int count = 0;
		char buf[1024];
		long bytes;
		while ((bytes = read(sd, buf, sizeof buf)) >0 ) {
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
			return bytes;			
		}
	}
	


	return 0;
}

