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
} URL_CONNECTION_ERROR;

typedef enum {
	URLConnectionProtocolHTTP,
	URLConnectionProtocolHTTPS
} URL_CONNECTION_PROTOCOL;

typedef enum {
	GET,
	POST
} URL_CONNECTION_METHOD;

typedef struct{
	char value[256];
	char key[256];
} HTTPHeaderItem;

typedef struct {
	URL_CONNECTION_PROTOCOL protocol;
	URL_CONNECTION_PROTOCOL method;
	char hostname[256];
	int port;
	char request[BUFSIZ];
	char *httpBody;
	HTTPHeaderItem *headerItems;
	int *headerItemsCount;
} URLRequest;



int URL_connection(const char *http_string, HttpMethod *method, char *header, void *data, int (*callback)(char*,int,int*,void*));
