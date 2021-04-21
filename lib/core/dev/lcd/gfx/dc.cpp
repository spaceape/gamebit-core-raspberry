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
#include "dc.h"

namespace dev {
namespace gfx {

      std::uint8_t   dc::g_support;
      std::uint16_t  dc::g_display_sx;
      std::uint16_t  dc::g_display_sy;
      std::uint8_t*  dc::g_charmap;
      std::uint8_t*  dc::g_palette;
      lcd*           dc::g_driver;
 
      dc::dc() noexcept
{
}

      dc::dc(const dc&) noexcept
{
}

      dc::dc(dc&&) noexcept
{
}

      dc::~dc()
{
}

// bool  dc::gdc_cbo_reserve(std::uint8_t*& cbo, std::size_t size) noexcept
// {
//       return s_driver->gfx_cbo_reserve(cbo, size);
// }

// bool  dc::gdc_cbo_dispose(std::uint8_t*& cbo, std::size_t size) noexcept
// {
//       return s_driver->gfx_cbo_dispose(cbo, size);
// }

// bool  dc::gdc_pbo_reserve(std::uint8_t*& pbo, std::size_t size) noexcept
// {
//       return s_driver->gfx_pbo_reserve(pbo, size);
// }

// bool  dc::gdc_pbo_dispose(std::uint8_t*& pbo, std::size_t size) noexcept
// {
//       return s_driver->gfx_pbo_dispose(pbo, size);
// }

dc&   dc::operator=(const dc&) noexcept
{
      return *this;
}

dc&   dc::operator=(dc&&) noexcept
{
      return *this;
}

/*namespace gfx*/ }
/*namespace dev*/ }

