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


