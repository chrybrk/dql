#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "analyzer.h"

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

typedef struct GENERATOR_STRUCT
{
	char* path; // where the command will get executed. (TODO: temporary)
} generator_T;

/*
 * initiate generator with pathname
*/
generator_T* init_generator(char*);

/*
 * generate code
*/
void generator_generate(ast_T*);

/*
 * handle AST_STATEMENT
*/
void generator_handle_statement(ast_T*);

/*
 * handle AST_CREATE
*/
void generator_handle_create(ast_T*);

#endif
