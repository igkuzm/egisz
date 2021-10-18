/**
 * File              : list.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

struct dictionary_t {
   char key[128];
   char *value;
   struct dictionary_t *next;
   struct dictionary_t *prev;
};
typedef struct dictionary_t dictionary;

dictionary *dictionary_new(dictionary *head);
