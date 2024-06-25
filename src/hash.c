#include <stdio.h>
#include <string.h>
#include "include/hash.h"

// utility function
ssize_t hash_get_index(char* key, ssize_t max_size)
{
	// formula
	ssize_t index = 0;
	for (size_t i = 0; i < strlen(key); i++)
	{
		index += (int)key[i];
	}

	// clamp it to max size
	index = index % max_size;
	
	return index;
}

hash_T* init_hash(ssize_t max_size)
{
	hash_T* new_hash = calloc(1, sizeof(struct HASH_STRUCT));
	new_hash->pairs = calloc(1, new_hash->size * sizeof(HashPair));
	new_hash->size = max_size;
	return new_hash;
}

void hash_insert(hash_T* hash, char* key, ssize_t value)
{
	ssize_t index = hash_get_index(key, hash->size);

	// retrive already existing value *if
	HashPair* try_existing_value = hash->pairs[index];

	if (!try_existing_value)
	{
		HashPair* new_pair = calloc(1, sizeof(HashPair));
		new_pair->key = key;
		new_pair->value = value;
		hash->pairs[index] = new_pair;
	}
	else
	{
		int flag = 1;
		for (ssize_t i = index; i < hash->size; i++)
		{
			try_existing_value = hash->pairs[i];

			if (try_existing_value && !strcmp(key, try_existing_value->key))
			{
				printf("[ERROR]: hash: failed to insert `%s` into hashmap, it already exist.\n", key);
				exit(EXIT_FAILURE);
			}
			else
			{
				HashPair* new_pair = calloc(1, sizeof(HashPair));
				new_pair->key = key;
				new_pair->value = value;
				hash->pairs[i] = new_pair;
				flag = 0;
			}
		}

		if (!flag)
		{
			printf("[ERROR]: hash: failed to insert `%s`, overflow.\n", key);
			exit(EXIT_FAILURE);
		}
	}
}

HashPair* hash_find(hash_T* hash, char* key)
{
	ssize_t index = hash_get_index(key, hash->size);

	// retrive already existing value *if
	HashPair* try_existing_value = hash->pairs[index];
	if (!try_existing_value) return NULL;

	if (!strcmp(key, try_existing_value->key)) return try_existing_value;

	for (ssize_t i = index + 1; i < hash->size; i++)
	{
		try_existing_value = hash->pairs[index];
		if (!strcmp(key, try_existing_value->key)) return try_existing_value;
	}

	return NULL;
}
