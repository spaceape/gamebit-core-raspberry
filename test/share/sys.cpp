#include "sys.h"
#include <stdio.h>

void  sys_abort() noexcept
{
      printf("/!\\ CAUGHT: abort() from user program.\n");
}

void  sys_exit(int rc) noexcept
{
      printf("/!\\ CAUGHT: exit(%d) from user program.\n", rc);
}

void  sys_signal(int signal, void* data) noexcept
{
      printf("/!\\ CAUGHT: signal(%d, %p) from user program.\n", signal, data);
}
