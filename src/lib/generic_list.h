#ifndef _GENERIC_TREE_H_
#define _GENERIC_TREE_H_

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