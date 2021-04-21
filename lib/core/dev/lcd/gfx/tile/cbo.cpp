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
#include "cbo.h"
#include <cstring>

namespace dev {
namespace gfx {

      cbo::cbo() noexcept:
      dc(),
      m_sx(0),
      m_sy(0),
      m_format(fmt_undef),
      m_size(0),
      m_data(nullptr)
{
}

      cbo::cbo(std::uint8_t format, std::uint16_t sx, std::uint16_t sy) noexcept:
      cbo()
{
      reset(format, sx, sy);
}

      cbo::cbo(std::uint8_t format, std::uint16_t sx, std::uint16_t sy, std::uint8_t*) noexcept:
      cbo()
{
      reset(format, sx, sy);
}


      cbo::cbo(const cbo& copy) noexcept:
      cbo()
{
      reset(copy);
}

      cbo::cbo(cbo&& copy) noexcept:
      cbo(copy)
{
      copy.release();
}

      cbo::~cbo()
{
      dispose();
}

bool  cbo::reset(std::uint8_t format, std::uint16_t sx, std::uint16_t sy) noexcept
{
      std::size_t l_size = compute_data_size(format, sx, sy);
      dispose();
      if(l_size) {
          if(m_data = reinterpret_cast<std::uint8_t*>(malloc(l_size)); m_data != nullptr) {
              m_sx     = sx;
              m_sy     = sy;
              m_format = format;
              m_size   = l_size;
              return true;
          }
      }
      return false;
}

bool  cbo::reset(const cbo& copy) noexcept
{
      if(reset(copy.m_format, copy.m_sx, copy.m_sy)) {
          std::memcpy(m_data, copy.m_data, copy.m_size);
          return true;
      }
      return false;
}

void  cbo::dispose() noexcept
{
      if(m_data) {
          free(m_data);
          release();
      }
}

void  cbo::release() noexcept
{
      m_format = 0;
      m_size = 0;
      m_data = nullptr;
}
  
cbo&  cbo::operator=(const cbo& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          reset(rhs);
      }
      return *this;
}

cbo&  cbo::operator=(cbo&& rhs) noexcept
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
