/**
 * File              : rest_service_NR.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 12.10.2021
 * Last Modified Date: 18.10.2021
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
#include "cJSON.h"
#include "url_connector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define HOST "nr.egisz.rosminzdrav.ru"
#define PORT 443


URLRequest *url_request_prepare(){
	URLRequest *request = url_request_new();
	url_request_set_url_connection_protocol(request, URL_CONNECTION_PROTOCOL_HTTPS);
	url_request_set_http_method(request, HTTP_METHOD_GET);
	url_request_set_hostname(request, HOST);
	url_request_set_port(request, PORT);

	url_request_set_header_item(request, HTTP_HEADER_ITEM_KEY_Connection, "close");
	url_request_set_header_item(request, HTTP_HEADER_ITEM_KEY_Accept, "application/json");
	url_request_set_header_item(request, HTTP_HEADER_ITEM_KEY_Host, HOST);

	return request;
}


cJSON *json_from_url_connection_send_request(URLRequest *request){
	URLConnectAnswer *answer = url_connect_answer_new();
	if (url_connection_send_request(request, answer, url_connect_answer_callback)){
		fprintf(stderr, "Error in function url_connection_send_request\n");
		return NULL;
	}
	
	//printf("HEADER: %s\n", answer->header);
	//printf("BODY: %s\n", answer->body);
	
	cJSON *json = cJSON_Parse(answer->body);
	url_connect_answer_free(answer);
	url_request_free(request);
	return json;
}

///////////////////
int egisz_rest_refs_list(struct egisz_rest_refs_list_struct **_array){
	URLRequest *request = url_request_prepare();
	url_request_set_request_string(request, "rest/refs/list");
	cJSON *json = json_from_url_connection_send_request(request);
	if (!json) {
		return -1;
	}
	if (!cJSON_IsArray(json)){
		return -1;
	}
	
	int i;
	int	count = cJSON_GetArraySize(json);
	struct egisz_rest_refs_list_struct *array = malloc(sizeof(struct egisz_rest_refs_list_struct));
	if (array == NULL) {
		fprintf(stderr, "Error allocate memory for egisz_rest_refs_list_struct\n");
		return -1;
	}

	cJSON *item = json->child;	
	for (i = 0; i < count; ++i) {
		struct egisz_rest_refs_list_struct dict;
		cJSON *id = cJSON_GetObjectItem(item, "id");
		dict.id = cJSON_GetNumberValue(id);
		//printf("ID: %d\n", dict.id);
		
		cJSON *refsName = cJSON_GetObjectItem(item, "refsName");
		strcpy(dict.refsName, cJSON_GetStringValue(refsName));
		//printf("NAME: %s\n", dict.refsName);

		array[i] = dict;
		
		item = item->next;
	}

	if (_array) {
		*_array = array;
	}
	else {
		free(array);
	}

	return count;
}

int egisz_rest_refs_parts(int id){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/refs/parts?id=%d", id);
	url_request_set_request_string(request, requestString);

	cJSON *json = json_from_url_connection_send_request(request);
	if (!json) {
		return -1;
	}
	if (!cJSON_IsNumber(json)){
		return -1;
	}

	return cJSON_GetNumberValue(json);
}

int egisz_rest_refs_part(struct egisz_rest_refs_part_struct **_array, int id, int part){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/refs/part?id=%d&part=%d", id, part);
	url_request_set_request_string(request, requestString);

	cJSON *json = json_from_url_connection_send_request(request);
	if (!json) {
		return -1;
	}
	if (!cJSON_IsArray(json)){
		return -1;
	}
	
	int i;
	int	count = cJSON_GetArraySize(json);
	struct egisz_rest_refs_part_struct *array = malloc(sizeof(struct egisz_rest_refs_list_struct));
	if (array == NULL) {
		fprintf(stderr, "Error allocate memory for egisz_rest_refs_list_struct\n");
		return -1;
	}

	cJSON *item = json->child;	
	for (i = 0; i < count; ++i) {
		struct egisz_rest_refs_part_struct dict;
		cJSON *id = cJSON_GetObjectItem(item, "id");
		dict.id = cJSON_GetNumberValue(id);
		
		cJSON *region = cJSON_GetObjectItem(item, "region");
		strcpy(dict.region, cJSON_GetStringValue(region));

		cJSON *territoryCode = cJSON_GetObjectItem(item, "territoryCode");
		dict.territoryCode = cJSON_GetNumberValue(territoryCode);

		cJSON *isFederalCity = cJSON_GetObjectItem(item, "isFederalCity");
		dict.isFederalCity = cJSON_GetNumberValue(isFederalCity);		
		array[i] = dict;
		
		item = item->next;
	}

	if (_array) {
		*_array = array;
	}
	else {
		free(array);
	}

	return count;
}

int egisz_rest_refs_part_all(struct egisz_rest_refs_part_struct **_array, int id){
	/*int part_count = egisz_rest_refs_parts(id); */
	/*if (part_count < 1) {*/
		/*fprintf(stderr, "ERROR in finction egisz_rest_refs_parts\n");	*/
		/*return -1;*/
	/*}*/
	/*struct egisz_rest_refs_part_struct *all_array = malloc(part_count * sizeof(struct egisz_rest_refs_part_struct));*/
	/*if (all_array == NULL) {*/
		/*fprintf(stderr, "Error allocate memory for egisz_rest_refs_part_struct\n");*/
		/*return -1;*/
	/*}	*/

	int i, all_count=0;
	/*for (i = 1; i < part_count + 1; ++i) {*/
		/*struct egisz_rest_refs_part_struct *array;*/
		/*int count = egisz_rest_refs_part(&array, id, i);*/
		/*if (count < 0) {*/
			/*fprintf(stderr, "Error allocate memory for egisz_rest_refs_part\n");*/
			/*return -1;			*/
		/*}*/
		/*int k;*/
		/*for (k=0; k < count; k++) {*/
			/*all_array[all_count] = array[k];*/
			/*all_count++;*/
		/*}*/
		/*free(array);*/
	/*}*/
	/*if (_array) {*/
		/**_array = all_array;*/
	/*}*/
	/*else {*/
		/*free(all_array);*/
	/*}	*/

	return all_count;
}

///////////////////
cJSON *egisz_rest_org_parts(int orgTypeId){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/org/parts?orgTypeId=%d", orgTypeId);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}

cJSON *egisz_rest_org_part(int orgTypeId, int part){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/org/part?orgTypeId=%d&part=%d", orgTypeId, part);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}

cJSON *egisz_rest_org_get(char *oid){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/org/get?oid=%s", oid);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}

///////////////////
cJSON *egisz_rest_depart_parts(int departTypeId){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/depart/parts?departTypeId=%d", departTypeId);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}

cJSON *egisz_rest_depart_part(int departTypeId, int part){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/depart/part?departTypeId=%d&part=%d", departTypeId, part);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}
cJSON *egisz_rest_depart_get(char *oid){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/depart/get?oid=%s", oid);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}





cJSON *egisz_rest_mo(char *oid){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/mo?oid=%s", oid);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}


cJSON *egisz_rest_lic_get(int service){
	URLRequest *request = url_request_prepare();
	char requestString[BUFSIZ];	
	sprintf(requestString, "rest/lic/get?service=%d", service);
	url_request_set_request_string(request, requestString);
	return json_from_url_connection_send_request(request);	
}


