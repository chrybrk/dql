#include <stdbool.h>
#include "include/defs.h"
#include "include/hash.h"
#include "include/REPL.h"
#include "include/tokenizer.h"
#include "include/analyzer.h"
#include "include/generator.h"

hash_T* tokenizer_keywords;

int main(void)
{
	tokenizer_keywords = init_hash();
	hash_set(tokenizer_keywords, ".exit", (void*)&(struct tokenizer_keyword_data){ .value = COMMAND_EXIT });
	hash_set(tokenizer_keywords, "insert", (void*)&(struct tokenizer_keyword_data){ .value = INSERT });
	hash_set(tokenizer_keywords, "create", (void*)&(struct tokenizer_keyword_data){ .value = CREATE });
	hash_set(tokenizer_keywords, "select", (void*)&(struct tokenizer_keyword_data){ .value = SELECT });

	input_buffer_T* buffer = init_input_buffer();
	token_buffer_T* t_buffer = init_token_buffer();

	while (true)
	{
		printf("dql> ");

		input_buffer_read(buffer);
		token_buffer_create(t_buffer, buffer->buffer);

		for (ssize_t i = 0; i < t_buffer->token_buffer->index; i++)
			printf("%s\n", token_print(((token_T*)t_buffer->token_buffer->buffer[i])->type));

		// analyzer_T* analyzer = init_analyzer(t_buffer->token_buffer);
		// ast_T* root = analyzer_analyze(analyzer);
		// generator_T* generator = init_generator(".");
		// generator_generate(root);
	}

	return 0;
}
