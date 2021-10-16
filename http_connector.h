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
	URL_CONNECTION_PROTOCOL_HTTP,
	URL_CONNECTION_PROTOCOL_HTTPS,
} URL_CONNECTION_PROTOCOL;

typedef enum {
	URL_CONNECTION_GET,
	URL_CONNECTION_POST
} URL_CONNECTION_METHOD;

typedef struct{
	const char value[256];
	const char key[256];
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

URLRequest *url_request_new();
URLRequest *url_request_new_with_string(char *url_string);
void url_request_free(URLRequest *urlRequest);
void url_request_add_header_item(URLRequest *urlRequest, const char *key, const char *value);



int URL_connection(const char *http_string, HttpMethod *method, char *header, void *data, int (*callback)(char*,int,int*,void*));
