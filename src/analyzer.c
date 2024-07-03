#include <stdio.h>
#include <string.h>
#include "include/defs.h"
#include "include/analyzer.h"

ast_T* init_ast(AST_TYPE type)
{
	ast_T* ast = calloc(1, sizeof(struct AST_STRUCT));
	ast->type = type;
	return ast;
}

analyzer_T* init_analyzer(array_T* t_buffer)
{
	analyzer_T* new_analyzer = calloc(1, sizeof(struct ANALYZER_STRUCT));
	new_analyzer->t_buffer = t_buffer;
	new_analyzer->index = 0;
	new_analyzer->current_token = new_analyzer->t_buffer->buffer[new_analyzer->index];

	return new_analyzer;
}

token_T* analyzer_consume(analyzer_T* analyzer, TokenType ttype)
{
	/*
	 * if ttype is matched with the current token type
	 * 	advance token.
	 * else
	 * 	return null.
	*/
	if (analyzer->current_token->type == ttype)
	{
		token_T* token = analyzer->current_token;
		analyzer->index++;
		analyzer->current_token = analyzer->t_buffer->buffer[analyzer->index];
		return token;
	}

	printf("Expected `%s` got `%s`.\n", token_print(ttype), token_print(analyzer->current_token->type));

	return NULL;
}

ast_T* analyzer_analyze_expr(analyzer_T* analyzer)
{
	ast_T* expr;

	switch (analyzer->current_token->type)
	{
		case STRING:
		{
			token_T* new_string_token = analyzer_consume(analyzer, STRING);

			expr = init_ast(AST_EXPR);
			expr->token = new_string_token;
			break;
		}
	}

	return expr;
}

ast_T* analyzer_analyze(analyzer_T* analyzer)
{
	ast_T* statement = init_ast(AST_STATEMENT);
	statement->statement = init_array(sizeof(struct AST_STRUCT));

	ast_T* returned_ast = NULL;

	switch (analyzer->current_token->type)
	{
		case CREATE: returned_ast = analyzer_create(analyzer); break;
		case COMMAND_EXIT: exit(EXIT_SUCCESS); break;
		default: printf("invalid syntax.\n"), analyzer_consume(analyzer, analyzer->current_token->type);
	}
	
	if (returned_ast)
		array_push(statement->statement, returned_ast);

	return statement;
}

ast_T* analyzer_analyze_db_creation(analyzer_T* analyzer)
{
	/*
	 * db creation only requires name of the db.
	 * and just expr can handle that.
	*/

	return analyzer_analyze_expr(analyzer);
}

void
error_handle_collect_status(
	const char* error_message,
	void* on_what,
	int* status)
{
	if (on_what == NULL)
		puts(error_message), *status = 1;
}

table_T* collect_parameter(analyzer_T* analyzer)
{
	token_T* variable_name = analyzer_consume(analyzer, STRING);
	int flag = 0;

	error_handle_collect_status("collect_parameter: Expected variable name.", variable_name, &flag);

	token_T* data_type_name = analyzer_consume(analyzer, STRING);
	error_handle_collect_status(
			"collect_parameter: Expected data type. (INT, FLOAT, VARCHAR | VARCHAR(0-255))",
			data_type_name,
			&flag
	);

	int data_type_size = 0;
	if (analyzer->current_token->type == LPARAN)
	{
		analyzer_consume(analyzer, LPARAN);
		token_T* size = analyzer_consume(analyzer, DIGIT);
		error_handle_collect_status("collect_parameter: Expected `digit` after `(`.", size, &flag);
		
		data_type_size = size->integer;
		error_handle_collect_status("collect_parameter: Expected `)` after `expr`.", analyzer_consume(analyzer, RPARAN), &flag);
	}

	if (flag == 1)
	{
		puts("Something went wrong, cannot create table.");
		return NULL;
	}

	struct store_keyword* _dt_hp = hash_get(table_metadata_keywords, data_type_name->value);
	error_handle_collect_status("Cannot find data type", _dt_hp, &flag);

	if (analyzer->current_token->type != STRING)
	{
		table_T* table = init_table(
				variable_name->value,
				_dt_hp->value, data_type_size == 0 ? _dt_hp->value : data_type_size,
				NIL
				);
		return table;
	}

	token_T* secondary_type_name = analyzer_consume(analyzer, STRING);

	struct store_keyword* _st_hp = hash_get(table_metadata_keywords, secondary_type_name->value);
	error_handle_collect_status("Cannot find secondary data type", _st_hp, &flag);

	if (flag == 1)
	{
		puts("Something went wrong, cannot create table.");
		return NULL;
	}

	table_T* table = init_table(
			variable_name->value,
			_dt_hp->value, data_type_size == 0 ? _dt_hp->value : data_type_size,
			_st_hp->value
			);
	return table;
}

/*
 * FIXME: Not so efficient way of writing code.
 *
 * This function needs little too much work and,
 *
 * TODO: Better way of handling errors.
*/
ast_T* analyzer_analyze_table_creation(analyzer_T* analyzer)
{
	/*
	 * table creation requires name and more expr which includes data type and variable name.
	 * we have to first create ast node which will have token which is name of the table.
	 *
	 * note:
	 * (variable 	int | float | varchar(size)	 primary | not null | none, ...)
	 *
	 * we can create structure to store that information.
	 *
	 * struct variable_info_T {
	 *	const char* name;
	 *	DataType dt;
	 *	int dts;
	 *	SecondaryType st;
	 * }
	 *
	 * enum DataType {
	 * 	INT,
	 * 	FLOAT,
	 * 	CHAR
	 * }
	 * 
	 * enum SecondaryType {
	 * 	PRIMARY,
	 * 	NOT_NULL,
	 * 	NIL
	 * }
	*/
	
	/*
	 * create_table will hold
	 * 	-> name of the table
	 * 	-> parameters.
	 *
	 * name of the table can be stored in token;
	 * parameters can be stored in array;
	 * 	-> array will hold pointer to table_T
	*/
	ast_T* create_table = init_ast(AST_TABLE);

	// name_of_the_table cannot be nulled but it can be of different type.
	token_T* name_of_the_table = analyzer_consume(analyzer, STRING);
	if (name_of_the_table == NULL)
	{
		printf("analyzer_analyze_table_creation: failed to find name of the table.");
		return NULL;
	}
	
	create_table->token = name_of_the_table;

	/*
	 * Now we have to parse the bigger thing.
	 * Grammar of how it should look like.
	 *
	 * expr:
	 * 	string
	 *
	 * create <db | table> expr (expr expr <expr | null>, ...)
	*/

	// now we have to look for LPARAN;
	if (analyzer->current_token->type != LPARAN)
	{
		printf("Expected lparan and followd by expr...\n");
		return NULL;
	}

	analyzer_consume(analyzer, LPARAN);

	// this will hold all of the table metadata
	create_table->statement = init_array(sizeof(struct TABLE_STRUCT));

	const char** keys = hash_bucket(table_metadata_keywords);
	printf("p - %p\n", keys);

	table_T* param = collect_parameter(analyzer);
	if (param)
		array_push(create_table->statement, param);

	while (analyzer->current_token->type == COMMA)
	{
		analyzer_consume(analyzer, COMMA);

		table_T* param = collect_parameter(analyzer);
		if (param)
			array_push(create_table->statement, param);
	}

	token_T* rparan = analyzer_consume(analyzer, RPARAN);
	if (rparan == NULL)
	{
		puts("Expected `)` after `expr`.");
		return NULL;
	}

	return create_table;
}

ast_T* analyzer_create(analyzer_T* analyzer)
{
	/*
	 * init ast with `AST_CREATE`
	 *
	 * check if it is table creation,
	 * or database creation.
	 * 
	 * Both, have different way of parsing, creating database parser is simple,
	 * because we need create db and name.
	 *
	 * But, creating table will require more than that.
	 * create table `name` (variable with data type, ...)
	*/
	ast_T* create_ast = init_ast(AST_CREATE);
	ast_T* left,* right;

	analyzer_consume(analyzer, CREATE);

	/*
	 * Check for create type.
	 *
	 * if `db` then we only need `name`.
	 * else if `table` then we need `name`, `(expr)`.
	*/
	token_T* create_type = analyzer_consume(analyzer, STRING);

	if (create_type == NULL)
		return  NULL;

	left = init_ast(strcmp(create_type->value, "db") == 0 ? AST_DATABASE : AST_TABLE);
	right = left->type == AST_DATABASE ? analyzer_analyze_db_creation(analyzer) : analyzer_analyze_table_creation(analyzer);

	printf("left_node - AST_TYPE - %p\n", left);
	printf("right_node - AST_EXPR - %p\n", right);

	if (left == NULL || right == NULL)
	{
		printf("Something went wrong.\n");
		return NULL;
	}

	create_ast->left = left;
	create_ast->right = right;

	return create_ast;
}

ast_T* analyzer_insert(analyzer_T* analyzer)
{
	return NULL;
}

ast_T* analyzer_select(analyzer_T* analyzer)
{
	return NULL;
}

/*
 * extern function *init_table(...)
 * it is a function that is required to create table metadata.
*/
table_T*
init_table(
	const char* variable_name,
	DataType data_type,
	char data_type_size,
	SecondaryType secondary_type)
{
	table_T* table = calloc(1, sizeof(struct TABLE_STRUCT));
	if (table == NULL)
	{
		printf("table_init: could not create table metadata.");
		return NULL;
	}

	table->variable_name = variable_name;
	table->data_type = data_type;
	table->data_type_size = data_type_size;
	table->secondary_type = secondary_type;

	return table;
}
