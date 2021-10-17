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
#include "egisz_ssl_connector.h"
#include "url_connector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HOST "nr.egisz.rosminzdrav.ru"
#define PORT 443
#define APIKEY ""

cJSON *json_from_egisz_ssl_connector_answer(char *method, int argc, char *argv[]){
	EgiszSSLConnectorAnswer *answer = egisz_ssl_connector_answer_new();
	if (egisz_ssl_connector_socket(HOST, PORT, APIKEY, method, argc, argv, answer, egisz_ssl_connector_answer_callback)){
		fprintf(stderr, "Error in function egisz_ssl_connector\n");
		return NULL;
	}

	//printf("HEADER: %s\n", answer->header);
	//printf("BODY: %s\n", answer->body);
	
	cJSON *json = cJSON_Parse(answer->body);
	//if (json == NULL) { //get answer string
		//if (answer->len_body > 0) {
			//char answer_str[answer->len_body + 2];
			//sprintf(answer_str, "\"%s\"", answer->body);	
			//json = cJSON_Parse(answer_str);
		//}
	//}
	egisz_ssl_connector_answer_free(answer);
	return json;
}

///////////////////
cJSON *egisz_rest_refs_list(){
	char *method = "GET /rest/refs/list";
	return json_from_egisz_ssl_connector_answer(method, 0, NULL);
}

cJSON *egisz_rest_refs_parts(int id){
	char *method = "GET /rest/refs/parts";
	char _id[128];
	sprintf(_id, "id=%d", id);
	char *argv[] = {_id};	
	return json_from_egisz_ssl_connector_answer(method, 1, argv);
}

cJSON *egisz_rest_refs_part(int id, int part){
	char *method = "GET /rest/refs/part";
	char _id[128];
	sprintf(_id, "id=%d", id);
	char _part[128];
	sprintf(_part, "part=%d", part);
	char *argv[] = {_id, _part};	
	return json_from_egisz_ssl_connector_answer(method, 2, argv);
}

///////////////////
cJSON *egisz_rest_org_parts(int orgTypeId){
	char *method = "GET /rest/org/parts";
	char _orgTypeId[128];
	sprintf(_orgTypeId, "orgTypeId=%d", orgTypeId);
	char *argv[] = {_orgTypeId};	
	return json_from_egisz_ssl_connector_answer(method, 1, argv);
}

cJSON *egisz_rest_org_part(int orgTypeId, int part){
	char *method = "GET /rest/org/part";
	char _orgTypeId[128];
	sprintf(_orgTypeId, "orgTypeId=%d", orgTypeId);
	char _part[128];
	sprintf(_part, "part=%d", part);	
	char *argv[] = {_orgTypeId, _part};	
	return json_from_egisz_ssl_connector_answer(method, 2, argv);
}

cJSON *egisz_rest_org_get(char *oid){
	char *method = "GET /rest/org/get";
	char _oid[256];
	sprintf(_oid, "oid=%s", oid);
	char *argv[] = {_oid};	
	return json_from_egisz_ssl_connector_answer(method, 1, argv);
}

///////////////////
cJSON *egisz_rest_depart_parts(int departTypeId){
	char *method = "GET /rest/depart/parts";
	char _departTypeId[128];
	sprintf(_departTypeId, "departTypeId=%d", departTypeId);
	char *argv[] = {_departTypeId};	
	return json_from_egisz_ssl_connector_answer(method, 1, argv);
}

cJSON *egisz_rest_depart_part(int departTypeId, int part){
	char *method = "GET /rest/depart/part";
	char _departTypeId[128];
	sprintf(_departTypeId, "departTypeId=%d", departTypeId);
	char _part[128];
	sprintf(_part, "part=%d", part);	
	char *argv[] = {_departTypeId, _part};	
	return json_from_egisz_ssl_connector_answer(method, 2, argv);
}
cJSON *egisz_rest_depart_get(char *oid){
	char *method = "GET /rest/depart/get";
	char _oid[256];
	sprintf(_oid, "oid=%s", oid);
	char *argv[] = {_oid};	
	return json_from_egisz_ssl_connector_answer(method, 1, argv);
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


