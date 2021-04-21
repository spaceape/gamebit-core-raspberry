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
#include "pbo.h"
#include <cstring>

namespace dev {
namespace gfx {

      pbo::pbo() noexcept:
      dc(),
      m_sx(0),
      m_sy(0),
      m_format(fmt_undef),
      m_size(0),
      m_data(nullptr)
{
}

      pbo::pbo(std::uint8_t format, std::uint16_t sx, std::uint16_t sy) noexcept:
      pbo()
{
      reset(format, sx, sy);
}

      pbo::pbo(std::uint8_t format, std::uint16_t sx, std::uint16_t sy, std::uint8_t*) noexcept:
      pbo()
{
      reset(format, sx, sy);
}


      pbo::pbo(const pbo& copy) noexcept:
      pbo()
{
      reset(copy);
}

      pbo::pbo(pbo&& copy) noexcept:
      pbo(copy)
{
      copy.release();
}

      pbo::~pbo()
{
      dispose();
}

bool  pbo::reset(std::uint8_t format, std::uint16_t sx, std::uint16_t sy) noexcept
{
      // return sdc_make_pbo(*this, format, sx, sy);
      return false;
}

bool  pbo::reset(const pbo& copy) noexcept
{
      if(reset(copy.m_format, copy.m_sx, copy.m_sy)) {
          std::memcpy(m_data, copy.m_data, copy.m_size);
          return true;
      }
      return false;
}

void  pbo::dispose() noexcept
{
      // sdc_free_pbo(*this, m_format, m_sx, m_sy);
}

void  pbo::release() noexcept
{
      m_size = 0;
      m_data = nullptr;
}
  
pbo&  pbo::operator=(const pbo& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          reset(rhs);
      }
      return *this;
}

pbo&  pbo::operator=(pbo&& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          m_sx = rhs.m_sx;
          m_sy = rhs.m_sy;
          m_format = rhs.m_format;
          m_size = rhs.m_size;
          m_data = rhs.m_data;
          rhs.release();
      }
      return *this;
}

/*namespace gfx*/ }
/*namespace dev*/ }
