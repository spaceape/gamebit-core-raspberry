#ifndef dev_gfx_tile_h
#define dev_gfx_tile_h
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
#include "raster.h"

namespace dev {
namespace gfx {

/* tile format flags
*/
constexpr std::uint8_t mode_lb = 1;
constexpr std::uint8_t fmt_mono_bcs = mode_lb;            // monochrome and basic charset

constexpr std::uint8_t mode_hb = 2;
constexpr std::uint8_t fmt_mono_xcs = mode_lb | mode_hb;  // monochrome and extended charset

// high byte flags for mode_hb
constexpr std::uint8_t hbi_char_bits = 0b0000'1111;       // the 4 MSBs of the character index
constexpr std::uint8_t hbi_fuse_bits = 0b0011'0000;       // fuse up to 8 characters to form a multicolour char

constexpr std::uint8_t mode_fg = 4;
constexpr std::uint8_t mode_bg = 8;
constexpr std::uint8_t fmt_argb_xcs = fmt_argb | mode_fg | mode_bg;
constexpr std::uint8_t fmt_indexed_xcs = fmt_indexed | mode_fg | mode_bg;

/*namespace gfx*/ }
/*namespace dev*/ }
#endif
