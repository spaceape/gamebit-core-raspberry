#ifndef cor_string_h
#define cor_string_h
#include <stdlib.h>
// string.h
// bindings for a few basic string functions

void sys_sprintf(char*, char*, ...) noexcept;
void sys_snprintf(char*, size_t, char*, ...) noexcept;

#endif

