#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#ifdef DEBUG_MEMORY

void* memalloc(const char* fname, const char* func, int line, size_t size);
void* memrealloc(const char* fname, const char* func, int line, void* ptr, size_t size);
void memfree(const char* fname, const char* func, int line, void* ptr);
const char* memstrdup(const char* fname, const char* func, int line, const char*);

#define _malloc(s)      memalloc(__FILE__, __func__, __LINE__, s)
#define _calloc(n, s)   memalloc(__FILE__, __func__, __LINE__, (n)*(s))
#define _realloc(p, s)  memrealloc(__FILE__, __func__, __LINE__, (p), (s))
#define _free(p)        memfree(__FILE__, __func__, __LINE__, p)
#define _alloc_ds(t)    (t*)memalloc(__FILE__, __func__, __LINE__, sizeof(t))
#define _alloc_ds_array(t, n)       (t*)memalloc(__FILE__, __func__, __LINE__, (n)*sizeof(t))
#define _realloc_ds_array(p, t, n)  (t*)memrealloc(__FILE__, __func__, __LINE__, (p), sizeof(t)*(n))
#define strdup(s)   memstrdup(__FILE__, __func__, __LINE__, s)
#define _strdup(s)  memstrdup(__FILE__, __func__, __LINE__, s)

#else

void* memalloc(size_t size);
void* memrealloc(void* ptr, size_t size);
void memfree(void* ptr);
const char* memstrdup(const char*);

#define _malloc(s)      memalloc(s)
#define _calloc(n, s)   memalloc((n)*(s))
#define _realloc(p, s)  memrealloc((p), (s))
#define _free(p)        memfree(p)
#define _alloc_ds(t)    (t*)memalloc(sizeof(t))
#define _alloc_ds_array(t, n)       (t*)memalloc((n)*sizeof(t))
#define _realloc_ds_array(p, t, n)  (t*)memrealloc((p), sizeof(t)*(n))
#define strdup(s)       memstrdup(s)
#define _strdup(s)      memstrdup(s)
#endif

#endif