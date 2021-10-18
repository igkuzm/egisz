/**
 * File              : list.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>
#include <uuid/uuid.h>

struct dictionary_data_t {
	char key[128];
	char *value;
	struct dictionary_data_t *next;
};

struct Dictionary_t {
	uuid_t id;
	struct dictionary_data_t *data;
};
typedef struct Dictionary_t Dictionary;

Dictionary *dictionary_new();
dictionary_add_key()
