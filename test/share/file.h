#ifndef cor_file_h
#define cor_file_h
// file.h
// bindings for a few basic file i/o functions
#include <stdlib.h>
#include <memory>
#include <vector>

int   open(const char*, int) noexcept;
// FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);			/* Read data from the file */
// FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	/* Write data to the file */
// FRESULT f_lseek (FIL* fp, FSIZE_t ofs);								/* Move file pointer of the file object */
// #define f_eof(fp) ((int)((fp)->fptr == (fp)->obj.objsize))
// #define f_error(fp) ((fp)->err)
// #define f_tell(fp) ((fp)->fptr)
// #define f_size(fp) ((fp)->obj.objsize)
// #define f_rewind(fp) f_lseek((fp), 0)
int   close(int) noexcept;
#endif
