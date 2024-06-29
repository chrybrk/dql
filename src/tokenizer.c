#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/defs.h"
#include "include/tokenizer.h"

struct buffer_data {
	size_t index;
	size_t size;
	char c;
	char* buffer;
};

int alpha_dot(int c)
{
	return isalpha(c) || c == '.';
}

token_T* init_token(TokenType type, char* value)
{
	token_T* new_token = calloc(1, sizeof(struct TOKEN_STRUCT));
	new_token->type = type;
	new_token->value = value;

	return new_token;
}

char* token_print(TokenType ttype)
{
	switch (ttype)
	{
		case COMMA: return "COMMA";
		case LPARAN: return "LPARAN";
		case RPARAN: return "RPARAN";
		case COMMAND_EXIT: return "COMMAND_EXIT";
		case STRING: return "STRING";
		case DIGIT: return "DIGIT";
		case INSERT: return "INSERT";
		case CREATE: return "CREATE";
		case SELECT: return "SELECT";
		case EOS: return "EOS(end of statement)";
	}
}

token_buffer_T* init_token_buffer()
{
	token_buffer_T* new_t_buffer = calloc(1, sizeof(struct TOKEN_BUFFER_STRUCT));
	new_t_buffer->token_buffer = NULL;
	new_t_buffer->input_buffer = NULL;

	return new_t_buffer;
}

void buffer_advance(struct buffer_data* b_data)
{
	if (b_data->index < b_data->size)
		b_data->index++, b_data->c = b_data->buffer[b_data->index];

	else
		b_data->c = '\0';
}

char* collect_string_from_buffer(struct buffer_data* b_data, int (*f)(int))
{
	char* word = calloc(1, sizeof(char));
	ssize_t index = 0;

	while (b_data->c && (*f)(b_data->c))
	{
		word[index] = b_data->c;
		index++;

		word = realloc(word, index * sizeof(char));
		buffer_advance(b_data);
	}

	word[index] = '\0';

	return word;
}

token_T* buffer_digit(struct buffer_data* b_data)
{
	char* word = collect_string_from_buffer(b_data, isdigit);

	token_T* token = init_token(DIGIT, NULL);
	token->integer = atoi(word);

	return token;
}

token_T* buffer_string(struct buffer_data* b_data)
{
	char* word = collect_string_from_buffer(b_data, alpha_dot);

	void* _hp = hash_get(tokenizer_keywords, word);
	printf("%s - %p\n", word, _hp);
	token_T* token = init_token(_hp ? ((struct tokenizer_keyword_data*)_hp)->value : STRING, word);

	return token;
}

void token_buffer_create(token_buffer_T* token_buffer, char* buffer)
{
	// allocate input buffer and token buffer
	token_buffer->input_buffer = buffer;
	token_buffer->token_buffer = init_array(sizeof(struct TOKEN_STRUCT));

	struct buffer_data b_data = { .index = 0, .size = strlen(token_buffer->input_buffer), .c = token_buffer->input_buffer[0], .buffer = token_buffer->input_buffer };

	while (b_data.c)
	{
		// skip whitespace
		if (b_data.c == ' ') buffer_advance(&b_data);

		if (isdigit(b_data.c)) array_push(token_buffer->token_buffer, buffer_digit(&b_data));
		if (isalpha(b_data.c) || b_data.c == '.') array_push(token_buffer->token_buffer, buffer_string(&b_data));

		switch (b_data.c)
		{
			case ',':
			{
				token_T* token = init_token(COMMA, ",");
				buffer_advance(&b_data);

				array_push(token_buffer->token_buffer, token);
				break;
			}
			case '(':
			{
				token_T* token = init_token(LPARAN, "(");
				buffer_advance(&b_data);

				array_push(token_buffer->token_buffer, token);
				break;
			}
			case ')':
			{
				token_T* token = init_token(RPARAN, ")");
				buffer_advance(&b_data);

				array_push(token_buffer->token_buffer, token);
				break;
			}
		}
	}

	array_push(token_buffer->token_buffer, init_token(EOS, NULL));
}
