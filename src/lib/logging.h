#ifndef LOGGING_H
#define LOGGING_H

void setTraceLevel(int lev);
int getTraceLevel();
void trace(int level, const char* fmt, ...);

void fatalError(const char* fmt, ...);

#ifdef DEBUG_VM_TRACE
#define VTRACE(n, f, ...) do { \
        if((n) <= getTraceLevel()) { \
            marker(f, ## __VA_ARGS__ ); \
        }} while(0)
void marker(const char* fmt, ...);
#else
// This should optimize out of the code for a release build.
#define VTRACE(n, f, ...) do {} while(0)
#endif

// defined by the app that the library links to
void genericError(const char* fmt, ...);

#endif