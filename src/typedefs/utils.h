#ifndef UTILS_H
#define UTILS_H

#include <execinfo.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jmp_buf savebuff;

// comment this in/out to toggle debug mode
// #define DEBUG

// Path to the docs folder from current directory
#define DOCS_PATH "/docs"

#define TRY if (setjmp(savebuff) == 0)
#define CATCH else
#define THROW longjmp(savebuff, 1);

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)                                           \
    TRY                                                     \
    {                                                       \
        if (!(n)) {                                         \
            THROW                                           \
        }                                                   \
    }                                                       \
    CATCH                                                   \
    {                                                       \
        static void *callstack[128];                        \
        int frames = backtrace(callstack, 128);             \
        char **strs = backtrace_symbols(callstack, frames); \
        for (int i = 0; i < frames; ++i) {                  \
            printf("%s\n", strs[i]);                        \
        }                                                   \
        free(strs);                                         \
        assert(n);                                          \
    }
#endif

void logger(char *text);
char *getMyCWD(void);

#endif
