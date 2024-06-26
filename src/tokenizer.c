#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/global_pointers.h"
#include "include/tokenizer.h"

token_T* init_token(TokenType type, char* value)
{
	token_T* new_token = calloc(1, sizeof(struct TOKEN_STRUCT));
	new_token->type = type;
	new_token->value = value;

	return new_token;
}

char* token_print(TokenType ttype)
{
	char* token_type_to_string;
	switch (ttype)
	{
		case COMMAND_EXIT: token_type_to_string = "COMMAND_EXIT"; break;
		case STRING: token_type_to_string = "STRING"; break;
		case DIGIT: token_type_to_string = "DIGIT"; break;
		case INSERT: token_type_to_string = "INSERT"; break;
		case CREATE: token_type_to_string = "CREATE"; break;
		case SELECT: token_type_to_string = "SELECT"; break;
	}

	/*
	if (token->value)
		printf("Token :: %s, %d - %s\n", token->value, token->type, token_type_to_string);
	else
		printf("Token :: %d, %d - %s\n", token->integer, token->type, token_type_to_string);
	*/
	
	return token_type_to_string;
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
	// allocate input buffer and token buffer
	token_buffer->input_buffer = buffer;
	token_buffer->token_buffer = init_array(sizeof(struct TOKEN_STRUCT));

	// starting point
	size_t index = 0;
	char c = token_buffer->input_buffer[index];

	while (c)
	{
		// buffer start with `.` is command
		if (c == '.')
		{
			HashPair* _hp = hash_find(keywords, token_buffer->input_buffer);

			if (_hp)
			{
				token_T* token = init_token(_hp->value, token_buffer->input_buffer);
				array_push(token_buffer->token_buffer, token);
			}
			else
				printf("keyword `%s` not implemented.\n", token_buffer->input_buffer);

			index += strlen(token_buffer->input_buffer);
			c = token_buffer->input_buffer[index];
		}
		else
		{
			if (isalpha(c))
			{
				char* word = calloc(1, sizeof(char));
				ssize_t idx = 0;

				while (c && isalpha(c))
				{
					word[idx] = c;
					word[idx + 1] = '\0';
					idx++;

					word = realloc(word, strlen(word) * idx);

					index++;
					c = token_buffer->input_buffer[index];
				}

				HashPair* _hp = hash_find(keywords, word);

				if (_hp)
				{
					token_T* token = init_token(_hp->value, word);
					array_push(token_buffer->token_buffer, token);
				}
				else
				{
					token_T* token = init_token(STRING, word);
					array_push(token_buffer->token_buffer, token);
				}
			}

			if (isdigit(c))
			{
				int digit = 0;

				while (c && isdigit(c))
				{
					char s[2] = { c, '\0' };
					digit = digit * 10 + atoi(s);

					index++;
					c = token_buffer->input_buffer[index];
				}

				token_T* token = init_token(DIGIT, NULL);
				token->integer = digit;
				array_push(token_buffer->token_buffer, token);
			}

			index++;
			c = token_buffer->input_buffer[index];
		}
	}
}
