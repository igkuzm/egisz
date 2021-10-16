/**
 * File              : http_connector.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.10.2021
 * Last Modified Date: 16.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

typedef enum {
	HTTP_CONNECTOR_ERROR_PROTOCOL = 1,
	HTTP_CONNECTOR_ERROR_HOSTNAME
} URL_connection_error;

typedef enum {
	HTTP,
	HTTPS
} HttpProtocol;

typedef struct {
	HttpProtocol protocol;
	char hostname[256];
	int port;
	char request[BUFSIZ];
} HTTP_GET;


typedef enum {
	GET,
	POST
} HttpMethod;

int URL_connection(const char *http_string, HttpMethod *method, char *header, void *data, int (*callback)(char*,int,int*,void*));
