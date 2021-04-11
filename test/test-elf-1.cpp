#include <stdio.h>
#include <com.h>
#include <io.h>
#include <uld/object.h>
#include <uld/elf32.h>

bool  test_elf_01() noexcept
{
      uld::target        l_target;
      uld::elf32::loader l_loader;
      if(l_loader.load("blink.elf", l_target, 0)) {
      }
      return true;
}

int   main(int, char**)
{
      com l_com;
      io  l_filesystem;
      test_elf_01();
      return 0;
}
