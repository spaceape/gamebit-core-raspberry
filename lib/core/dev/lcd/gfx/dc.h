#ifndef dev_gfx_dc_h
#define dev_gfx_dc_h
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

/* dc
 * device context
*/
class dc
{
  protected:
  static  constexpr  int  s_charset_sx   = 8;   // tile width
  static  constexpr  int  s_charset_sy   = 8;   // tile height
  static  constexpr  int  s_charmap_sx   = 32;  // character map memory representation horizontal size, in characters
  static  constexpr  int  s_charmap_sy   = 8;   // character map memory representation default vertical size, in characters
  static  constexpr  int  s_charmap_size = 512; // how many characters in the character map

  static  std::uint8_t    g_support;
  static  std::uint16_t   g_display_sx;
  static  std::uint16_t   g_display_sy;
  static  std::uint8_t*   g_charmap;            // where is the character map
  static  std::uint8_t*   g_palette;            // colour indexes
  static  lcd*            g_driver;             // generic pointer to the driver, mainly to avoid useless context switching

  public:
          dc() noexcept;
          dc(const dc&) noexcept;
          dc(dc&&) noexcept;
          ~dc();
          dc& operator=(const dc& rhs) noexcept;
          dc& operator=(dc&& rhs) noexcept;
};

/*namespace gfx*/ }
/*namespace dev*/ }
#endif
