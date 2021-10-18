/**
 * File              : nsi.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "nsi.h"
#include <string.h>
#include "url_connector.h"

#define HOST "nsi.rosminzdrav.ru"
#define PORT 443

URLRequest *nsi_url_request_prepare(){
	URLRequest *request = url_request_new();
	url_request_set_port(request, PORT);
	url_request_set_hostname(request, HOST);
	url_request_set_http_method(request, HTTP_METHOD_POST);
	url_request_set_url_connection_protocol(request, URL_CONNECTION_PROTOCOL_HTTPS);
	url_request_set_request_string(request, "wsdl/SOAP-server.v2.php");
	
	url_request_set_header_item(request, HTTP_HEADER_ITEM_KEY_Connection, "close");
	url_request_set_header_item(request, HTTP_HEADER_ITEM_KEY_Host, HOST);
	url_request_set_header_item(request, HTTP_HEADER_ITEM_KEY_SOAPAction, "");

	return request;
}

ezxml_t nsi_xml_from_url_connection_send_request(URLRequest *request){
	URLConnectAnswer *answer = url_connect_answer_new();
	if (url_connection_send_request(request, answer, url_connect_answer_callback)){
		fprintf(stderr, "Error in function url_connection_send_request\n");
		return NULL;
	}
	
	printf("HEADER: %s\n", answer->header);
	printf("BODY: %s\n", answer->body);
	
	ezxml_t ezxml = ezxml_parse_str(answer->body, strlen(answer->body));	

	url_connect_answer_free(answer);
	url_request_free(request);
	return ezxml;
}


time_t egisz_nsi_get_server_time(){
	URLRequest *request = nsi_url_request_prepare(); 
	char *msg = 
	"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:urn=\"urn:service\">\r\n"
	"   <soapenv:Header/>\r\n"
	"   <soapenv:Body>\r\n"
	"	  <urn:getServerTime>\r\n"
	"	  </urn:getServerTime>\r\n"
	"   </soapenv:Body>\r\n"
	"</soapenv:Envelope>\r\n";
		
	url_request_set_http_body_from_string(request, "text/xml;charset=UTF-8", msg);

	ezxml_t envelop = nsi_xml_from_url_connection_send_request(request); 
	ezxml_t child = envelop->child;
	ezxml_t body;
	while (child != NULL) {
		char *name = child->name;
		if (strcmp(name, "soap:Body") == 0) {
			body = child;
		}
		child = child->child;
	}
	ezxml_t getServerTimeResponse = body->child;
	ezxml_t getServerTimeReturn = getServerTimeResponse->child;
	char *time_str = getServerTimeReturn->txt;
	time_t time = atoi(time_str);
	return time;
}

ezxml_t egisz_nsi_get_refbook_list(){
	URLRequest *request = nsi_url_request_prepare(); 
	char *msg = 
	"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:urn=\"urn:service\">\r\n"
	"   <soapenv:Header/>\r\n"
	"   <soapenv:Body>\r\n"
	"	  <urn:getRefbookList>\r\n"
    "		<userKey1></userKey1>\r\n"	
	"	  </urn:getRefbookList>\r\n"
	"   </soapenv:Body>\r\n"
	"</soapenv:Envelope>\r\n";
		
	url_request_set_http_body_from_string(request, "text/xml;charset=UTF-8", msg);

	return nsi_xml_from_url_connection_send_request(request); 
}


