#ifndef _ERROR_H_
#define _ERROR_H_

void runtimeError(const char* fmt, ...);
void setTraceLevel(int lev);
int getTraceLevel();

#ifdef DEBUG_VM_TRACE
#define VTRACE(n, f, ...) do { \
        if((n) >= getTraceLevel()) { \
            marker(f, ## __VA_ARGS__ ); printf("\n"); \
        }} while(0)
void marker(const char* fmt, ...);
#else
#define VTRACE(n, f, ...) do {} while(0)
#endif

#endif