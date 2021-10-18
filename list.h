/**
 * File              : list.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

struct DICT_t {
   char key[128];
   char *value;
   struct DICT_t *next;
};
typedef struct DICT_t DICT;
