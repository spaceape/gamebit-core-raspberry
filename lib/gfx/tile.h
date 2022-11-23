#ifndef gfx_tile_h
#define gfx_tile_h
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

namespace gfx {

/* tile format flags
 * [ - - - -  F F - - ]
 * 
 * LB:  [ C7 C6 C5 C4 C3 C2 C1 C0 ]
 * HB:  [ -- Z2 Z1 Z0 -- CA C9 C8 ]
 * XB0: [ B7 B6 B5 B4 B3 B2 B1 B0 ]
 * XB1: [ F7 F6 F5 F4 F3 F2 F1 F0 ]
 * 
 * Where:
 * CX - character index
 * BX - background colour index
 * FX - foreground colour index
 * ZX - fusion mode: 8x8 and 16x16 tile size only
 *      on specifically designed charsets, up to eight characters can be grouped together in order to form a multicolour tile; 
 *      each character in the group will encode one colour bit, with bit 0 having the lower index in the charset
*/
constexpr std::uint8_t mode_lb = 0u;
constexpr std::uint8_t fmt_mono_bcs = mode_lb;      // monochrome and basic charset

constexpr std::uint8_t mode_hb = 4u;
constexpr std::uint8_t fmt_mono_xcs = mode_hb;      // monochrome and extended charset

constexpr std::uint8_t mode_cb = 12u;                // 256 colour, extended charset
constexpr std::uint8_t fmt_colour_xcs = mode_cb;

constexpr std::uint8_t fmt_tile = 0b00001100;

/* tile size limits
*/
constexpr int glyph_sx_min = 8;
constexpr int glyph_sx_max = 32;
constexpr int glyph_sy_min = 8;
constexpr int glyph_sy_max = 32;

/* number of tile sets surfaces are allowed to own;
   the absolute maximum limit of this value is dictated by the number of 'C' bits in the tile format flags just above
   (currently 16)
*/
constexpr int charset_count = 6;

/*namespace gfx*/ }
#endif
