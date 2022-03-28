#ifndef _ERROR_H_
#define _ERROR_H_

void runtimeError(const char* fmt, ...);
void setTraceLevel(int lev);

#ifdef DEBUG_VM_TRACE
#define TRACE(n, f, ...) do {trace(__FILE__, __func__, __LINE__, n, f, ## __VA_ARGS__ );} while(0)
void trace(const char* file, const char* func, int line, int level, const char* fmt, ...);
#else
#define TRACE(n, f, ...) do {} while(0)
#endif

#endif