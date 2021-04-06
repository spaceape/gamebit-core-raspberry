#ifndef core_util_h
#define core_util_h
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
#include <utility>

constexpr std::uint16_t get_uint16_le(std::uint8_t* p) noexcept {
      return p[0] | (p[1] << 8);
}

constexpr std::uint16_t get_uint16_be(std::uint8_t* p) noexcept {
      return p[1] | (p[0] << 8);
}

constexpr std::uint32_t get_uint32_le(std::uint8_t* p) noexcept {
      return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

constexpr std::uint32_t get_uint32_be(std::uint8_t* p) noexcept {
      return p[3] | (p[2] << 8) | (p[1] << 16) | (p[0] << 24);
}

/* get_divided_value()
*/
constexpr unsigned long int get_quotient_value(unsigned long int value, std::size_t divider) noexcept {
      std::size_t ret = value / divider;
      if(value % divider) {
          ret += 1;
      }
      return ret;
}

/* get_round_value()
*/
constexpr unsigned long int get_round_value(unsigned long int value, std::size_t granularity) noexcept {
      std::size_t rem = value % granularity;
      if(rem) {
          value -= rem;
          value += granularity;
      }
      return value;
}
#endif
