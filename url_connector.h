/**
 * File              : url_connector.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

typedef enum { //errors
	URL_CONNECTOR_ERROR_PROTOCOL = -14141,
	URL_CONNECTOR_ERROR_HOSTNAME = -14142,
	URL_CONNECTOR_ERROR_SOCKET = -14143,
	URL_CONNECTOR_ERROR_SSL_INIT = -14144,
	URL_CONNECTOR_ERROR_SSL_SOCKET = -14145,
	URL_CONNECTOR_ERROR_MESSAGE = -14146,
} URL_CONNECTION_ERROR;

typedef enum { //connection protocols
	URL_CONNECTION_PROTOCOL_HTTP,
	URL_CONNECTION_PROTOCOL_HTTPS,
} URL_CONNECTION_PROTOCOL;

typedef enum { //http connection methods
	HTTP_METHOD_GET,
	HTTP_METHOD_PUT,
	HTTP_METHOD_POST,
} HTTP_METHOD;

typedef enum  { //item keys for HTTP header
	HTTP_HEADER_ITEM_KEY_Content_Length,
	HTTP_HEADER_ITEM_KEY_Content_Type,
	HTTP_HEADER_ITEM_KEY_Authorization,
	HTTP_HEADER_ITEM_KEY_SOAPAction,
	HTTP_HEADER_ITEM_KEY_Connection,
	HTTP_HEADER_ITEM_KEY_Host,
	HTTP_HEADER_ITEM_KEY_Proxy_Authenticate,
	HTTP_HEADER_ITEM_KEY_Proxy_Authorization,
	HTTP_HEADER_ITEM_KEY_Accept,
	HTTP_HEADER_ITEM_KEY_WWW_Authenticate
} HTTP_HEADER_ITEM_KEY;

typedef struct{ //HTTP header item
	HTTP_HEADER_ITEM_KEY key;
	char value[256];
} HTTPHeaderItem;

typedef struct httpHeaderItemList{ //struct list to handle header items
	HTTPHeaderItem *headerItem;
	struct httpHeaderItemList *prev;
} HTTPHeaderItemList;

typedef struct {
	URL_CONNECTION_PROTOCOL protocol;
	HTTP_METHOD method;
	char hostname[256];
	int port;
	char requestString[BUFSIZ];
	char *httpBody;
	HTTPHeaderItemList *headerItemList;
} URLRequest;

URLRequest *url_request_new();
URLRequest *url_request_new_with_string(const char *url_string);
void url_request_free(URLRequest *request);

void url_request_set_url_connection_protocol(URLRequest *request, URL_CONNECTION_PROTOCOL protocol);
void url_request_set_http_method(URLRequest *request, HTTP_METHOD method);
void url_request_set_hostname(URLRequest *request, char *hostname);
void url_request_set_port(URLRequest *request, int port);
void url_request_set_request_string(URLRequest *request, char *requestString);
void url_request_add_header_item(URLRequest *request, HTTP_HEADER_ITEM_KEY key, const char *value);

void url_request_set_http_body_from_string(URLRequest *request, const char *contentType, char *bodyString);
void url_request_set_http_body_from_file(URLRequest *request, const char *contentType, const char *filename);


int url_connection_send_request(URLRequest *request, void *data, int (*callback)(char*,int,int*,void*));

//struct to get answer from url_connection_send_request
typedef struct{
	char header[1024];
	int len_header;
	char content_type[256];
	char transfer_encoding[256];
	int	content_length;
	char *body; //allocates and frees automaticaly
	int len_body;
} URLConnectAnswer;

URLConnectAnswer *url_connect_answer_new();
void url_connect_answer_free(URLConnectAnswer *answer);

