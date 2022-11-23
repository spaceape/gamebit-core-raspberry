#ifndef core_sys_h
#define core_sys_h
/** 
    Copyright (c) 2022, wicked systems
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
#include <mmi.h>
#include "sys/adt.h"
#include "sys/ios.h"

constexpr char EOL ='\n';
constexpr char NUL = 0;
constexpr char EOS = NUL;

#ifndef EOF
constexpr char EOF = -1;
#endif

constexpr char RET ='\r';
constexpr char TAB ='\t';
constexpr char SPC =' ';
constexpr char ASCII_MAX = 127;

/* O_*
   file open flags for embedded (fcntl.h approved)
*/
#ifndef _FCNTL_H
constexpr long int O_RDONLY = 0;
constexpr long int O_WRONLY = 1;
constexpr long int O_RDWR = 2;
constexpr long int O_ACCMODE = (O_RDONLY | O_WRONLY | O_RDWR);

constexpr long int O_CREAT = 64;
constexpr long int O_EXCL = 128;
constexpr long int O_TRUNC = 512;
constexpr long int O_APPEND = 1024;
constexpr long int O_NONBLOCK = 2048;

constexpr long int B_TEXT = 0x1000;
constexpr long int O_BINARY = 0x2000;
constexpr long int O_RAW = O_BINARY;
#endif

constexpr long int  M_ACQUIRE = 0x00010000; // take ownership of file descriptor (for sys::fio)

/* file seek flags for embedded
*/
#ifndef SEEK_SET
constexpr int SEEK_SET = 0;
#endif
#ifndef SEEK_CUR
constexpr int SEEK_CUR = 1;
#endif
#ifndef SEEK_END
constexpr int SEEK_END = 2;
#endif 

constexpr unsigned int pin_none = 256;

/* g_adt
   root of the Abstract Device Tree
*/
extern sys::directory* g_adt;
#endif
