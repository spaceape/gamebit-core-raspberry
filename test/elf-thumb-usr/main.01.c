#include "sys.h"
#include "memory.h"

static const int test2_buffer_size = 8;
static const int test3_buffer_size = 256;

bool  test_01()
{
      uint8_t* data = malloc(256);
      data[0] = 0;
      free(data);
      return data != NULL;
}

bool  test_02()
{
      uint8_t* data = malloc(test2_buffer_size);
      for(int x = 0; x < test2_buffer_size; x++) {
          data[x] = x & 255;
      }
      free(data);
      return data != NULL;
}

bool  test_03()
{
      uint8_t* data = malloc(test3_buffer_size);
      for(int x = 0; x < test3_buffer_size; x++) {
          data[x] = x & 255;
      }
      free(data);
      return data != NULL;
}

bool  test_33(uint8_t* p, int size)
{
      uint8_t* data = malloc(size);
      for(int x = 0; x < size; x++) {
          data[x] = x & 255;
      }
      memcpy(p, data, size);
      free(data);
      return data != NULL;
}

int   main()
{
      int l_success = 
            (test_01() << 0) |
            (test_02() << 1) |
            (test_03() << 2);
      return l_success ^ 7;
}
