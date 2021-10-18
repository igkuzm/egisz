/**
 * File              : nsi.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "nsi.h"
#include "url_connector.h"

URLRequest *url_request_prepare(){
	URLRequest *request = url_request_new_with_string("https://nsi.rosminzdrav.ru/wsdl/SOAP-server.v2.php");
	url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_Connection, "close");
	//url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_Host, "nsi.rosminzdrav.ru");
	//url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_SOAPAction, "getRefbookList");
	url_request_set_http_method(request, HTTP_METHOD_POST);
}



ezxml_t egisz_nsi_get_server_time(){
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
	

}

