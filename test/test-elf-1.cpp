#include <stdio.h>
#include <uld/object.h>
#include <uld/elf32.h>
#include <uld/target/boot.h>
#include <uld/target/memory.h>
#include <gamebit.h>

bool  test_elf_01() noexcept
{
      uld::boot          l_boot;
      uld::elf32::loader l_loader;
      if(l_loader.load("blink.elf", l_boot, 0)) {
          return true;
      }
      return false;
}

int   main(int, char**)
{
      gamebit::initialise();
      test_elf_01();
      gamebit::dispose();
      return 0;
}
