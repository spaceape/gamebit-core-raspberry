#ifndef cor_sys_h
#define cor_sys_h
// sys.h
// bindings for a few basic "system" functions
#include <stdlib.h>

void  sys_abort() noexcept;
void  sys_exit(int rc) noexcept;
void  sys_signal(int signal, void* data) noexcept;
#endif
