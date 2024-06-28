#ifndef __DEFS_H__
#define __DEFS_H__

#include "hash.h"

struct tokenizer_keyword_data {
	ssize_t value;
};
extern hash_T* tokenizer_keywords;

#endif
