#ifndef __ANALYZER_H__
#define __ANALYZER_H__

/*
 * Analyzer will go through each tokens,
 * Try to convert each token into a meaningful NODE (AST-NODE).
 *
 * Example:
 * create db test;
 * 	{
 * 		type 	<- AST_CREATE
 * 		left 	<- DATABASE
 * 		right <- EXPR;
 * 	}
 *
 * 	each ast type can have more or different fields.
*/

#include "array.h"
#include "hash.h"
#include "tokenizer.h"

typedef enum
{
	AST_STATEMENT, 	// this will contain all the elements of the buffer.
	AST_EXPR,				// this will contain the expr in the statement.
	AST_CREATE,			// this will be the create node.
	AST_TABLE, AST_DATABASE // create can have two things (table, database)
} AST_TYPE;

/*
 * AST-NODE
 *
 * Holds the information of AST
*/
typedef struct AST_STRUCT
{
	AST_TYPE type;
	struct AST_STRUCT* left;
	struct AST_STRUCT* right;

	char* string_value;
	int digit_value;

	token_T* token;

	array_T* statement;
} ast_T;

/*
 * ast initializer
*/
ast_T* init_ast(AST_TYPE);

// It will hold all the tokens.
typedef struct ANALYZER_STRUCT
{
	ssize_t index;			// index of which token to use
	array_T* t_buffer;	// buffer of tokens
	token_T* current_token;		// current_token
} analyzer_T;


/*
 * init analyzer with buffer of tokens.
*/
analyzer_T* init_analyzer(array_T*);

/*
 * advance when you're done working with that token
*/
token_T* analyzer_consume(analyzer_T*, TokenType);

/*
 * analyzer analyze and return statement
*/
ast_T* analyzer_analyze(analyzer_T*);

/*
 * EACH MAJOR AST NODE
 * will have its own funcion
*/

// create node <- ast_T*
ast_T* analyzer_create(analyzer_T*);

// insert node <- ast_T*
ast_T* analyzer_insert(analyzer_T*);

// select node <- ast_T*
ast_T* analyzer_select(analyzer_T*);

#endif
