#ifndef core_global_h
#define core_global_h
/** 
    Copyright (c) 2020, wicked systems
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
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <new>
#include "util.h"
#ifdef __EXCEPTIONS
#include <stdexcept>
#endif

#define __str_of(s) #s
#define __name_of(d) __str_of(d)

using schar = char;
using uchar = unsigned char;

namespace std {

using int8_t = ::int8_t;
using uint8_t = ::uint8_t;
using int16_t = ::int16_t;
using uint16_t = ::uint16_t;
using int32_t = ::int32_t;
using uint32_t = ::uint32_t;
using size_t = ::size_t;

/*namespace std*/}

namespace global {

constexpr std::size_t system_page_size = 1024u;

/* enable implicit dynamic alloc */
/* cache size to use */
#ifdef CACHE_SMALL_MAX
constexpr std::size_t cache_small_max = CACHE_SMALL_MAX;
#else
constexpr std::size_t cache_small_max = 16u;
#endif

static_assert(cache_small_max >= 8, "small cache constant must be greater than 0 and should be at least 8.");

#ifdef CACHE_LARGE_MAX
constexpr std::size_t cache_large_max = CACHE_LARGE_MAX;
#else
constexpr std::size_t cache_large_max = cache_small_max * 4u;
#endif

/*namespace global*/ }
#endif
