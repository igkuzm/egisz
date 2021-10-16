/**
 * File              : http_connector.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.10.2021
 * Last Modified Date: 16.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

typedef enum {
	URL_CONNECTOR_ERROR_PROTOCOL = 1,
	URL_CONNECTOR_ERROR_HOSTNAME
} URL_CONNECTION_ERROR;

typedef enum {
	URL_CONNECTION_PROTOCOL_HTTP,
	URL_CONNECTION_PROTOCOL_HTTPS,
} URL_CONNECTION_PROTOCOL;

typedef enum {
	HTTP_HEADER_ITEM_KEY_Content_Length,
	HTTP_HEADER_ITEM_KEY_Content_Type,
	HTTP_HEADER_ITEM_KEY_Authorization,
	HTTP_HEADER_ITEM_KEY_SoapAction,
	HTTP_HEADER_ITEM_KEY_Connection,
	HTTP_HEADER_ITEM_KEY_Host,
	HTTP_HEADER_ITEM_KEY_Proxy_Authenticate,
	HTTP_HEADER_ITEM_KEY_Proxy_Authorization,
	HTTP_HEADER_ITEM_KEY_WWW_Authenticate
} HTTP_HEADER_ITEM_KEY;

typedef struct{
	HTTP_HEADER_ITEM_KEY key;
	const char value[256];
} HTTPHeaderItem;

typedef struct httpHeaderItemList{
	HTTPHeaderItem *headerItem;
	struct httpHeaderItemList *next;
	struct httpHeaderItemList *prev;
} HTTPHeaderItemList;

typedef struct {
	URL_CONNECTION_PROTOCOL protocol;
	char httpMethod[128];
	char hostname[256];
	int port;
	char requestString[BUFSIZ];
	char *httpBody;
	HTTPHeaderItemList *headerItemList;
} URLRequest;

URLRequest *url_request_new();
URLRequest *url_request_new_with_string(const char *url_string);
void url_request_free(URLRequest *urlRequest);

void url_request_set_http_method(URLRequest *urlRequest, const char *method);
void url_request_add_header_item(URLRequest *urlRequest, const char *key, const char *value);

void url_request_set_http_body_from_file(URLRequest *urlRequest, const char *filename);


int url_connection_send_request(URLRequest *request, void *data, int (*callback)(char*,int,int*,void*));
