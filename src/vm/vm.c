
#include "common.h"
#include "vmachine.h"

void runMachine(); // defined in runner.c

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
    cmd_line cl = create_cmd_line("Virtual machine runner.");
    add_num_param(cl, "tlevel", "-t", "set the trace level", 0, 0);
    add_str_param(cl, "tfile", "-f", "file name to write the trace to", "stdout", 0);
    parse_cmd_line(cl, argc, argv);

    setTraceLevel(get_num_param(cl, "tlevel"));
    reset_cmd_excess(cl);

    return virtualMachine(iterate_cmd_excess(cl));
}