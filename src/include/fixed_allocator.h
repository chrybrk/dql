#ifndef __FIXED_ALLOCATOR__
#define __FIXED_ALLOCATOR__

/*
 * Fixed allocator are linear allocator with,
 * each block being of fixed size.
 *
 * It is rather easy to implement,
 * and keep track of what blocks
 * have been freed.
 *
 * Fixed allocator implementation
 * would approach `Object oriented style`.
 *
 * @copyright 2024 - chrybrk
 * under legal terms:
 * 
 * You can use fixed allocator
 * however you want to,
 * if you do find bugs on this tool,
 * make sure you let me know.
 *
 * May god bless us.
*/

typedef struct FIXED_ALLOCATOR_STRUCT {
	void (*set);
	void (*get);
	void (*pop);
} fixed_allocator_T;

#endif
