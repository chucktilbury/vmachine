
#include "common.h"

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
    VMachine* vm = loadVM(fname); //createVirtualMachine();
    createCallStack();

    // run the program
    runMachine(vm);

    destroyVMachine(vm);
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