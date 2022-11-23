#ifndef usr_file_h
#define usr_file_h
#include "sys.h"

#define FOPEN_MAX 128

int   open(const char*, int) __attribute__((long_call));
int   close(int) __attribute__((long_call));

#endif
