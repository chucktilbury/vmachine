#ifndef _VM_ERROR_H_
#define _VM_ERROR_H_

void runtimeError(const char* fmt, ...);
void genericError(const char* fmt, ...);

#endif