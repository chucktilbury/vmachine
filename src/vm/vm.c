
#include "common.h"
#include <stdarg.h>

void runMachine(); // defined in runner.c

void runtimeError(const char* fmt, ...)
{
    fprintf(stderr, "Runtime Error: ");
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
    exit(1);
}

void dump_buffer(uint8_t* buffer, int size)
{

    for(int i = 0; i < size; i++) {
        printf(", 0x%02X", buffer[i]);
    }
    printf("\n");
}

int virtualMachine(const char* fname)
{
    // load the file
    loadVM(fname); //createVirtualMachine();
    createCallStack();

    // run the program
    runMachine();

    destroyVMachine();
    return 0;
}

int main(int argc, char** argv)
{
    setTraceLevel(3);

    if(argc < 2) {
        fprintf(stderr, "use: %s filename\n", argv[0]);
        exit(1);
    }

    return virtualMachine(argv[1]);
}