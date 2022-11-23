/** 
    Copyright (c) 2020, wicked systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
    conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
      disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
      disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of wicked systems nor the names of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
#include "log.h"
#include <stdarg.h>

void  vprintlog(const char* message, const char* file, int line, va_list va) noexcept
{
      vfprintf(stdout, message, va);
      fprintf(stdout, "\n");
}

void  printlog(const char* message, ...) noexcept
{
      va_list va;
      va_start(va, message);
      vfprintf(stdout, message, va);
      va_end(va);
      fprintf(stdout, "\n");
}

void  printlog(const char* message, const char* file, int line, ...) noexcept
{
      va_list va;
      va_start(va, line);
      vprintlog(message, file, line, va);
      va_end(va);
}

void  vprintdbg(const char* message, const char* file, int line, va_list va) noexcept
{
#ifndef NDEBUG
      fprintf(stderr, "(i) ");
      vfprintf(stderr, message, va);
      fprintf(stderr, "\n");
      if(file != nullptr) {
          if(line > 0) {
              fprintf(stderr, "    %s:%d\n", file, line);
          }
      }
#endif
}

void  printdbg(const char* message, ...) noexcept
{
#ifndef NDEBUG
      va_list va;
      va_start(va, message);
      vfprintf(stderr, message, va);
      va_end(va);
#endif
}

void  printdbg(const char* message, const char* file, int line, ...) noexcept
{
#ifndef NDEBUG
      va_list va;
      va_start(va, line);
      vprintdbg(message, file, line, va);
      va_end(va);
#endif
}
