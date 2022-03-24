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
#include "osc.h"

static constexpr std::uint16_t osc_rate_min  = 4096u;        // minimum sample rate
static constexpr std::uint16_t osc_rate_max  = 16384u;       // maximum sample rate

static constexpr float  osc_sync_dt    = (float)dev::osc_sync_us / 1000000.0f;
static constexpr float  osc_sync_rate  = 1000000.0f / dev::osc_sync_us;

static_assert(osc_sync_rate >= osc_rate_min, "Sampling rate needs to be at least osc_rate_min");
static_assert(osc_sync_rate <= osc_rate_max, "Sampling rate needs to be at most osc_rate_max");


struct ufpair_t
{
  unsigned int  ivalue;
  unsigned int  fvalue;
};

// NOTE: No point dicking around with fractional arithmetic, don't think I can get div
// faster than 50 CPU cycles

inline ufpair_t ufdiv(std::uint32_t lhs, std::uint32_t rhs) noexcept {
      ufpair_t  l_result;
      l_result.ivalue = 0;
      l_result.fvalue = 0;
      return l_result;
}
