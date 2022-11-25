#ifndef cor_file_h
#define cor_file_h
// file.h
// bindings for a few basic file i/o functions
#include <stdlib.h>
#include <memory>
#include <vector>

#ifndef _STDIO_H
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

int       open(const char*, int) noexcept;
ssize_t   read(int, std::uint8_t*, std::size_t) noexcept;
ssize_t   write(int, const std::uint8_t*, std::size_t) noexcept;
off_t     seek(int, off_t, int) noexcept;
off_t     tell(int) noexcept;
bool      eof(int) noexcept;
int       close(int) noexcept;
#endif

