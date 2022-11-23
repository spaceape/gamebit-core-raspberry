#ifndef gfx_config_h
#define gfx_config_h
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

namespace gfx {

/* driver_slot_count
   how many drivers can a device link in at the same time 
*/
constexpr int     driver_slot_count = 4;

static_assert(driver_slot_count >= driver_slot_count_min, "a device needs to support at least driver_slot_count_min driver slots.");
static_assert(driver_slot_count <= driver_slot_count_max, "a device can support at most driver_slot_count_max driver slots.");

/* support_tile_graphics
*/
constexpr bool    support_tile_graphics = true;

/* support_tile_graphics
*/
constexpr bool    support_sprites = false;

/* support_raster_graphics
*/
constexpr bool    support_raster_graphics = false;

/* support_vector_graphics
*/
constexpr bool    support_vector_graphics = false;

/* charset_count_*
 * how many tile sets can the engine offer for surfaces to address;
 * limits are given by the number of charset bits in the tile format flags, while the effective number is a
 * customization point useful to keep the surface data nodes within sensible limits
*/
constexpr int     charset_count_min = 2;
constexpr int     charset_count_max = 32;

/* charset_size
 * how many glyphs in a tile set there are
*/
constexpr int     charset_size = 128;

/* palette_size_*
 * how many glyphs colours a palette can accept at least and at most
*/
constexpr int     palette_size_min = 2;
constexpr int     palette_size_max = 256;

/*namespace gfx*/ }
#endif
