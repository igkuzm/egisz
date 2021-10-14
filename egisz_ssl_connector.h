/**
 * File              : egisz_ssl_connector.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 12.10.2021
 * Last Modified Date: 14.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

int egisz_ssl_connector_bio(const char *hostname, int port, const char *apikey, const char *http_method, int argc, char *argv[], void *data, int (*callback)(char*,int,int*,void*));
//connector for egisz returns 0 on success, or error int
int egisz_ssl_connector_socket(
		const char *host, //host name (without http:// or https://) 
		int port, //posrt number - 80 for http, 443 for https 
		const char *apikey, //apikey for authendication 
		const char *method, //HTTP method with string - eg. GET 
		int argc, //number of arguments of http request 
		char *argv[], //an array of arguments of http request - eg. id=13
		void *data, //data for callback 
		int (*callback)(char *buf, //buffer string from SSL BIO_read - to stop function, return non-zero from callback
						int bufsize, //size of buf
						int *count, //counter of returned string
						void *data //data for callback
						)
		);

//struct to get answer from HTTP GET request of REST service
typedef struct{
	char header[1024];
	int len_header;
	char content_type[256];
	char transfer_encoding[256];
	int	content_length;
	char *body; //allocates and frees automaticaly
	int len_body;
	int chunked_size;
	int chunked_len;
} EgiszSSLConnectorAnswer;

//allocate struct
EgiszSSLConnectorAnswer *egisz_ssl_connector_answer_new();

//free struct
void egisz_ssl_connector_answer_free(EgiszSSLConnectorAnswer *answer);

//callback for egisz_ssl_connector
int egisz_ssl_connector_answer_callback(char *str, int len, int *count, void *answer);
