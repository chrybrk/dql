#include <stdbool.h>
#include "include/global_pointers.h"
#include "include/hash.h"
#include "include/REPL.h"
#include "include/tokenizer.h"
#include "include/analyzer.h"

// TODO: move keywords to tokenizer

hash_T* keywords;

int main(void)
{
	keywords = init_hash(1024);
	hash_insert(keywords, ".exit", COMMAND_EXIT);
	hash_insert(keywords, "insert", INSERT);
	hash_insert(keywords, "create", CREATE);
	hash_insert(keywords, "select", SELECT);

	input_buffer_T* buffer = init_input_buffer();
	token_buffer_T* t_buffer = init_token_buffer();

	while (true)
	{
		printf("dql> ");

		input_buffer_read(buffer);
		token_buffer_create(t_buffer, buffer->buffer);

		analyzer_T* analyzer = init_analyzer(t_buffer->token_buffer);
		ast_T* root = analyzer_analyze(analyzer);
	}

	return 0;
}
