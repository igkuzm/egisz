/**list.h
 * File              : list.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "list.h"
#include <stdlib.h>
#include <errno.h>
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

void dictionary_new_value_for_key(Dictionary *dictionary, char *value, char *key){
	struct dictionary_data_t *ptr = dictionary->data;	
	struct dictionary_data_t *new_data = malloc(sizeof(struct dictionary_data_t));
	new_data->next = ptr;
	if (dictionary == NULL) {
		fprintf(stderr, "Error to allocate memory\n");
		exit(ENOMEM);
	}   




}


