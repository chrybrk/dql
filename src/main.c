#include <stdbool.h>
#include "include/global_pointers.h"
#include "include/hash.h"
#include "include/REPL.h"
#include "include/tokenizer.h"

hash_T* keywords;

int main(void)
{
	keywords = init_hash(1024);
	hash_insert(keywords, ".exit", COMMAND_EXIT);

	input_buffer_T* buffer = init_input_buffer();
	token_buffer_T* t_buffer = init_token_buffer();

	while (true)
	{
		printf("dql> ");
		input_buffer_read(buffer);
		token_buffer_create(t_buffer, buffer->buffer);

		for (ssize_t i = 0; i < t_buffer->token_buffer->index; i++)
		{
			token_T* token = (token_T*)t_buffer->token_buffer->buffer[i];
			token_print(token);
		}
	}

	return 0;
}
