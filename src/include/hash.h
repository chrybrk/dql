#ifndef __HASH_H__
#define __HASH_H__

/*
 * It is pretty simple hashmap library and it uses basic algorithm to find index where it would fit the key.
 * Formula:
 * 	int index = 0;
 * 	loop through "test" in char {
 * 		index = index + (int)char;
 * 	}
*/

#include <stdlib.h>

#define DEFAULT_SIZE 1024

/*
 * char* key
 * ssize_t value
*/
typedef struct {
	char* key;
	ssize_t value;
} HashPair;

/*
 * size: total size of buffer
 * pairs: actual buffer that stores pair
*/
typedef struct HASH_STRUCT {
	ssize_t size;
	HashPair** pairs;
} hash_T;

// initialize hashmap
hash_T* init_hash(ssize_t max_size);

/*
 * hash_insert(hash_T*, char*, ssize_t)
 * insert value into hashmap
*/
void hash_insert(hash_T* hash, char* key, ssize_t value);

/*
 * hash_find(hash_T*, char*)
 * find value from hashmap
*/
HashPair* hash_find(hash_T* hash, char* key);

#endif
