/**list.h
 * File              : dictionary.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "dictionary.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <uuid/uuid.h>

void dictionary_print_data(Dictionary *dictionary) {
   struct dictionary_data_t *ptr = dictionary->data;
   printf("\n{ ");
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%s: %s)\n",ptr->key,ptr->value);
      ptr = ptr->next;
   }
	
   printf(" }");
}

Dictionary *dictionary_new(){
	Dictionary *dictionary = malloc(sizeof(Dictionary));
	if (dictionary == NULL) {
		fprintf(stderr, "Error to allocate memory\n");
		exit(ENOMEM);
	}

	dictionary->data = NULL;
	uuid_generate_random(dictionary->id);

	return dictionary;
}

void dictionary_new_value_for_key(Dictionary *dictionary, char *value, const char *key){
	struct dictionary_data_t *ptr = dictionary->data;	
	struct dictionary_data_t *new_data = malloc(sizeof(struct dictionary_data_t));
	if (new_data == NULL) {
		fprintf(stderr, "Error to allocate memory\n");
		exit(ENOMEM);
	}   
	new_data->next = ptr;
	dictionary->data = new_data;

	strncpy(new_data->key, key, 127);
	new_data->key[127]='\0';

	new_data->value = value;
}


