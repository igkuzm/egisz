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
} Http_connector_error;

typedef enum {
	GET,
	POST
} HttpMethod;

int http_connector(const char *http_string, HttpMethod *method, void *data, int (*callback)(char*,int,int*,void*));
