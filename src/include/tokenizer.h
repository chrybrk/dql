#ifndef __TOKENIZER_H__

#include "array.h"
#include "hash.h"

/*
 * Tokenizer: It helps to keep things organized and fast.
 * We can't waste resources on comparing strings all the time.
*/

// TokenType
typedef enum {
	COMMAND_EXIT // it will exit the program
} TokenType;

// TOKEN_STRUCT holds the token information.
typedef struct TOKEN_STRUCT
{
	TokenType type;
	char* value;
} token_T;

/*
 * Each line might have more than one token,
 * So it will hold all the tokens in buffer.
 *
 * Everytime we need to analyze buffer, it will allocate input buffer to this input buffer.
 * And, It will hold all the tokens.
*/
typedef struct TOKEN_BUFFER_STRUCT
{
	array_T* token_buffer;
	char* input_buffer;
} token_buffer_T;

// init token with type and value. 
token_T* init_token(TokenType type, char* value);

// print token
void token_print(token_T* token);

// init token buffer
token_buffer_T* init_token_buffer();

// create buffer and write tokens
void token_buffer_create(token_buffer_T* token_buffer, char* buffer);

#endif
