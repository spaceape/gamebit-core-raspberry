#include <sys.h>

int   test_divxy(int x, int y)
{
      return x / y;
}

int   test_modxy(int x, int y)
{
      return x % y;
}

char* test_02()
{
      static char text[64];
      text[0] = 'a';
      text[1] = 'b';
      text[2] = 'c';
      text[3] = 0;
      return text;
}
