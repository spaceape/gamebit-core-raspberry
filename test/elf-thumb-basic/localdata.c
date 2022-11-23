#include <stdint.h>

static char data[256];

void clear()
{
    char* i_data_0 = data;
    char* i_data_1 = i_data_0 + 256;
    while(i_data_0 < i_data_1) {
        *i_data_0 = 0;
        ++i_data_0;
    }
}
