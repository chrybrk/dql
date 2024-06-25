#include "include/array.h"

array_T* init_array(ssize_t item_size)
{
	array_T* new_array = calloc(1, sizeof(struct ARRAY_STRUCT));
	new_array->item_size = item_size;
	new_array->buffer = NULL;
	new_array->index = 0;
	return new_array;
}

void array_push(array_T* array, void* item)
{
	array->index++;
	if (array->buffer) array->buffer = realloc(array->buffer, array->item_size * array->index);
	else array->buffer = calloc(1, array->item_size * array->index);
	array->buffer[array->index - 1] = item;
}
