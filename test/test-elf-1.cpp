#include <stdio.h>
#include <uld/object.h>
#include <uld/elf32.h>
#include <uld/target/boot.h>
#include <uld/target/memory.h>

#include <pico/stdlib.h>
#include <hardware/watchdog.h>

#include <gamebit.h>

bool  test_elf_01() noexcept
{
      uld::boot          l_target;
      uld::elf32::loader l_loader;
      sleep_ms(2000);
      printf("Loading \"blink.elf\"\n");
      if(l_loader.load("blink.elf", l_target, 0)) {
          l_target.exec();
          return false;
      }
      return false;
}

void  test_reboot()
{
      sleep_ms(2000);
      watchdog_enable(1000, 1);
      while(true) {
      }
}

int   main(int, char**)
{
      gamebit::initialise();
      test_elf_01();
      // test_reboot();
      gamebit::dispose();
      return 0;
}
