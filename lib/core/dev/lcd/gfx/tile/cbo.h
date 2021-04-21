#ifndef gfx_tile_cbo_h
#define gfx_tile_cbo_h
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

/* cbo
   character buffer object
*/
class cbo: public dc
{
  std::uint16_t   m_sx;
  std::uint16_t   m_sy;

  protected:
  std::uint8_t    m_format;
  std::uint32_t   m_size:24;
  std::uint8_t*   m_data;

  protected:
  static constexpr std::size_t compute_line_size(std::uint16_t sx) noexcept {
          if(sx & 1) {
              return (sx & ~1) + 2;
          }
          return sx;
  }

  static constexpr std::size_t compute_data_size(std::uint8_t fmt, std::uint16_t sx, std::uint16_t sy) noexcept {
          std::uint16_t l_sx = compute_line_size(sx);
          std::uint16_t l_sy = sy;
          if(fmt & mode_bg) {
              return l_sx * l_sy * 4;
          } else
          if(fmt & mode_fg) {
              return l_sx * l_sy * 4;
          } else
          if(fmt & mode_hb) {
              return l_sx * l_sy * 2;
          } else
              return l_sx * l_sy;
  }

  friend class scene;
  public:
          cbo() noexcept;
          cbo(std::uint8_t, std::uint16_t, std::uint16_t) noexcept;
          cbo(std::uint8_t, std::uint16_t, std::uint16_t, std::uint8_t*) noexcept;
          cbo(const cbo&) noexcept;
          cbo(cbo&&) noexcept;
          ~cbo();

  inline  bool   has_format(std::uint8_t value) const noexcept {
          return m_format & value;
  }

  inline  std::uint8_t  get_format() const noexcept {
          return m_format;
  }

  inline  std::uint16_t get_sx() const noexcept {
          return m_sx;
  }
  
  inline  std::uint16_t get_sy() const noexcept {
          return m_sy;
  }

          bool  reset(std::uint8_t, std::uint16_t, std::uint16_t) noexcept;
          bool  reset(const cbo&) noexcept;
          void  dispose() noexcept;
          void  release() noexcept;
  
  inline  std::size_t get_lb_offset() const noexcept {
          return 0;
  }
  
  inline  std::uint8_t* get_lb_ptr(int offset = 0) const noexcept {
          return m_data + offset;
  }

  inline  std::size_t get_hb_offset() const noexcept {
          return m_sy * get_line_size();
  }

  inline  std::uint8_t* get_hb_ptr(int offset = 0) const noexcept {
          return m_data + get_hb_offset() + offset;
  }

  inline  std::size_t get_fx_offset() const noexcept {
          return get_hb_offset() + m_sy * get_line_size();
  }

  inline  std::uint8_t* get_fx_ptr(int offset = 0) const noexcept {
          return m_data + get_fx_offset() * 2;
  }

  inline  std::uint8_t* get_line_ptr(int index) const noexcept {
          return m_data + index * get_line_size();
  }

  inline  std::size_t   get_line_size() const noexcept {
          return compute_line_size(m_sx);
  }
  
  inline  std::uint8_t* get_data_ptr() const noexcept {
          return m_data;
  }
  
  inline  std::size_t   get_data_size() const noexcept {
          return m_size;
  }
  
  inline  operator bool() const noexcept {
          return m_data;
  }

          cbo&  operator=(const cbo& rhs) noexcept;
          cbo&  operator=(cbo&& rhs) noexcept;
};

/*namespace gfx*/ }
/*namespace dev*/ }
#endif
