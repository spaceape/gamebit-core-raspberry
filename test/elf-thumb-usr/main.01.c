#include "sys.h"
#include "memory.h"
#include "string.h"

static const int test2_buffer_size = 8;
static const int test3_buffer_size = 256;

char* message = "hello!";

int __aeabi_idiv(int lhs, int rhs)
{
    (void)lhs;
    (void)rhs;
    return 0;
}

unsigned int __aeabi_uidiv(unsigned int lhs, unsigned int rhs)
{
    (void)lhs;
    (void)rhs;
    return 0u;
}

int __aeabi_idivmod(int lhs, int rhs)
{
    (void)lhs;
    (void)rhs;
    return 0;
}

unsigned int __aeabi_uidivmod(unsigned int lhs, unsigned int rhs)
{
    (void)lhs;
    (void)rhs;
    return 0u;
}

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

char* test_04()
{
      return message;
}

char* test_05(int value)
{
      static char text[64];
      memset(text, value, 64);
      return text;
}

char* test_06(int value)
{
      static char text[64];
      snprintf(text, 64, "value=%d", value);
      return text;
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

int   test_divxy(int x, int y)
{
      return x / y;
}

int   test_modxy(int x, int y)
{
      return x % y;
}

int   main()
{
      int l_success = 
            (test_01() << 0) |
            (test_02() << 1) |
            (test_03() << 2);
      return l_success ^ 7;
}
