/**
 * File              : http_connector.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.10.2021
 * Last Modified Date: 15.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

typedef enum {
	HTTP_CONNECTOR_ERROR_PROTOCOL = 1,
	HTTP_CONNECTOR_ERROR_HOSTNAME
} Http_connector_error;


typedef enum {
	HTTP,
	HTTPS
} Http_protocol;

typedef struct {
	Http_protocol protocol;
	char hostname[256];
	int port;
	char request[BUFSIZ];
} HTTP_GET;

int http_connector(char *http_string, void *data, int (*callback)(char*,int,int*,void*));
int parse_http_string(const char *http_string, HTTP_GET *http_get);
