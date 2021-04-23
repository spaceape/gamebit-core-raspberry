#include <stdio.h>
#include <cstring>
#include <cstdint>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <dev.h>
#include <dev/bd/spio.h>
#include <dev/bd/drive.h>
#include <dev/bd/fat/part.h>
#include <sys/ios/fio.h>
#include <utility>

bool  test_fat_01()
{
      // create SD device
      dev::spio l_sda(4000000);

      // open the first partition on the device
      dev::fat::part l_part0(std::addressof(l_sda), 0);

      // open a file on the partition
      sys::fio l_file(std::addressof(l_part0), "dummy.txt", O_RDWR, 0777);

      char data[256];
      int  read_size;
      int  read_offset = 0;
      while(read_offset < l_file.get_size()) {
          read_size = l_file.read(256, data);
          if(read_size) {
              for(int index = 0; index < read_size; index++) {
                  auto value = data[index];
                  if(value > 31 && value < 127) {
                      printf("%c  ", value);
                  } else
                      printf("%.2x ", value);
              }
              printf("\n");
          } else
              break;
          read_offset += read_size;
      }

      return true;
}

int   main(int, char**)
{
      stdio_init_all();
      test_fat_01();
      return 0;
}
