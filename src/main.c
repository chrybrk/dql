#include <stdbool.h>
#include "include/REPL.h"

int main(void)
{
	T_input_buffer* buffer = init_input_buffer();
	while (true)
	{
		printf("dql> ");
		input_buffer_read(buffer);
		printf("%s\n", buffer->buffer);
	}

	return 0;
}
