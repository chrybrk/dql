#ifndef __REPL_H__
#define __REPL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * INPUT_BUFFER_STRUCT to store REPL data.
*/
typedef struct INPUT_BUFFER_STRUCT {
	char* 	buffer;
	size_t 	buffer_length;
	ssize_t input_length;
} input_buffer_T;

// initialize input_buffer
input_buffer_T* init_input_buffer();

// read input from console and store into INPUT_BUFFER_STRUCT
void input_buffer_read(input_buffer_T* input_buffer);

#endif
