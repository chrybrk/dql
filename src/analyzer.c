#include <stdio.h>
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
	if (analyzer->current_token->type == ttype)
	{
		token_T* token = analyzer->current_token;
		analyzer->index++;
		analyzer->current_token = analyzer->t_buffer->buffer[analyzer->index];
		return token;
	}

	printf("Expected `%s` got `%s`.", token_print(ttype), token_print(analyzer->current_token->type));

	return NULL;
}

ast_T* analyzer_analyze(analyzer_T* analyzer)
{
	ast_T* statement = init_ast(AST_STATEMENT);
	statement->statement = init_array(sizeof(struct AST_STRUCT));

	switch (analyzer->current_token->type)
	{
		case CREATE: array_push(statement->statement, analyzer_create(analyzer)); break;
		case COMMAND_EXIT: exit(EXIT_SUCCESS);
		default: printf("invalid syntax.\n"), analyzer_consume(analyzer, analyzer->current_token->type);
	}

	return statement;
}

ast_T* analyzer_create(analyzer_T* analyzer)
{
	ast_T* create_ast = init_ast(AST_CREATE);

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
