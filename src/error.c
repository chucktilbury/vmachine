
#include "common.h"
#include <stdarg.h>

static int trace_level;

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

void trace(const char* file, const char* func, int line, int level, const char* fmt, ...)
{
    if(level >= trace_level) {
        fprintf(stderr, "trace: %s: %s: %d: ", file, func, line);
        va_list args;

        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);

        fprintf(stderr, "\n");
    }
}

void setTraceLevel(int lev)
{
    trace_level = lev;
}