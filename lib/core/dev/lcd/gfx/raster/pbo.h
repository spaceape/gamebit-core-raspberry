#ifndef gfx_raster_pbo_h
#define gfx_raster_pbo_h
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
#include <dev/lcd/gfx/gfx.h>
#include <dev/lcd/gfx/dc.h>
#include <dev/lcd/gfx/tile.h>

namespace dev {
namespace gfx {

/* pbo
   pixel buffer object
*/
class pbo: public dc
{
  std::uint16_t   m_sx;
  std::uint16_t   m_sy;
  std::uint8_t    m_format;
  std::uint32_t   m_size:24;
  std::uint8_t*   m_data;

  friend class scene;
  public:
          pbo() noexcept;
          pbo(std::uint8_t, std::uint16_t, std::uint16_t) noexcept;
          pbo(std::uint8_t, std::uint16_t, std::uint16_t, std::uint8_t*) noexcept;
          pbo(const pbo&) noexcept;
          pbo(pbo&&) noexcept;
          ~pbo();

  inline  std::uint16_t get_sx() const noexcept {
          return m_sx;
  }
  
  inline  std::uint16_t get_sy() const noexcept {
          return m_sy;
  }

  inline  std::uint8_t* get_ptr() const noexcept {
          return m_data;
  }

  inline  std::uint8_t* get_ptr(int offset) const noexcept {
          return m_data + offset;
  }

  inline  std::uint8_t* get_line(int index) const noexcept {
          return m_data + index * get_data_stride();
  }

          bool  reset(std::uint8_t, std::uint16_t, std::uint16_t) noexcept;
          bool  reset(const pbo&) noexcept;
          void  dispose() noexcept;
          void  release() noexcept;
  
  inline  std::uint8_t* get_data_ptr() const noexcept {
          return reinterpret_cast<std::uint8_t*>(m_data);
  }
  
  inline  std::uint16_t get_data_stride() const noexcept {
          if(m_sx & 15) {
              return (m_sx & ~15) + 16;
          }
          return m_sx;
  }

  inline  std::uint8_t* operator[](int index) const noexcept {
          return get_line(index);
  }

  inline  operator bool() const noexcept {
          return m_data;
  }

          pbo&  operator=(const pbo& rhs) noexcept;
          pbo&  operator=(pbo&& rhs) noexcept;
};

/*namespace gfx*/ }
/*namespace dev*/ }
#endif
