#ifndef VAR_STORE_H
#define VAR_STORE_H

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
        uint32_t store_idx; // strings and structs
    } data;
#ifdef DEBUG_INFO
    const char file[64];
    int line;
    int col;
#endif
} __attribute__((__packed__)) Variable;

void createVarStore();
void destroyVarStore();

Variable* createVar(uint16_t type);
int addVar(Variable* var);
Variable* getVar(int index);
Variable* copyVar(Variable* var);
void printVar(Variable* var);
//const char* varTypeToStr(int type);
void assignVar(Variable* dest, Variable* src);
void castVar(Variable* var, uint8_t type);

void loadVarStore(FILE* fp);
void saveVarStore(FILE* fp);

void dumpVarStore();

#endif