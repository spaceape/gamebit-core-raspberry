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
#include <util.h>
#include <cstring>

namespace gfx {
  
      cbo::cbo() noexcept:
      m_sx(0),
      m_sy(0),
      m_format(fmt_undef),
      m_size(0),
      m_data(nullptr)
{
}

      cbo::cbo(std::uint8_t format, int sx, int sy) noexcept:
      cbo()
{
      reset(format, sx, sy);
}

      cbo::cbo(std::uint8_t format, int sx, int sy, std::uint8_t*) noexcept:
      cbo()
{
      reset(format, sx, sy);
}


      cbo::cbo(const cbo& copy) noexcept:
      cbo()
{
      clone(copy);
}

      cbo::cbo(cbo&& copy) noexcept:
      cbo()
{
      m_sx = copy.m_sx;
      m_sy = copy.m_sy;
      m_format = copy.m_format;
      m_size = copy.m_size;
      m_data = copy.m_data;
      copy.release();
}

      cbo::~cbo()
{
      dispose();
}

bool  cbo::clone(const cbo& copy) noexcept
{
      dispose();
      if(std::addressof(copy) != this) {
          if(reset(copy.m_format, copy.m_sx, copy.m_sy)) {
              std::memcpy(m_data, copy.m_data, copy.m_size);
          }
          return true;
      }
      return false;
}

bool  cbo::reset(std::uint8_t format, int sx, int sy) noexcept
{
      if((sx < std::numeric_limits<short int>::max()) &&
          (sy < std::numeric_limits<short int>::max())) {
          int  l_data_size    = get_data_size(format, sx, sy);
          int  l_reserve_size = get_round_value(l_data_size, global::cache_small_max);
          dispose();
          if((l_reserve_size > 0) &&
              (l_reserve_size <= size_max)) {
              if(m_data = (*gfx::ac::gfx_cbo_reserve)(l_reserve_size); m_data != nullptr) {
                  m_sx     = sx;
                  m_sy     = sy;
                  m_format = format;
                  m_size   = l_reserve_size;
                  return true;
              }
          }
      }
      return false;
}

void  cbo::dispose() noexcept
{
      if(m_data) {
          gfx_cbo_dispose(m_data, m_size);
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
          clone(rhs);
      }
      return *this;
}

cbo&  cbo::operator=(cbo&& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          dispose();
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
