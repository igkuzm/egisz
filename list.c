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
	Dictionary *new = malloc(sizeof(Dictionary));
	if (new == NULL) {
		fprintf(stderr, "Error to allocate memory\n");
		exit(ENOMEM);
	}

	new->next = NULL;
	return new;
}


