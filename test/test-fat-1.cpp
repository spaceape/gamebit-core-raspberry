#include <stdio.h>
#include <cstring>
#include <cstdint>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <dev.h>
#include <dev/spio.h>
#include <sys/fs/raw/filesystem.h>
#include <sys/fs/fat/partition.h>
#include <utility>

bool  test_fat_01()
{
      dev::spio l_sda(spi1, 1000000);
      sys::fat::partition l_part0(std::addressof(l_sda), 0);

      l_part0.open("dummy.txt");

      return true;
}

int   main(int, char**)
{
      stdio_init_all();
      test_fat_01();
     
      return 0;
}
