#include <stdio.h>
#include <cstring>
#include <cstdint>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <dev.h>
#include <dev/spio.h>
#include <sys/fs/raw/drive.h>
#include <sys/fs/fat/partition.h>
#include <sys/ios/fio.h>
#include <utility>

bool  test_fat_01()
{
      // create SD device
      dev::spio l_sda(spi1, 1000000);

      // open the first partition on the device
      sys::fat::partition l_part0(std::addressof(l_sda), 0);

      // open a file on the partition
      sys::fio(std::addressof(l_part0), "dummy.txt", O_RDWR, 0777);

      return true;
}

int   main(int, char**)
{
      stdio_init_all();
      test_fat_01();
      return 0;
}
