#ifndef gamebit_h
#define gamebit_h
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
#include <global.h>
#include <dev.h>
#include <sys.h>
#include <sys/ios/fio.h>

static dev::bd*     sd0;
static dev::bd*     sd1;
static dev::drive*  part0;
static dev::drive*  part1;
static dev::drive*  part2;
static dev::drive*  cache;

static void*        display;
static void*        console;

/* if_*
   initialisation flags
*/
static constexpr unsigned int if_stdio = 1;
static constexpr unsigned int if_filesystem = 16;
static constexpr unsigned int if_all = 0xffffffff;

namespace gamebit {

bool  initialise(unsigned int = if_all) noexcept;
sys::fio     open(const char*, long int = O_RDONLY, long int = 0777) noexcept;
sys::fio     close(sys::fio&) noexcept;
dev::drive*  get_default_disk() noexcept;
dev::drive*  get_default_cache() noexcept;
bool  dispose(unsigned int = if_all) noexcept;

/*namespace gamebit*/ }
#endif
