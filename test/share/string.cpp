#include "string.h"
#include <stdio.h>
#include <stdarg.h>

void  sys_sprintf(char* string, char* format, ...) noexcept
{
      va_list va;
      va_start(va, format);
      vsprintf(string, format, va);
      va_end(va);
}

void  sys_snprintf(char* string, size_t size, char* format, ...) noexcept
{
      va_list va;
      va_start(va, format);
      vsnprintf(string, size, format, va);
      va_end(va);
}
