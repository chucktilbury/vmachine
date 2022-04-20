#ifndef _VAR_STORE_H_
#define _VAR_STORE_H_

#include <stdint.h>
#include <stddef.h>
#include "var_type.h"

typedef struct {
    //ValType type;
    uint8_t type;
    bool isAssigned;// has a value
    bool isConst;   // declared as constant
    bool isLiteral; // not connected to a symbol
    //uint32_t hash;
    union {
        uint32_t unum;
        uint32_t addr;
        int32_t inum;
        float fnum;
        bool boolean;
        void* obj;
    } data;
} Variable;

typedef struct {
    Variable** list;
    size_t cap;
    size_t len;
} VarStore;

void createVarStore();
void destroyVarStore();

Variable* createVar(uint16_t type);
int addVar(Variable* var);
Variable* getVar(int index);
Variable* copyVar(Variable* var);
void printVar(Variable* var);
const char* varTypeToStr(int type);
void assignVar(Variable* dest, Variable* src);
void castVar(Variable* var, uint8_t type);

void loadVarStore(FILE* fp);
void saveVarStore(FILE* fp);


#endif