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

void dictionary_print_data(Dictionary *dictionary) {
   Dictionary *ptr = dictionary;
   printf("\n{ ");
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%s: %s)\n",ptr->key,ptr->value);
      ptr = ptr->next;
   }
	
   printf(" }");
}

Dictionary *dictionary_new(){


}

void dictionary_add_value_for_key(Dictionary *dictionary, void *value, const char *key){
	Dictionary *ptr = dictionary;
	if (ptr == NULL) {
		return;
	}
	else {
		while(ptr->next != NULL) { //find last dict
			ptr = ptr->next;
		}
	}
}
Dictionary *dictionary_new_with_value_for_key(void *value, const char *key){
   Dictionary *dictionary = malloc(sizeof(Dictionary));
   if (dictionary == NULL) { //check memory allocation
		fprintf(stderr, "ERROR to allocate memory for dictionary\n");
		exit(ENOMEM);
   }
   dictionary->next = NULL;
   strncpy(dictionary->key, key, 127);
   dictionary->key[127] = 0;
   dictionary->value = value;
   return dictionary;
}

