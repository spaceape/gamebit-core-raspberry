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
#include "pico/stdlib.h"
#include <cstdint>
#include <memory>

#define __str_of(s) #s
#define __name_of(d) __str_of(d)

namespace global {

/* page size: allocation size that memory:: containers use
*/
constexpr int system_page_size = 512u;

/* default cache sizes
*/
#ifdef CACHE_SMALL_MAX
constexpr int cache_small_max = CACHE_SMALL_MAX;
#else
constexpr int cache_small_max = 16u;
#endif

static_assert(cache_small_max >= 8, "small cache constant must be greater than 0 and should be at least 8.");

#ifdef CACHE_LARGE_MAX
constexpr int cache_large_max = CACHE_LARGE_MAX;
#else
constexpr int cache_large_max = cache_small_max * 4u;
#endif


constexpr int msps = 1000;
constexpr int usps = 1000 * msps;
constexpr int nsps = 1000 * usps;

/*namespace global*/ }
#endif
