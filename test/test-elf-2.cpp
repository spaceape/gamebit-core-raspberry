#include <stdio.h>
#include "smp.h"
#include "pico/stdio.h"

bool  test_elf_01() noexcept
{
      return true;
}

int   main(int, char**)
{
      stdio_init_all();
      if(true) {
          smp l_env;
          l_env.load("sumxy.o");
      }
      return 0;
}
