#ifndef __HASH_H__
#define __HASH_H__

#include <stdlib.h>

/*
 * This hash library is inspired (copied) from this reference.
 * https://benhoyt.com/writings/hash-table-in-c/
 *
 * Legal Notice:
 * You may use however you want to, including the right godfather.
 *
 * It also uses `djb2` hashing function. 
 * reference: http://www.cse.yorku.ca/~oz/hash.html
*/

/*
 * hash struct consist of capacity, length and buffer.
 * it is a pretty simple and straight forward hash library.
*/
typedef struct HASH_STRUCT hash_T;

/*
 * init hash table with default capacity,
 * it still needs improvement for allocating more than default capacity.
 *
 * TODO: More capacity.
*/
hash_T* init_hash(void);

/*
 * it will free the buffer first and then free the hash itself.
 * It may need to free all the keys, as in `benhoyt` implementation.
 *
 * But, for my use case i think it is fine to leave it as it is.
*/
void hash_free(hash_T*);

/*
 * It will return list of keys,
 * that are present int the hashmap.
*/
const char** hash_bucket(hash_T*);

/*
 * It will set the value if it founds the key in entry,
 * else it will create new entry.
*/
void hash_set(hash_T*, const char*, void*);

/*
 * it will loop through from hash_index till capacity.
 * if it found non-null value, it will return.
 * else null will be returned
*/
void* hash_get(hash_T*, const char*);

#endif
