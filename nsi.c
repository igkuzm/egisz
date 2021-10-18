/**
 * File              : nsi.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "nsi.h"
#include "url_connector.h"

#define HOST "nsi.rosminzdrav.ru"
#define PORT 443

URLRequest *url_request_prepare(){
	URLRequest *request = url_request_new();
	url_request_set_port(request, PORT);
	url_request_set_hostname(request, HOST);
	url_request_set_http_method(request, HTTP_METHOD_POST);
	url_request_set_url_connection_protocol(request, URL_CONNECTION_PROTOCOL_HTTPS);
	url_request_set_request_string(request, "wsdl/SOAP-server.v2.php");
	
	url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_Connection, "close");

	return request;
}



ezxml_t egisz_nsi_get_server_time(){
	URLRequest *request = url_request_prepare(); 
	char *msg = 
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
	"<SOAP-ENV:Envelope\r\n"
	" xmlns:SOAP-ENV=\"http:schemas.xmlsoap.org/soap/envelope/\"\r\n"
	" xmlns:SOAP-ENC=\"http:schemas.xmlsoap.org/soap/encoding/\"\r\n"
	" xmlns:xsi=\"http:www.w3.org/2001/XMLSchema-instance\"\r\n"
	" xmlns:xsd=\"http:www.w3.org/2001/XMLSchema\"\r\n"
	" xmlns:ns1=\"urn:service\">\r\n"
	" <SOAP-ENV:Body>\r\n"
	"  <ns1:getServerTime>\r\n"
	"  </ns1:getServerTime>\r\n"
	" </SOAP-ENV:Body>\r\n"
	"</SOAP-ENV:Envelope>\r\n";
	url_request_set_http_body_from_string(request, "text/xml;charset=UTF-8", msg);
}


