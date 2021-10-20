/**list.h
 * File              : dictionary.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 20.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "dictionary.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void dictionary_print(Dictionary *dictionary) {
   Dictionary *ptr = dictionary;
   printf("\n{ ");
	
   //start from the beginning
   while(ptr != NULL) {
	   switch (ptr->data_type) {
	   	case DICTIONARY_DATA_TYPE_STRING: printf("\"%s\": \"%s\"\n",ptr->key,(char *)ptr->value); break;
	   	case DICTIONARY_DATA_TYPE_DICTIONARY: dictionary_print(ptr); break;
	   	case DICTIONARY_DATA_TYPE_CHAR:{
			                             char *value = ptr->value; 
										 printf("\"%s\": %c\n",ptr->key,value[0]); break;
									   } 
	   	case DICTIONARY_DATA_TYPE_NUMBER:{
			                             int *value = ptr->value; 
										 printf("\"%s\": %d\n",ptr->key,value[0]); break;
									   } 
	   	case DICTIONARY_DATA_TYPE_DATA: printf("\"%s\": (data)\n",ptr->key); break;
									   									 
											  
		default: break;
	   }
      ptr = ptr->next;
   }

   printf(" }");
}

Dictionary *dictionary_new(){
	Dictionary *dictionary = malloc(sizeof(Dictionary));
	if (dictionary == NULL) { //check memory allocation
		fprintf(stderr, "ERROR to allocate memory for dictionary\n");
		exit(ENOMEM);
	}
	dictionary->next = NULL;
	return dictionary;
}

void set_value_for_key(Dictionary *dictionary, void *value, DICTIONARY_DATA_TYPE data_type,  const char *key){
	strncpy(dictionary->key, key, 127);
	dictionary->key[127] = 0;
	dictionary->value = value;
	dictionary->data_type = data_type;
}

Dictionary *search_dictionary_with_key(Dictionary *dictionary, const char *key, Dictionary *prev){
	Dictionary *ptr = dictionary;
	if (ptr == NULL) {
		fprintf(stderr, "ERROR - dictionary is NULL\n");
		return NULL;
	}	

	while(ptr != NULL) { //find dictionary with key
		if (strncmp(ptr->key, key, 127) == 0) {
			return ptr;
		}
		prev = ptr;
		ptr = ptr->next;
	}	

	printf("Dictionary has no key: %s\n", key);
	return NULL;
}

Dictionary *dictionary_new_with_value_for_key(void *value, DICTIONARY_DATA_TYPE data_type, const char *key){
	Dictionary *dictionary = dictionary_new();
	set_value_for_key(dictionary, value, data_type, key);
	return dictionary;
}

void dictionary_add_value_for_key(Dictionary *dictionary, void *value, DICTIONARY_DATA_TYPE data_type, const char *key){
	Dictionary *ptr = dictionary;
	if (ptr == NULL) {
		fprintf(stderr, "ERROR - dictionary is NULL\n");
		return;
	}

	while(ptr->next != NULL) { //find last dict
		ptr = ptr->next;
	}

	Dictionary *new = dictionary_new();
	set_value_for_key(new, value, data_type, key);
	ptr->next = new;
}

void *dictionary_get_value_for_key(Dictionary *dictionary, const char *key){
	Dictionary *ptr = search_dictionary_with_key(dictionary, key, NULL);
	if (ptr != NULL) {
		return ptr->value;
	}	
	return NULL;
}

void dictionary_set_value_for_key(Dictionary *dictionary, void *value, DICTIONARY_DATA_TYPE data_type, const char *key){
	Dictionary *ptr = search_dictionary_with_key(dictionary, key, NULL);
	if (ptr != NULL) {
		set_value_for_key(ptr, value, data_type, key);
	}	
}

void dictionary_remove_value_for_key(Dictionary *dictionary, const char *key){
	Dictionary *prev;
	Dictionary *ptr = search_dictionary_with_key(dictionary, key, prev);
	if (ptr != NULL) {
		Dictionary *next = ptr->next;
		if (prev != NULL) {
			prev->next = next;
		}
		else {
			dictionary = next;
		}
		free(ptr);
	}
}

Array *array_new(){
	Array *array = malloc(sizeof(Array));
	array->data = malloc(8); //size of poin is 8
	array->count = 0;
	return array;
}
void array_free(Array *array){
	int i;
	for (i = 0; i < array->count; ++i) {
		void *item = array->data[i];
		free(item);	
	}
	free(array);
}
void array_add_item(Array *array, void *item){
	array->data = realloc(array->data, (array->count + 1) * 8);
	array->data[array->count] = item;
	array->count++;
}
void array_insert_item_at_index(Array *array, void *new_item, int index){
	array->data = realloc(array->data, (array->count + 1) * 8);
	int	i, k=0;
	for (i = 0; i < array->count; ++i) {
		void *item = array->data[i];
		if (index == i) {
			array->data[k] = new_item;
			k++;
		}	
		array->data[k] = item;
		k++;
	}
	array->count++;
}
void *array_item_at_index(Array *array, int index){
	int	i;
	for (i = 0; i < array->count; ++i) {
		void *item = array->data[i];
		if (index == i) {
			return item;
		}	
	}
	return NULL;
}
void array_remove_item_at_index(Array *array, int index){
	Array *new_array = array_new();
	int	i;
	for (i = 0; i < array->count; ++i) {
		void *item = array->data[i];
		if (index != i) {
			array_add_item(new_array, item);
		}
	}
	array_free(array);
	array = new_array;
}

