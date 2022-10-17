#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_FMT_LEN 1024

#define FMT(arg) _Generic((arg), \
    char        : "\'%c\'", \
    int         : "%d", \
    size_t      : "%lu", \
    float       : "%f", \
    const char* : "\"%s\"", \
    char*       : "\"%s\"", \
    void*       : "%p", \
    ptrdiff_t   : "%lu" )

#define FMT2( a, b) FMT(a), FMT(b)
#define FMT3( a, b, c) FMT2(a, b), FMT(c)
#define FMT4( a, b, c, d) FMT3(a, b, c), FMT(d)
#define FMT5( a, b, c, d, e) FMT4(a, b, c, d), FMT(e)
#define FMT6( a, b, c, d, e, f) FMT5(a, b, c, d, e), FMT(f)
#define FMT7( a, b, c, d, e, f, g) FMT6(a, b, c, d, e, f), FMT(g)
#define FMT8( a, b, c, d, e, f, g, h) FMT7(a, b, c, d, e, f, g), FMT(h)
#define FMT9( a, b, c, d, e, f, g, h, i) FMT8(a, b, c, d, e, f, g, h), FMT(i)
#define FMT10(a, b, c, d, e, f, g, h, i, j) FMT9(a, b, c, d, e, f, g, h, i), FMT(j)

#define UNPACK(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ARGN, ...) ARGN 

#define ARG1(...) 1, UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)
#define ARG2(...) 2, UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)
#define ARG3(...) 3, UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)
#define ARG4(...) 4, UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)
#define ARG5(...) 5, UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)
#define ARG6(...) 6, UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)
#define ARG7(...) 7, UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)
#define ARG8(...) 8, UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)
#define ARG9(...) 9, UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)
#define ARG10(...)10,UNPACK(__VA_ARGS__, FMT10, FMT9, FMT8, FMT7, FMT6, FMT5, FMT4, FMT3, FMT2, FMT)(__VA_ARGS__)

#define PRINT(delim, ...) printf(build_format(delim, 1, UNPACK(__VA_ARGS__, ARG10, ARG9, ARG8, ARG7, ARG6, ARG5, ARG4, ARG3, ARG2, ARG1)(__VA_ARGS__)), __VA_ARGS__)

char* build_format(const char* delimiter, int grouping, int count, ...)
{
    static char fmt[MAX_FMT_LEN] = {0};
    fmt[0] = '\0';

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i)
    {
         const char *arg = va_arg(args, const char*);
         strcat(fmt, arg);
         if (i < count - 1 && (i + 1) % grouping == 0) strcat(fmt, delimiter);
    } 
    va_end(args);
    
    strcat(fmt, "\n");
    return fmt;
}

int main (int argc, char *argv[])
{
    char buf[5] = {'a', 'b', 'c', 'd', 0};
    int num = 99;
    char ch = 'A';
    void *ptr = &ch;
    ptrdiff_t diff = 30;

    PRINT("\n", num, buf, ch, ptr, diff, 1 + 2, true, false);

    return 0;
}
