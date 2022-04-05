#ifndef _UTILS_H_
#define _UTILS_H_

#ifndef strdup
const char* strdup(const char*);
#endif

void* memalloc(size_t size);
void* memrealloc(void* ptr, size_t size);
void memfree(void* ptr);

#define _malloc(s)      memalloc(s)
#define _calloc(n, s)   memalloc((n)*(s))
#define _realloc(p, s)  memrealloc((p), (s))
#define _free(p)        memfree(p)
#define _alloc_ds(t)    (t*)memalloc(sizeof(t))
#define _alloc_ds_array(t, n)       (t*)memalloc((n)*sizeof(t))
#define _realloc_ds_array(p, t, n)  (t*)memrealloc((p), sizeof(t)*(n))

typedef struct {
    void** list;
    int cap;
    int len;
    int idx;
} GenericPtrList;

GenericPtrList* createGPL();
void destroyGPL(GenericPtrList* gpl);

int addGPL(GenericPtrList* gpl, void* data);
void* getGPL(GenericPtrList* gpl, int idx);
void setGPL(GenericPtrList* gpl, int idx, void* data);
void* delGPL(GenericPtrList* gpl, int idx);

void resetGPL(GenericPtrList* gpl);
void* iterGPL(GenericPtrList* gpl);

int pushGPL(GenericPtrList* gpl, void* data);
void* popGPL(GenericPtrList* gpl);
void* peekGPL(GenericPtrList* gpl);

#endif