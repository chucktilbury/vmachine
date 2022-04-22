#ifndef VM_ERROR_H
#define VM_ERROR_H

void runtimeError(const char* fmt, ...);
void genericError(const char* fmt, ...);

#endif