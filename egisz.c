/**
 * File              : egisz.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 11.10.2021
 * Last Modified Date: 15.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "egisz.h"
#include "cJSON.h"
//#include "rest_service_NR.h"
#include "http_connector.h"
#include <stdio.h>
#include <stdlib.h>
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
	//cJSON *json = egisz_refs_list();
	//cJSON *json = egisz_refs_parts(15);
	//cJSON *json = egisz_depart_part(1, 10);
	//cJSON *json = egisz_org_part(2, 1);
	//cJSON *json = egisz_rest_org_get("1.2.643.5.1.13.13.12.1.42.202");		
	//cJSON *json = egisz_depart_get("1.2.643.5.1.13.13.12.2.51.4977.0.2787.2107");		
	//cJSON *json = egisz_mo("1.2.643.5.1.13.13.12.1.42.202");		
	//cJSON *json = egisz_rest_lic_get(1);		
	//print_json(json);
	//printf("ITEM: %s\n", cJSON_Print(json));	
	
	//soap_test();
	
	printf("Write HTTP request and press enter\n");

	parse_http_string(readString(), NULL);


	printf("Press any key to close application\n");
	getchar();

	return 0;
}
