#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "analyzer.h"
#include "hash.h"

/*
 * Generator will convert AST-NODE to command.
 * A command can be executed by machine.
 *
 * Example:
 * 	create db test
 * 	it should command machine to create a folder named `test`.
 *
 *
 * The main idea of generator is to build the ast node into a simple command.
*/

/*
 * IDEAS?
 *
 * We may need to store which database, and its schemes.
 *
 * struct {
 *	database;
 *	table;
 *	scheme {
 *		**clm_name,
 *		**clm_data_type
 *	}
 * }
*/

/*
 * table structure
 *
 * it consist of variable name,
 * variable data type,
 * variable data size,
 * if it is primary key or not,
 * if the value must be null or not.
*/
typedef struct TABLE
{
	const char* var_name;
	enum {
		INT,
		FLOAT,
		CHAR
	} data_type;
	int data_type_size;
	int primary;
	int not_null;
} table_T;

/*
 * init table data
 * var_name, data_type, data_type_size, isPrimary, isNotNull
*/
table_T* init_table_structure(const char*, int, int, int, int);

/*
 * GENERATOR_STRUCT will hold the generator as well as db info.
 * db are just folder that have been created somewhere in filesystem.
 * each folder name becomes a key in hashmap.
 * And, the value for them would be another hashmap.
 *
 * Now, that will store table data with table structure.
 *
 * We need a way to store structure in memory but data on persistant storage.
 * And, retrive back the structure from persistant storage.
 *
 * storing metadata for databases using json might work.
 * storing actual data on persistant storage, with millons of data.
 * using BTree, Pages and some other stuff might work.
 *
 * NOTE: We cannot apply query on data without loading it in memory,
 * but loading millons of data in memory is impossible.
 *
 * TODO: Find a way to store data in filesystem, not in memory.
 * TODO: Custom BTree for storing table data.
 * TODO: Searching and Sorting neends to be performed within storage chunks, rather than memory.
*/
typedef struct GENERATOR_STRUCT
{
	char* path; // where the command will get executed. (TODO: temporary)
	const char* current_db; // which database we are in.
	hash_T* db; // hashmap for storing databse, table and table structure.
} generator_T;

/*
 * initiate generator with pathname
*/
generator_T* init_generator(char*);

/*
 * generate code
*/
void generator_generate(generator_T*, ast_T*);

/*
 * handle AST_STATEMENT
*/
void generator_handle_statement(generator_T*, ast_T*);

/*
 * handle AST_CREATE
*/
void generator_handle_create(generator_T*, ast_T*);

#endif
