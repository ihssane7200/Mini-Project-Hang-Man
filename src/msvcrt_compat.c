#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

// This file provides compatibility stubs for linking a UCRT/modern-MinGW 
// compiled libraylib.a against the legacy MinGW.org GCC 6.3.0 compiler.

extern FILE _iob[];

// Standard I/O streams for UCRT
FILE *__cdecl __acrt_iob_func(unsigned index) {
    return &_iob[index];
}

FILE *(*_imp____acrt_iob_func)(unsigned index) = __acrt_iob_func;

// vsscanf stub
int __mingw_vsscanf(const char * buffer, const char * format, va_list argPtr) {
    return vsscanf(buffer, format, argPtr);
}

// strtod stub
double __mingw_strtod(const char *nptr, char **endptr) {
    return strtod(nptr, endptr);
}

// math stubs
void sincosf(float x, float *s, float *c) {
    *s = sinf(x);
    *c = cosf(x);
}
