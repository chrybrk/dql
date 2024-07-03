#include <stdbool.h>
#include "include/defs.h"
#include "include/hash.h"
#include "include/REPL.h"
#include "include/tokenizer.h"
#include "include/analyzer.h"
#include "include/generator.h"

hash_T* tokenizer_keywords;
hash_T* table_metadata_keywords;

int main(void)
{
	tokenizer_keywords = init_hash();
	hash_set(tokenizer_keywords, ".exit", (void*)&(struct store_keyword){ .value = COMMAND_EXIT });
	hash_set(tokenizer_keywords, "insert", (void*)&(struct store_keyword){ .value = INSERT });
	hash_set(tokenizer_keywords, "create", (void*)&(struct store_keyword){ .value = CREATE });
	hash_set(tokenizer_keywords, "select", (void*)&(struct store_keyword){ .value = SELECT });

	// init table_metadata_keywords;
	table_metadata_keywords = init_hash();
	hash_set(table_metadata_keywords, "int", (void*)&(struct store_keyword){ .value = INT });
	hash_set(table_metadata_keywords, "float", (void*)&(struct store_keyword){ .value = FLOAT });
	hash_set(table_metadata_keywords, "varchar", (void*)&(struct store_keyword){ .value = CHAR });
	hash_set(table_metadata_keywords, "primary", (void*)&(struct store_keyword){ .value = PRIMARY });
	hash_set(table_metadata_keywords, "n_nil", (void*)&(struct store_keyword){ .value = NOT_NULL });

	input_buffer_T* buffer = init_input_buffer();
	token_buffer_T* t_buffer = init_token_buffer();

	while (true)
	{
		printf("dql> ");

		input_buffer_read(buffer);
		token_buffer_create(t_buffer, buffer->buffer);
		analyzer_T* analyzer = init_analyzer(t_buffer->token_buffer);
		ast_T* root = analyzer_analyze(analyzer);
		generator_T* generator = init_generator(".");
		generator_generate(generator, root);
	}

	return 0;
}
