/**
 * File              : list.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

struct Dictionary_t {
   char key[128];
   char *value;
   struct Dictionary_t *next;
};
typedef struct Dictionary_t Dictionary;

Dictionary *dictionary_new();
