#ifndef __ARRAY_H__
#define __ARRAY_H__

/*
 * It is a dynamic array utility, which might have bugs in future production.
 * Legal Notice:
 * 	You're allowed to use it however you want to, i don't care.
 * 	But, if you find a bug, please let me know.
*/

// TODO: Arena Allocator

#include <stdlib.h>

// Simple Dynamic Array
typedef struct ARRAY_STRUCT
{
	void** buffer;
	ssize_t index;
	ssize_t item_size;
} array_T;

/*
 * ssize_t: item_size
 * each element must have a similar size.
*/
array_T* init_array(ssize_t item_size);

/*
 * It will push at the end of the buffer.
*/
void array_push(array_T* array, void* item);

#endif
