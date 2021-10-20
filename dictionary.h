/**
 * File              : dictionary.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 18.10.2021
 * Last Modified Date: 20.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>

/*! \enum DICTIONARY_DATA_TYPE
 *
 *  type of date, used in dictionary value
 */
typedef enum DICTIONARY_DATA_TYPE {	
	DICTIONARY_DATA_TYPE_STRING,
	DICTIONARY_DATA_TYPE_CHAR,
	DICTIONARY_DATA_TYPE_NUMBER,
	DICTIONARY_DATA_TYPE_DICTIONARY,
	DICTIONARY_DATA_TYPE_ARRAY,
	DICTIONARY_DATA_TYPE_DATA,
} DICTIONARY_DATA_TYPE;

struct Dictionary_t {
	char key[128];
	void *value;
	DICTIONARY_DATA_TYPE data_type;
	struct Dictionary_t *next;
};
typedef struct Dictionary_t Dictionary;

Dictionary *dictionary_new_with_value_for_key(void *value, DICTIONARY_DATA_TYPE data_type, const char *key);
void dictionary_free();
void dictionary_print(Dictionary *dictionary);
void dictionary_add_value_for_key(Dictionary *dictionary, void *value, DICTIONARY_DATA_TYPE data_type, const char *key);
void dictionary_set_value_for_key(Dictionary *dictionary, void *value, DICTIONARY_DATA_TYPE data_type, const char *key);
void *dictionary_get_value_for_key(Dictionary *dictionary, const char *key);
void dictionary_remove_value_for_key(Dictionary *dictionary, const char *key);

struct Array_t {
	void *data;
	int count;
};
typedef struct Array_t Array;

Array *array_new();
void array_free();
void array_add_item(Array *array, void *item);
void array_insert_item(Array *array, int index, void *item);
void *array_item_at_index(Array *array, int index);
