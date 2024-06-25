#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/global_pointers.h"
#include "include/tokenizer.h"

token_T* init_token(TokenType type, char* value)
{
	token_T* new_token = calloc(1, sizeof(struct TOKEN_STRUCT));
	new_token->type = type;
	new_token->value = value;

	return new_token;
}

void token_print(token_T* token)
{
	char* token_type_to_string;
	switch (token->type)
	{
		case COMMAND_EXIT: token_type_to_string = "COMMAND_EXIT"; break;
	}

	printf("Token :: %s, %d\n", token->value, token->type);
}

token_buffer_T* init_token_buffer()
{
	token_buffer_T* new_t_buffer = calloc(1, sizeof(struct TOKEN_BUFFER_STRUCT));
	new_t_buffer->token_buffer = NULL;
	new_t_buffer->input_buffer = NULL;
	return new_t_buffer;
}

void token_buffer_create(token_buffer_T* token_buffer, char* buffer)
{
	// free if allocated
	if (token_buffer->input_buffer)
		free(token_buffer->input_buffer);

	if (token_buffer->token_buffer)
		free(token_buffer->token_buffer);

	// allocate input buffer and token buffer
	token_buffer->input_buffer = buffer;
	token_buffer->token_buffer = init_array(sizeof(struct TOKEN_STRUCT));

	// starting point
	size_t index = 0;
	char c = token_buffer->input_buffer[index];

	while (c)
	{
		// if it starts with `.` then it is a command.
		if (c == '.')
		{
			// get HashPair and check if it is a valid command keyword.
			HashPair* hp = hash_find(keywords, token_buffer->input_buffer);
			if (hp == NULL) printf("[ERROR]: tokenizer: `%s` is not defined.\n", token_buffer->input_buffer), exit(EXIT_FAILURE);

			// create new token, push it to array.
			token_T* new_token = init_token(hp->value, token_buffer->input_buffer);
			array_push(token_buffer->token_buffer, new_token);

			// advance
			index += strlen(token_buffer->input_buffer);
			c = token_buffer->input_buffer[index];
		}
	}
}
