/**list.h
 * File              : dictionary.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 19.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "dictionary.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <uuid/uuid.h>

void dictionary_print(Dictionary *dictionary) {
   Dictionary *ptr = dictionary;
   printf("\n{ ");
	
   //start from the beginning
   while(ptr != NULL) {
	   switch (ptr->data_type) {
	   	case DICTIONARY_DATA_TYPE_STRING: printf("(%s: %s)\n",ptr->key,(char *)ptr->value); break;
	   	case DICTIONARY_DATA_TYPE_CHAR: printf("(%s: %c)\n",ptr->key, *ptr->value); break;
	   		
	   		
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

void set_value_for_key(Dictionary *dictionary, void *value, const char *key){
	strncpy(dictionary->key, key, 127);
	dictionary->key[127] = 0;
	dictionary->value = value;
}

Dictionary *search_dictionary_with_key(Dictionary *dictionary, const char *key){
	Dictionary *ptr = dictionary;
	if (ptr == NULL) {
		fprintf(stderr, "ERROR - dictionary is NULL\n");
		return NULL;
	}	

	while(ptr != NULL) { //find dictionary with key
		if (strncmp(ptr->key, key, 127) == 0) {
			return ptr;
		}
		ptr = ptr->next;
	}	

	printf("Dictionary has no key: %s\n", key);
	return NULL;
}

Dictionary *dictionary_new_with_value_for_key(void *value, DICTIONARY_DATA_TYPE data_type, const char *key){
	Dictionary *dictionary = dictionary_new();
	set_value_for_key(dictionary, value, key);
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
	set_value_for_key(new, value, key);
	ptr->next = new;
}

void *dictionary_get_value_for_key(Dictionary *dictionary, const char *key){
	Dictionary *ptr = search_dictionary_with_key(dictionary, key);
	if (ptr != NULL) {
		return ptr->value;
	}	
	return NULL;
}

void dictionary_set_value_for_key(Dictionary *dictionary, void *value, DICTIONARY_DATA_TYPE data_type, const char *key){
	Dictionary *ptr = search_dictionary_with_key(dictionary, key);
	if (ptr != NULL) {
		set_value_for_key(ptr, value, key);
	}	
}

