/**
 * File              : test.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 11.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "cJSON.h"
#include "ezxml.h"
#include "rest_service_NR.h"
#include "url_connector.h"
#include "nsi.h"
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

	//struct egisz_rest_refs_list_struct *array;
	//int c = egisz_rest_refs_list(&array);
	//int	i;
	//for (i = 0; i < c; ++i) {
		//printf("ID: %d, NAME: %s\n", array[i].id, array[i].refsName);
	//}

	//printf("PARTS: %d\n", egisz_rest_refs_parts(15));
	
	//struct egisz_rest_refs_part_struct *array;
	//int c = egisz_rest_refs_part(&array, 15, 1);
	//int	i;
	//for (i = 0; i < c; ++i) {
		//printf("REGION: %s, CODE: %d\n", array[i].region, array[i].territoryCode);
	//}
	
	struct egisz_rest_refs_part_struct *array;
	int c = egisz_rest_refs_part_all(&array, 15);
	printf("COUNT: %d\n", c);
	//int	i;
	//for (i = 0; i < c; ++i) {
		//printf("REGION: %s, CODE: %d\n", array[i].region, array[i].territoryCode);
	//}
	
	//cJSON *json = egisz_rest_depart_part(1, 10);
	//cJSON *json = egisz_rest_org_part(2, 1);
	//cJSON *json = egisz_rest_org_get("1.2.643.5.1.13.13.12.1.42.202");		
	//cJSON *json = egisz_rest_depart_get("1.2.643.5.1.13.13.12.2.51.4977.0.2787.2107");		
	//cJSON *json = egisz_mo("1.2.643.5.1.13.13.12.1.42.202");		
	//cJSON *json = egisz_rest_lic_get(1);		
	//print_json(json);

	

	//ezxml_t ezxml = egisz_nsi_get_server_time(); 
	//ezxml_t ezxml = egisz_nsi_get_refbook_list(); 
	//printf("EZXML NAME: %s\n", ezxml->name);

	printf("Press any key to close application\n");
	getchar();

	return 0;
}
