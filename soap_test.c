/**
 * File              : soap_test.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 14.10.2021
 * Last Modified Date: 15.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "soap_test.h"
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
#include <netdb.h>
#endif


#define HOST "nsi.rosminzdrav.ru"
#define PORT 443

int soap_test(){

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
 
    if ( (host = gethostbyname(HOST)) == NULL ){
		fprintf(stderr, "Error. Can't get host ip address with hostname: %s\n", HOST);	
		return -1;
	} 
    int sd = socket(PF_INET, SOCK_STREAM, 0); //init socket
    //bzero(&addr, sizeof(addr));
	memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof addr)){ //connect socket
		fprintf(stderr, "Error. Can't connect to socket with address: %s:%d\n", host->h_addr, PORT);	
		return -1;
	} 
    
    SSL *ssl = SSL_new(ctx); //create ssl structure
    SSL_set_fd(ssl, sd); //connect SSL to socket 
    if ( SSL_connect(ssl) == -1 ){
		fprintf(stderr, "Error. Can't connect SSL to socket with address: %s:%d\n", host->h_addr, PORT);	
		return -1;
	}  

	//generage HTTP REQUEST MESSAGE
	char *write_buf = "PUT";
	//SSL WRITE
	int retval;
	retval = SSL_write(ssl, write_buf, strlen(write_buf));


    //SSL READ
	int count = 0;
    char buf[1024];
	long bytes;
    while ((bytes = SSL_read(ssl, buf, sizeof buf)) >0 ) {
		buf[bytes] = 0;
		
		printf("%s", buf); //print for debug
	}
		
	//Close SSL
    SSL_free(ssl);   
    close(sd);      
    SSL_CTX_free(ctx);   
	

	return 0;

}
