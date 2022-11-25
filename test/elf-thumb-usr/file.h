#ifndef usr_file_h
#define usr_file_h
#include "sys.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define FOPEN_MAX 128

int     open(const char*, int) __attribute__((long_call));
ssize_t read(int, uint8_t*, size_t) __attribute__((long_call));
ssize_t write(int, const uint8_t*, size_t) __attribute__((long_call));
off_t   seek(int, off_t, int) __attribute__((long_call));
off_t   tell(int) __attribute__((long_call));
bool    eof(int) __attribute__((long_call));
int     close(int) __attribute__((long_call));

#endif
