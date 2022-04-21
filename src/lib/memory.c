
#include "common.h"

#ifdef DEBUG_MEMORY

enum {
    MEM_ALLOC,
    MEM_REALLOC,
    MEM_STRDUP,
};

typedef struct _mem_entry_ {
    const char* file;
    const char* func;
    int line;
    int oper;
    size_t size;
    struct _mem_entry_* prev;
    struct _mem_entry_* next;
} mem_header;

typedef struct {
    mem_header* head;
    mem_header* tail;
    size_t total_alloc;
    size_t total_free;
    size_t crnt_alloc;
    size_t num_allocs;
    size_t num_frees;
} memory_tracker;

static memory_tracker tracker = {NULL, NULL, 0, 0, 0, 0, 0};

#define MEM_TO_STRUC(p)     (mem_header*)(((uint64_t)(p))-sizeof(mem_header))
#define STRUC_TO_MEM(p)     (void*)(((uint64_t)(p))+sizeof(mem_header))
#define ALLOC_SIZE(s)       ((s)+sizeof(mem_header))

static const char* dup_string(const char* str)
{
    const char* retv = NULL;

    if(str != NULL) {
        size_t len = strlen(str) + 1;
        retv = malloc(len);
        memcpy((void*)retv, str, len);
    }
    else {
        retv = (const char*)malloc(1);
        ((char*)(retv))[0] = '\0';
    }

    assert(retv != NULL);
    return retv;
}

static void dump()
{
    printf("Dump Memory\n");
    if(tracker.head != NULL) {
        mem_header* tmp, *next;
        for(tmp = tracker.head; tmp != NULL; tmp = next) {
            next = tmp->next;
            printf("alloc: %s: %s: %d: %lu: %p\n", tmp->file, tmp->func, tmp->line, tmp->size, tmp);
        }
    }
    printf("total_alloc: %lu\n", tracker.total_alloc);
    printf("total_free:  %lu\n", tracker.total_free);
    printf("crnt_alloc:  %lu\n", tracker.crnt_alloc);
    printf("num_allocs:  %lu\n", tracker.num_allocs);
    printf("num_frees:   %lu\n", tracker.num_frees);

}

static void del(mem_header* ptr)
{
    if(tracker.head != NULL) {
        mem_header* next = ptr->next;
        mem_header* prev = ptr->prev;
        if(prev != NULL) {
            prev->next = next;
        }

        if(next != NULL) {
            next->prev = prev;
        }



        free((void*)ptr->file);
        free((void*)ptr->func);
        free(ptr);

        tracker.num_frees++;
        tracker.crnt_alloc -= ptr->size;
        tracker.total_free += ptr->size;
    }
    else {
        fprintf(stderr, "memory error: cannot find pointer to free: %p\n", ptr);
        exit(1);
    }
}

static void destroy()
{
    if(tracker.head != NULL) {
        mem_header* tmp, *next;
        for(tmp = tracker.head; tmp != NULL; tmp = next) {
            next = tmp->next;
            del(tmp);
        }
    }
}

static void add(mem_header* ptr)
{
    if(tracker.head != NULL) {
        tracker.head->prev = ptr;
        ptr->next = tracker.head;
        tracker.head = ptr;
    }
    else {
        tracker.head = ptr;
        tracker.tail = ptr;
        atexit(dump);
        //atexit(destroy);
    }

    tracker.num_allocs++;
    tracker.total_alloc += ptr->size;
    tracker.crnt_alloc += ptr->size;
}

void* memalloc(const char* fname, const char* func, int line, size_t size)
{
    mem_header* mem = malloc(ALLOC_SIZE(size));
    assert(mem != NULL);

#ifdef TRACE_MEMORY
    printf("alloc: %s: %s: %d: %lu: %p\n", fname, func, line, size, mem);
#endif

    memset(mem, 0, ALLOC_SIZE(size));

    mem->file = dup_string(fname);
    mem->func = dup_string(func);
    mem->line = line;
    mem->size = size;
    mem->oper = MEM_ALLOC;

    add(mem);

    return STRUC_TO_MEM(mem);
}

void* memrealloc(const char* fname, const char* func, int line, void* ptr, size_t size)
{
    mem_header* mem = malloc(ALLOC_SIZE(size));
    assert(mem != NULL);

#ifdef TRACE_MEMORY
    printf("realloc: %s: %s: %d: %lu: %p\n", fname, func, line, size, mem);
#endif

    memcpy(STRUC_TO_MEM(mem), ptr, size);

    mem->file = dup_string(fname);
    mem->func = dup_string(func);
    mem->line = line;
    mem->size = size;
    mem->oper = MEM_REALLOC;

    add(mem);
    del(MEM_TO_STRUC(ptr));

    return STRUC_TO_MEM(mem);
}

void memfree(const char* fname, const char* func, int line, void* ptr)
{
    mem_header* mem = MEM_TO_STRUC(ptr);
#ifdef TRACE_MEMORY
    printf("free: %s: %s: %d: %lu: %p\n", fname, func, line, mem->size, mem);
#endif

    del(mem);
}

const char* memstrdup(const char* fname, const char* func, int line, const char* str)
{
    size_t size = strlen(str) + 1;
    mem_header* mem = malloc(ALLOC_SIZE(size));
    assert(mem != NULL);

#ifdef TRACE_MEMORY
    printf("strdup: %s: %s: %d: %lu: %p\n", fname, func, line, size, mem);
#endif

    memcpy(STRUC_TO_MEM(mem), str, size);

    mem->file = dup_string(fname);
    mem->func = dup_string(func);
    mem->line = line;
    mem->size = size;
    mem->oper = MEM_ALLOC;

    add(mem);

    return STRUC_TO_MEM(mem);
}


#else
#ifdef NEED_STRDUP
const char* memstrdup(const char* str)
{
    if(str != NULL) {
        size_t len = strlen(str) + 1;
        char* s = malloc(len);
        assert(s != NULL);
        memcpy(s, str, len);
        return s;
    }
    else {
        char* s = malloc(1);
        assert(s != NULL);
        s[0] = '\0';
        return s;
    }
}
#endif

void* memalloc(size_t size)
{
    void* ptr = malloc(size);
    assert(ptr != NULL);
    memset(ptr, 0, size);
    return ptr;
}

void* memrealloc(void* ptr, size_t size)
{
    void* nptr = realloc(ptr, size);
    assert(nptr != NULL);
    return nptr;
}

void memfree(void* ptr)
{
    if(ptr != NULL) {
        free(ptr);
    }
}

#endif