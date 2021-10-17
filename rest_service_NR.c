/**
 * File              : rest_service_NR.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 12.10.2021
 * Last Modified Date: 17.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/**
 * rest_service_NR.c
 * Copyright (c) 2021 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "rest_service_NR.h"
#include "url_connector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define HOST "nr.egisz.rosminzdrav.ru"
#define PORT 443
#define APIKEY ""


//struct to get answer from HTTP GET request of REST service
typedef struct{
	char header[1024];
	int len_header;
	char content_type[256];
	char transfer_encoding[256];
	int	content_length;
	char *body; //allocates and frees automaticaly
	int len_body;
} RestServiceAnswer;

RestServiceAnswer *rest_service_answer_new(){
	RestServiceAnswer *answer = malloc(sizeof(RestServiceAnswer));
	if (answer == NULL) {
		fprintf(stderr, "Cannot allocate memory for RestServiceAnswer\n");
		exit(ENOMEM);
	}
	answer->body = malloc(BUFSIZ*sizeof(char));
	if (answer->body == NULL) {
		fprintf(stderr, "Cannot allocate memory for RestServiceAnswer->body\n");
		exit(ENOMEM);
	}	
	return answer;
}

void rest_service_answer_free(RestServiceAnswer *answer){
	free(answer->body);
	free(answer);
}

int rest_service_answer_callback(char *str, int len, int *count, void *_answer){
	RestServiceAnswer *answer = _answer;
	
	if (*count == 0) { //find header
		int len_header = 0, len_line = 0;
		int i;
		char line[BUFSIZ];
		for (i = 0; i < len; ++i) {
			if (str[i] == '\n' && str[i+1] == '\r') {//new line with \r is header stop
				len_header = i;
			}
			if (str[i] == '\n') { //new line in header
				//find header arguments
				sscanf(line, " Content-Type: %s", answer->content_type);
				sscanf(line, " Content-Length: %d", &answer->content_length);
				sscanf(line, " Transfer-Encoding: %s", answer->transfer_encoding);
				len_line = 0;
				line[0] = '\0';
			}			
			else {
				line[len_line] = str[i];
				len_line++;	
			}
		}
		answer->len_header = len_header;
		strncpy(answer->header, str, len_header);

		//remove header, and continue to parce answer
		answer->len_body = len - len_header;
		for (i = 0; i < len_header + 1; ++i) {
			memmove(&str[0],&str[1],len-1);
			len--;
		}
	}

	////////////////////////////
	if (answer->content_length > 0) { //OK - we have a standart content
		if (answer->content_length > answer->len_body) {
			answer->body = realloc(answer->body, answer->content_length * sizeof(char)); //realoc body string to match content length
			if (answer->body == NULL) {
				fprintf(stderr, "Cannot reallocate memory for EgiszSSLConnectorAnswer->body\n");
				exit(ENOMEM);
			}		
		}
		answer->len_body = answer->content_length;
		strncat(answer->body, str, len); //cat string to body
	}

	////////////////////////////
	if (strcmp(answer->transfer_encoding, "chunked") == 0) { //We have a chunked content
		//lets find size of chunked
		bool we_have_chunked_size = false, chunked_size_is_zero = false;
		int start_of_chunked = 0;
		int i;
		for (i = 0; i < len; ++i) {
			if (str[i] == '\r' && str[i+1] == '\n') { //chunked size goes after \r\n
				char chunked_size_str[5];
				chunked_size_str[0] = str[i+2];
				chunked_size_str[1] = str[i+3];
				chunked_size_str[2] = str[i+4];
				chunked_size_str[3] = str[i+5];
				chunked_size_str[4] = '\0';

				//get size integer
				int chunked_size;
				if (sscanf(chunked_size_str,"%x",&chunked_size) != 1) {
					fprintf(stderr, "Can't get chunked_size from string: %s\n", chunked_size_str);
				} 
				else { //good - we have a chunked size
					//printf("CHUNKED SIZE: %d\n", chunked_size);
					answer->len_body += chunked_size;
					answer->body = realloc(answer->body, answer->len_body * sizeof(char)); //realoc body string to add chunked_size
					if (answer->body == NULL) {
						fprintf(stderr, "Cannot reallocate memory for EgiszSSLConnectorAnswer->body\n");
						exit(ENOMEM);
					}		
					we_have_chunked_size = 1;
					start_of_chunked = i;
					i = len; //stop FOR cicle
					if (chunked_size == 0) {
						chunked_size_is_zero = true;	
					}
				}
			}
			else { //no chunked size this callback
			
			}
		}
		if (we_have_chunked_size) {
		//remove chunked_size string from str
			for (i = 0; i < 8; ++i) {
				memmove(&str[start_of_chunked],&str[start_of_chunked + 1],len-1);
				len--;
			}
		}
		if (chunked_size_is_zero) {
			len++;
		}
		//
		strncat(answer->body, str, len); //cat string to body			
		//
	}	

	return 0;
}

URLRequest *url_request_prepare(){
	URLRequest *request = url_request_new();
	url_request_set_url_connection_protocol(request, URL_CONNECTION_PROTOCOL_HTTPS);
	url_request_set_http_method(request, HTTP_METHOD_GET);
	url_request_set_hostname(request, HOST);
	url_request_set_port(request, PORT);

	url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_Accept, "application/json");
	url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_Host, HOST);
	url_request_add_header_item(request, HTTP_HEADER_ITEM_KEY_Connection, "close");

	return request;
}

cJSON *json_from_url_connection_send_request(URLRequest *request){
	RestServiceAnswer *answer = rest_service_answer_new();
	if (url_connection_send_request(request, answer, rest_service_answer_callback)){
		fprintf(stderr, "Error in function url_connection_send_request\n");
		return NULL;
	}
	cJSON *json = cJSON_Parse(answer->body);
	rest_service_answer_free(answer);
	return json;
}

///////////////////
cJSON *egisz_rest_refs_list(){
	URLRequest *request = url_request_prepare();
	url_request_set_request_string(request, "rest/refs/list");
	return json_from_url_connection_send_request(request);
}

cJSON *egisz_rest_refs_parts(int id){
	URLRequest *request = url_request_prepare();
	char requestString[256];	
	sprintf(requestString, "rest/refs/parts?id=%d", id);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);
}

cJSON *egisz_rest_refs_part(int id, int part){
	URLRequest *request = url_request_prepare();
	char requestString[256];	
	sprintf(requestString, "rest/refs/part?id=%d&part=%d", id, part);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}

///////////////////
cJSON *egisz_rest_org_parts(int orgTypeId){
	URLRequest *request = url_request_prepare();
	char requestString[256];	
	sprintf(requestString, "rest/org/parts?orgTypeId=%d", orgTypeId);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}

cJSON *egisz_rest_org_part(int orgTypeId, int part){
	URLRequest *request = url_request_prepare();
	char requestString[256];	
	sprintf(requestString, "rest/org/part?orgTypeId=%d&part=%d", orgTypeId, part);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}

cJSON *egisz_rest_org_get(char *oid){
	URLRequest *request = url_request_prepare();
	char requestString[256];	
	sprintf(requestString, "rest/org/get?oid=%s", oid);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}

///////////////////
cJSON *egisz_rest_depart_parts(int departTypeId){
	URLRequest *request = url_request_prepare();
	char requestString[256];	
	sprintf(requestString, "rest/depart/parts?departTypeId=%d", departTypeId);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}

cJSON *egisz_rest_depart_part(int departTypeId, int part){
	URLRequest *request = url_request_prepare();
	char requestString[256];	
	sprintf(requestString, "rest/depart/part?departTypeId=%d&part=%d", departTypeId, part);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}
cJSON *egisz_rest_depart_get(char *oid){
	URLRequest *request = url_request_prepare();
	char requestString[256];	
	sprintf(requestString, "rest/depart/get?oid=%s", oid);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}





cJSON *egisz_rest_mo(char *oid){
	char *method = "GET /rest/mo";
	char _oid[256];
	sprintf(_oid, "oid=%s", oid);
	char *argv[] = {_oid};	
	return json_from_egisz_ssl_connector_answer(method, 1, argv);
}


cJSON *egisz_rest_lic_get(int service){
	char *method = "GET /rest/lic/get";
	char _service[256];
	sprintf(_service, "service=%d", service);
	char *argv[] = {_service};	
	return json_from_egisz_ssl_connector_answer(method, 1, argv);
}


