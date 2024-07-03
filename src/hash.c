#include <string.h>
#include "include/hash.h"

#define MAX_HASH_TABLE_CAPACITY 1024

typedef struct {
	const char* key;
	void* value;
} hash_entry;

typedef struct HASH_STRUCT {
	hash_entry* buffer;
	ssize_t capacity;
	ssize_t length;
	char** keys;
} hash_T;

/*
 * Hashing function
 * 
 * It is a djb2 hashing function.
 * reference: http://www.cse.yorku.ca/~oz/hash.html
 *
 * Legal Notice:
 * I am not the godfather of this hashing function but if it works on your project, use it.
*/
unsigned long long
hash_function(const char* string)
{
	// magic number is supposed to be `33`.
	unsigned long hash = 5381;
	int c;

	while ((c = *string++) != 0)
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

hash_T* init_hash(void)
{
	hash_T* hash = malloc(sizeof(struct HASH_STRUCT));
	if (hash == NULL) return NULL;

	hash->length = 0;
	hash->capacity = MAX_HASH_TABLE_CAPACITY;
	hash->buffer = malloc(sizeof(hash_entry) * hash->capacity);
	if (hash->buffer == NULL)
	{
		free(hash);
		return NULL;
	}

	return hash;
}

void hash_free(hash_T* hash)
{
	free(hash->keys);
	free(hash->buffer);
	free(hash);
}

void hash_set(hash_T* hash, const char* key, void* value)
{
	unsigned long long magic_number = hash_function(key);
	ssize_t index = (ssize_t)(magic_number & (MAX_HASH_TABLE_CAPACITY - 1));

	int if_found = 0;

	while (hash->buffer[index].key != NULL)
	{
		if (strcmp(key, hash->buffer[index].key) == 0)
		{
			hash->buffer[index].value = value;
			if_found = 1;
			break;
		}

		index++;
		if (index >= MAX_HASH_TABLE_CAPACITY) index = 0;
	}

	if (if_found == 0)
	{
		hash->buffer[index].key = (const char*)strdup(key);
		hash->buffer[index].value = value;
		hash->length++;

		if (hash->keys)
			hash->keys = realloc(hash->keys, sizeof(char*) * hash->length);
		else
			hash->keys = calloc(1, sizeof(char*));

		hash->keys[hash->length - 1] = (char*)strdup(key);
	}
}

void* hash_get(hash_T* hash, const char* key)
{
	unsigned long long magic_number = hash_function(key);
	ssize_t index = (ssize_t)(magic_number & (MAX_HASH_TABLE_CAPACITY - 1));

	while (hash->buffer[index].key != NULL)
	{
		if (strcmp(key, hash->buffer[index].key) == 0)
			return hash->buffer[index].value;

		index++;
		if (index >= MAX_HASH_TABLE_CAPACITY) index = 0;
	}
	
	return NULL;
}

const char** hash_bucket(hash_T* hash)
{
	return (const char**)hash->keys;
}
