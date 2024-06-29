#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "include/generator.h"

table_T*
init_table_structure(
		const char* var_name,
		int data_type,
		int data_type_size,
		int isPrimary,
		int isNotNull)
{
	table_T* table = calloc(1, sizeof(struct TABLE));
	table->var_name = var_name;
	table->data_type = data_type;
	table->data_type_size = data_type_size;
	table->primary = isPrimary;
	table->not_null = isNotNull;

	return table;
}

generator_T* init_generator(char* path)
{
	generator_T* new_generator = calloc(1, sizeof(struct GENERATOR_STRUCT));
	new_generator->path = path;
	new_generator->db = init_hash();

	if (new_generator->db == NULL)
	{
		free(new_generator->path);
		free(new_generator->db);
		free(new_generator);

		printf("Failed to create database storage. Exiting program.\n");
		exit(EXIT_FAILURE);
	}

	return new_generator;
}

void generator_generate(generator_T* generator, ast_T* root)
{
	switch (root->type)
	{
		case AST_STATEMENT: generator_handle_statement(generator, root); break;
		case AST_CREATE: generator_handle_create(generator, root); break;
		default: printf("not implemented.\n");
	}
}

void generator_handle_statement(generator_T* generator, ast_T* root)
{
	for (ssize_t i = 0; i < root->statement->index; i++)
		generator_generate(generator, (ast_T*)root->statement->buffer[i]);
}

void create_database(generator_T* generator, ast_T* root)
{
	int sucess = mkdir(root->right->token->value, S_IRWXU | S_IRWXG | S_IRWXO);

	if (!sucess) printf("Error creating database.\n");
	else
	{
		/*
		 * No exception be handled here.
		 * table can be null, but we are not going to check here that now.
		*/
		hash_T* table = init_hash();
		hash_set(generator->db, root->right->token->value, table);
	}
}

void create_table(generator_T* generator, ast_T* root)
{
	/*
	 * if current_db is NULL then we are not inside any database.
	 * else
	 * 	we need to create a file named after `create table <name>`,
	 * 	also, we need to parse the parameters.
	 *
	 * 	create table <name> (structure)
	 *
	 * 	but, we do not want to store the data on our memory.
	 * 	so, insert command will and should be performed on file itself.
	*/
	if (generator->current_db == NULL)
		printf("you're not inside database, use `use <db>` command to use a database.\n");
	else
	{
		/*
		 * Create array of `table_T` which will store the structure.
		 * array will be the value of database->table->value
		 *
		 * where database and table are hashmap.
		*/

		array_T* buffer_of_table_structure = init_array(sizeof(struct TABLE));
	}
}

void generator_handle_create(generator_T* generator, ast_T* root)
{
	if (root->left && root->right)
	{
		if (root->left->type == AST_DATABASE)
			create_database(generator, root);

		if (root->left->type == AST_TABLE)
			create_table(generator, root);
	}
	else
		printf("Expected create <(db, table)> <expr>");
}
