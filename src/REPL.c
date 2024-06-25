#include "include/REPL.h"

input_buffer_T* init_input_buffer()
{
	input_buffer_T* new_input_buffer = calloc(1, sizeof(struct INPUT_BUFFER_STRUCT));
	new_input_buffer->buffer = NULL;
	new_input_buffer->buffer_length = 0;
	new_input_buffer->input_length = 0;

	return new_input_buffer;
}

void input_buffer_read(input_buffer_T* input_buffer)
{
	ssize_t total_bytes_read = getline(&input_buffer->buffer, &input_buffer->buffer_length, stdin);

	if (total_bytes_read <= 0)
	{
		fprintf(stdout, "Error reading input.\n");
		exit(EXIT_FAILURE);
	}

	input_buffer->input_length = total_bytes_read - 1;
	input_buffer->buffer[total_bytes_read - 1] = 0;
}
