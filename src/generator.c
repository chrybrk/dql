#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "include/generator.h"

generator_T* init_generator(char* path)
{
	generator_T* new_generator = calloc(1, sizeof(struct GENERATOR_STRUCT));
	new_generator->path = path;
	return new_generator;
}

void generator_generate(ast_T* root)
{
	switch (root->type)
	{
		case AST_STATEMENT: generator_handle_statement(root); break;
		case AST_CREATE: generator_handle_create(root); break;
		default: printf("not implemented.\n");
	}
}

void generator_handle_statement(ast_T* root)
{
	for (ssize_t i = 0; i < root->statement->index; i++)
		generator_generate((ast_T*)root->statement->buffer[i]);
}

void generator_handle_create(ast_T* root)
{
	if (root->left && root->right)
	{
		if (root->left->type == AST_DATABASE)
			mkdir(root->right->token->value, S_IRWXU | S_IRWXG | S_IRWXO);

		if (root->left->type == AST_TABLE)
			printf("craete file inside database.\n");
	}
	else
		printf("Expected create <(db, table)> <expr>");
}
