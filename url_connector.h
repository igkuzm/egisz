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

//URL Request
typedef struct {
	URL_CONNECTION_PROTOCOL protocol; //connection protocol - eg: HTTP, HTTPS - to set use url_request_set_url_connection_protocol
	HTTP_METHOD method; //HTTP method - eg.: GET, POST - to set use url_request_set_http_method
	char hostname[256]; //hostname - to set use url_request_set_hostname
	int port; //port to connect - eg. 80, 443 - to set use url_request_set_port
	char requestString[BUFSIZ]; //request string - to set use url_request_set_request_string
	char *httpBody; //pointer to httpBody - to set body use url_request_set_http_body_from_string, or url_request_set_http_body_from_file
	HTTPHeaderItemList *headerItemList; //list of header Items - to add item use: url_request_add_header_item
} URLRequest;

URLRequest *url_request_new(); //create new url request
URLRequest *url_request_new_with_string(const char *url_string); //create new url GET request from string - eg. http://example.com/?username=user&password=password
void url_request_free(URLRequest *request); //free memory

void url_request_set_url_connection_protocol(URLRequest *request, URL_CONNECTION_PROTOCOL protocol);
void url_request_set_http_method(URLRequest *request, HTTP_METHOD method);
void url_request_set_hostname(URLRequest *request, char *hostname);
void url_request_set_port(URLRequest *request, int port);
void url_request_set_request_string(URLRequest *request, char *requestString);
void url_request_add_header_item(URLRequest *request, HTTP_HEADER_ITEM_KEY key, const char *value);

void url_request_set_http_body_from_string(URLRequest *request, const char *contentType, char *bodyString);
void url_request_set_http_body_from_file(URLRequest *request, const char *contentType, const char *filename);

int url_connection_send_request(URLRequest *request, void *data, int (*callback)(char*,int,int*,void*)); //perform connection

//struct to get answer from url_connection_send_request
typedef struct{
	char header[1024]; //header of answer
	int len_header; //size of header
	char content_type[256]; //type of content - eg. json
	char transfer_encoding[256]; //encoding
	int	content_length; //length of content
	char *body; //allocates and frees automaticaly
	int len_body;
} URLConnectAnswer;

URLConnectAnswer *url_connect_answer_new();
void url_connect_answer_free(URLConnectAnswer *answer);

int url_connect_answer_callback(char *str, int len, int *count, void *_answer);

