#ifndef __DEFS_H__
#define __DEFS_H__

#include "hash.h"

/*
 * it may be a waste of heap resource, but i had to do it god.
 * we need to pass pointer to the hashmap,
 * in order hashmap to store it as value.
*/
struct store_keyword {
	ssize_t value;
};

/*
 * it is a global definition of keywords associated to tokenizer.
 *
 * FIXME: Can we store it within tokenizer file?
*/
extern hash_T* tokenizer_keywords;

/*
 * table structure to define table metadata.
 * table metadata contains few things,
 *
 * name of the variable;
 * type of the variable;
 * size of the variable;
 * is it primary;
 * is it not null;
*/

typedef enum {
	INT = 4,
	FLOAT = 8,
	CHAR = 1
} DataType;

typedef enum {
	PRIMARY,
	NOT_NULL,
	NIL
} SecondaryType;

typedef struct TABLE_STRUCT {
	const char* variable_name;
	DataType data_type;
	char data_type_size;
	SecondaryType secondary_type;
} table_T;

/*
 * It will be easier to map enums to hashmap.
 * And, retrive back without using strcmp.
 * WOULD IT BE A WASTE OF RESOURCE? Maybe?
*/

extern hash_T* table_metadata_keywords;

/*
 * init table(*name, data type, size, secondary type);
 * it will return table with meta data,
 * that will be used to store in table hashmap.
*/
extern table_T* init_table(const char*, DataType, char, SecondaryType);

#endif
