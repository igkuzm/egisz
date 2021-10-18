/**
 * File              : egisz.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 11.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "egisz.h"
#include "cJSON.h"
#include "ezxml.h"
#include "rest_service_NR.h"
#include "url_connector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void print_json(cJSON *json){
	if (cJSON_IsArray(json)) {
		printf("JSON is array \n");		
		int i;
		int	count = cJSON_GetArraySize(json);
		cJSON *item = json->child;	
		for (i = 0; i < count; ++i) {
			printf("ITEM %d: %s\n", i, cJSON_Print(item));
			item = item->next;
		}
		printf("ARRAY COUNT: %d\n", cJSON_GetArraySize(json));		
	}
	else if (cJSON_IsObject(json)) {
		printf("JSON is object\n");		
		printf("OBJECT: %s\n", cJSON_Print(json));		
	}
	else if (cJSON_IsObject(json)) {
		printf("JSON is object\n");		
		printf("OBJECT: %s\n", cJSON_Print(json));		
	}	
	else if (cJSON_IsNumber(json)){
		printf("JSON is number\n");		
		printf("NUMBER: %s\n", cJSON_Print(json));		
	}	
	else if (cJSON_IsString(json)){
		printf("JSON is string\n");		
		printf("STRING: %s\n", cJSON_Print(json));		
	}		
	else if (cJSON_IsBool(json)){
		printf("JSON is bool\n");		
		printf("BOOL: %s\n", cJSON_Print(json));		
	}			
	
	else {
		printf("JSON: %s\n", cJSON_Print(json));		
	}
}

char *readString(){
	int i = 0;
	char *a = (char *) calloc(BUFSIZ, sizeof(char));
	if (!a) {
		fprintf(stderr, "ERROR. Cannot allocate memory\n");		
		return NULL;
	}										
	
	while (1) {
		scanf("%c", &a[i]);
		if (a[i] == '\n') {
			break;
		}
		else {
			i++;
		}
	}
	a[i] = '\0';
	return a;
}

int main(int argc, char *argv[])
{
	//cJSON *json = egisz_rest_refs_list();
	//cJSON *json = egisz_rest_refs_parts(15);
	//cJSON *json = egisz_rest_depart_part(1, 10);
	//cJSON *json = egisz_rest_org_part(2, 1);
	//cJSON *json = egisz_rest_org_get("1.2.643.5.1.13.13.12.1.42.202");		
	//cJSON *json = egisz_rest_depart_get("1.2.643.5.1.13.13.12.2.51.4977.0.2787.2107");		
	//cJSON *json = egisz_mo("1.2.643.5.1.13.13.12.1.42.202");		
	//cJSON *json = egisz_rest_lic_get(1);		
	//print_json(json);

	
	URLRequest *request = url_request_new_with_string("https://nsi.rosminzdrav.ru/wsdl/SOAP-server.v2.php");
	url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_Connection, "close");
	url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_Host, "nsi.rosminzdrav.ru");
	url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_SOAPAction, "getRefbookList");
	url_request_set_http_method(request, HTTP_METHOD_POST);
	

char *msg = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"<SOAP-ENV:Envelope\r\n"
"    xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\"\r\n"
"    xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\"\r\n"
"    xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\r\n"
"    xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\r\n"
"    xmlns:ns1=\"urn:service\">\r\n"
" <SOAP-ENV:Body>\r\n"
"  <ns1:getRefbookList>\r\n"
"   <userKey></userKey>\r\n"
"  </ns1:getRefbookList>\r\n"
" </SOAP-ENV:Body>\r\n"
"</SOAP-ENV:Envelope>\r\n";

	url_request_set_http_body_from_string(request, "text/xml;charset=UTF-8", msg);	

	printf("HOST: %s\n", request->hostname);
	printf("PORT: %d\n", request->port);
	printf("METHOD: %d\n", request->method);
	printf("PROTOCOL: %d\n", request->protocol);
	HTTPHeaderItemList *list = request->headerItemList;
	while (list->prev != NULL) {
		HTTPHeaderItem *item = list->headerItem;
		printf("HEADER ITEM: %d: %s\n", item->key, item->value);
		list = list->prev;
	}
	printf("BODY: %s\n", request->httpBody);

	URLConnectAnswer *answer = url_connect_answer_new();
	
	url_connection_send_request(request,  answer, url_connect_answer_callback);

	printf("HEADER: %s\n", answer->header);
	printf("BODY: %s\n", answer->body);


	//ezxml_t ezxml = ezxml_parse_str(answer->body, strlen(answer->body));
	//printf("EZXML NAME: %s\n", ezxml->name);
	
	

	url_request_free(request);
	url_connect_answer_free(answer);

	printf("Press any key to close application\n");
	getchar();

	return 0;
}
