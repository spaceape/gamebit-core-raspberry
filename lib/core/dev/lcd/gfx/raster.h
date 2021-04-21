#ifndef dev_gfx_raster_h
#define dev_gfx_raster_h
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
#include "gfx.h"

namespace dev {
namespace gfx {

/* raster format flags
*/
constexpr std::uint8_t fmt_undef = 0;
constexpr std::uint8_t fmt_argb = 16;
constexpr std::uint8_t mode_argb_1111 = fmt_argb | 0 | 1;
constexpr std::uint8_t mode_argb_2222 = fmt_argb | 0 | 0;
constexpr std::uint8_t mode_argb_4444 = fmt_argb | 2 | 1;
constexpr std::uint8_t mode_argb_8888 = fmt_argb | 2 | 0;

constexpr std::uint8_t fmt_indexed = 32;
constexpr std::uint8_t mode_x8 = fmt_indexed;

/* 8 bit ARGB
*/
union pixel8_t
{
  std::uint8_t value;

  struct {
    std::uint8_t b:2;
    std::uint8_t g:2;
    std::uint8_t r:2;
    std::uint8_t a:2;
  } argb;
};

/* 16 bit ARGB
*/
union pixel16_t
{
  std::uint16_t value;

  struct {
    std::uint8_t b:4;
    std::uint8_t g:4;
    std::uint8_t r:4;
    std::uint8_t a:4;
  } argb;
};

/* 32 bit ARGB
*/
union pixel32_t
{
  std::uint32_t value;

  struct {
    std::uint8_t b;
    std::uint8_t g;
    std::uint8_t r;
    std::uint8_t a;
  } argb;
};

union pixel_t
{
  pixel8_t  px8[4];
  pixel16_t px16[2];
  pixel32_t x32;
};

/*namespace gfx*/ }
/*namespace dev*/ }
#endif
