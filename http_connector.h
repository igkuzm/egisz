/**
 * File              : http_connector.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.10.2021
 * Last Modified Date: 15.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

typedef enum {
	HTTP,
	HTTPS
} Http_protocol;

typedef struct {
	Http_protocol protocol;
	char hostname[256];
	char request[BUFSIZ];
} Http_method;

int parse_http_string(const char *http_string, Http_method *method);
