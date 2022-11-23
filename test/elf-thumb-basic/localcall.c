#include <stdint.h>

char data[256];

void clear()
{
    char* i_char = &(data[0]);
    char* i_last = i_char + 256;
    while(i_char != i_last) {
        *i_char = 0;
        ++i_char;
    }
}

void fill()
{
    char* i_char = &(data[0]);
    char* i_last = i_char + 256;
    while(i_char != i_last) {
        *i_char ^= 255;
        ++i_char;
    }
}

void run()
{
    clear();
    fill();
}
