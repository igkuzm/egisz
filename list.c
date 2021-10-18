/**list.h
 * File              : list.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "list.h"
#include <stdlib.h>

void DICT_print(dictionary *head) {
   dictionary *ptr = head;
   printf("\n{ ");
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%s: %s)\n",ptr->key,ptr->value);
      ptr = ptr->next;
   }
	
   printf(" }");
}

dictionary *dictionary_new(dictionary *head){
	dictionary *new = malloc(sizeof(dictionary));
	

}


