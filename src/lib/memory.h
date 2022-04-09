#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>

void* memalloc(size_t size);
void* memrealloc(void* ptr, size_t size);
void memfree(void* ptr);
const char* memstrdup(const char*);

#ifdef NEED_STRDUP
#define strdup memstrdup
#define _strdup memstrdup
#endif

#define _malloc(s)      memalloc(s)
#define _calloc(n, s)   memalloc((n)*(s))
#define _realloc(p, s)  memrealloc((p), (s))
#define _free(p)        memfree(p)
#define _alloc_ds(t)    (t*)memalloc(sizeof(t))
#define _alloc_ds_array(t, n)       (t*)memalloc((n)*sizeof(t))
#define _realloc_ds_array(p, t, n)  (t*)memrealloc((p), sizeof(t)*(n))

#endif