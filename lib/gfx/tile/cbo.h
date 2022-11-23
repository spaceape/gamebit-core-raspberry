#ifndef gfx_cbo_h
#define gfx_cbo_h
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
#include <gfx.h>
#include <gfx/tile.h>
#include <gfx/ac.h>

namespace gfx {

/* cbo
   character buffer object
*/
class cbo: public ac
{
  short int       m_sx;
  short int       m_sy;
  std::uint8_t    m_format;
  std::int32_t    m_size:24;
  std::uint8_t*   m_data;

  protected:
  static constexpr int size_max = (1 << 23) - global::cache_small_max;

  static constexpr int get_line_size(int sx) noexcept {
          return sx;
  }

  static constexpr int get_data_size(std::uint8_t fmt, int sx, int sy) noexcept {
          int l_sx = get_line_size(sx);
          int l_sy = sy;
          return l_sx * l_sy * (1 + ((fmt & fmt_tile) >> 2));
  }

  public:
          cbo() noexcept;
          cbo(std::uint8_t, int, int) noexcept;
          cbo(std::uint8_t, int, int, std::uint8_t*) noexcept;
          cbo(const cbo&) noexcept;
          cbo(cbo&&) noexcept;
          ~cbo();

  inline  bool has_format(std::uint8_t format) const noexcept {
          return (m_format & format) == format;
  }
  
  inline  std::uint8_t  get_format() const noexcept {
          return m_format;
  }

  inline  int    get_sx() const noexcept {
          return m_sx;
  }
  
  inline  int    get_sy() const noexcept {
          return m_sy;
  }

          bool  clone(const cbo&) noexcept;
          bool  reset(std::uint8_t, int, int) noexcept;
          void  dispose() noexcept;
          void  release() noexcept;
  
  inline  int   get_lb_offset() const noexcept {
          return 0;
  }
  
  inline  std::uint8_t* get_lb_ptr(int dx = 0, int dy = 0) const noexcept {
          return m_data + get_lb_offset() + dy * get_line_size() + dx;
  }
  
  inline  int   get_hb_offset() const noexcept {
          return get_lb_offset() + m_sy * get_line_size();
  }

  inline  std::uint8_t* get_hb_ptr(int dx = 0, int dy = 0) const noexcept {
          return m_data + get_hb_offset() + dy * get_line_size() + dx;
  }
  
  inline  int   get_xb0_offset() const noexcept {
          return get_hb_offset() + m_sy * get_line_size();
  }
  
  inline  std::uint8_t* get_xb0_ptr(int dx = 0, int dy = 0) const noexcept {
          return m_data + get_xb0_offset() + dy * get_line_size() + dx;
  }

  inline  int   get_xb1_offset() const noexcept {
          return get_xb0_offset() + m_sy * get_line_size();
  }
  
  inline  std::uint8_t* get_xb1_ptr(int dx = 0, int dy = 0) const noexcept {
          return m_data + get_xb1_offset() + dy * get_line_size() + dx;
  }

  inline  int   get_line_size() const noexcept {
          return get_line_size(m_sx);
  }
  
  inline  std::uint8_t* get_data_ptr() const noexcept {
          return m_data;
  }
  
  inline  int   get_data_size() const noexcept {
          return m_size;
  }
  
  inline  operator bool() const noexcept {
          return m_data;
  }

          cbo&  operator=(const cbo&) noexcept;
          cbo&  operator=(cbo&&) noexcept;
};

/*namespace gfx*/ }
#endif
