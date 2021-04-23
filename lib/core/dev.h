#ifndef core_dev_h
#define core_dev_h
/** 
    Copyright (c) 2021, wicked systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
    conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
      disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
      disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of wicked systems nor the names of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
#include "global.h"

/* Raspberry Pico hardware structs
*/
struct spi_inst;

/* file open flags (fcntl.h approved)
*/
#ifndef _FCNTL_H
constexpr long int O_RDONLY = 0;
constexpr long int O_WRONLY = 1;
constexpr long int O_RDWR = 2;
constexpr long int O_ACCMODE = (O_RDONLY | O_WRONLY | O_RDWR);

constexpr long int O_CREAT = 0x0100;
constexpr long int O_TRUNC = 0x0200;
constexpr long int O_EXCL = 0x0400;

constexpr long int O_TEXT = 0x4000;
constexpr long int O_BINARY = 0x8000;
constexpr long int O_RAW = O_BINARY;
#endif

// #ifndef _LINUX_FS_H
// constexpr int SEEK_SET = 0; /* seek relative to beginning of file */
// constexpr int SEEK_CUR = 1; /* seek relative to current file position */
// constexpr int SEEK_END = 2; /* seek relative to end of file */
// #endif

namespace dev {

using  spi_inst_t = spi_inst;

class  bd;
class  drive;
class  lcd;

// SD Card config
constexpr unsigned int spi_sdc_baud_auto = 1000000;

// LCD config
constexpr unsigned int spi_lcd_baud_auto = 1000000;

/*namespace dev*/ }
#endif
