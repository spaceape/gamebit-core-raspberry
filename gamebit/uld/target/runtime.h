#ifndef uld_elf_runtime_h
#define uld_elf_runtime_h
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
#include <uld.h>

namespace uld {

/* rsa_*
   runtime segment/symbol attributes
*/
constexpr std::uint8_t   rsa_region_none = 0;
constexpr std::uint8_t   rsa_region_rom  = 1;
constexpr std::uint8_t   rsa_region_data = 2;
constexpr std::uint8_t   rsa_region_code = 3;
constexpr std::uint8_t   rsa_region = 15;

constexpr std::uint8_t   rsa_mode_r = 16;
constexpr std::uint8_t   rsa_mode_w = 32;
constexpr std::uint8_t   rsa_mode_x = 64;
constexpr std::uint8_t   rsa_mode_a = 128;      // allocate
constexpr std::uint8_t   rsa_mode_rw = rsa_mode_r | rsa_mode_w;
constexpr std::uint8_t   rsa_mode_rwa = rsa_mode_rw | rsa_mode_a;
constexpr std::uint8_t   rsa_mode_rx = rsa_mode_r | rsa_mode_x;
constexpr std::uint8_t   rsa_mode_rxa = rsa_mode_rx | rsa_mode_a;
constexpr std::uint8_t   rsa_mode = rsa_mode_r | rsa_mode_w | rsa_mode_x;

/* rtl_address_t
   virtual/physical address pair
*/
struct rtl_address_t
{
  std::uint32_t base;
  std::uint8_t* data;
};

constexpr std::uint32_t  rsa_base_undef = 0xffffffff;  // address undefined, signals an rtl_address_t allocation error

/* rsa_page_size
   page size for segment allocation; segment sizes in bytes will be an integer multiple of this value;
   1K seems to be a decent value for Raspberry Pico
*/
constexpr int            rsa_page_size  = 1024;

/* rtl_segment_t
*/
struct rtl_segment_t
{
  std::uint8_t  attr;         // attribute flags (rsa_mode_* | rsa_region_*)
  std::uint16_t used;
  std::uint32_t size;         // size of the segment
  rtl_address_t address;
};

/* rtl_symbol_t
*/
struct rtl_symbol_t
{
  std::uint8_t  attributes;   // attribute flags
  std::uint32_t size:24;
  std::uint8_t* address;
  // char_ptr<56>  name;
};

/*namespace uld*/ }
#endif
