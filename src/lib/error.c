
#include "common.h"
#include <stdarg.h>

static int trace_level = 6;



void marker(const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

// void trace(const char* file, const char* func, int line, int level, const char* fmt, ...)
// {
//     fprintf(stderr, "trace: %s: %s: %d: ", file, func, line);
//     va_list args;
//
//     va_start(args, fmt);
//     vfprintf(stderr, fmt, args);
//     va_end(args);
//
//     fprintf(stderr, "\n");
// }

void setTraceLevel(int lev)
{
    trace_level = lev;
}

int getTraceLevel()
{
    return trace_level;
}