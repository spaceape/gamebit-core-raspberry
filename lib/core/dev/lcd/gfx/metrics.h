#ifndef dev_gfx_metrics_h
#define dev_gfx_metrics_h
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

/* pos2d_t
   position or point in a 2D plane
*/
class pos2d_t
{
  std::int16_t  m_px;
  std::int16_t  m_py;

  public:
          pos2d_t() noexcept;
          pos2d_t(std::uint16_t, std::uint16_t) noexcept;
          pos2d_t(const pos2d_t&) noexcept;
          pos2d_t(pos2d_t&&) noexcept;
          ~pos2d_t();
          pos2d_t& operator=(const pos2d_t&) noexcept;
          pos2d_t& operator=(pos2d_t&&) noexcept;
};

/* size2d_t
   size in a 2D plane
*/
class size2d_t
{
  std::uint16_t m_sx;
  std::uint16_t m_sy;

  public:
          size2d_t() noexcept;
          size2d_t(std::uint16_t, std::uint16_t) noexcept;
          size2d_t(const size2d_t&) noexcept;
          size2d_t(size2d_t&&) noexcept;
          ~size2d_t();
          size2d_t& operator=(const size2d_t&) noexcept;
          size2d_t& operator=(size2d_t&&) noexcept;
};

/* box2d_t
   box in a 2D plane, encoded as:
   [ x0 y0 ]
   [ x1 y1 ]
   or
   [ x0 y0 x1 y1 ]
*/
class box2d_t
{
  union {
    std::uint64_t  value;
    std::int16_t   vec[4];
    struct {
      std::int16_t px0;
      std::int16_t py0;
      std::int16_t px1;
      std::int16_t py1;
    } bit;
  } m_data;

  public:
          box2d_t() noexcept;
          box2d_t(std::int16_t, std::int16_t, std::int16_t, std::int16_t) noexcept;
          box2d_t(std::int16_t, std::int16_t, std::uint16_t, std::uint16_t) noexcept;
          box2d_t(const box2d_t&) noexcept;
          box2d_t(box2d_t&&) noexcept;
          ~box2d_t();

  inline  std::int16_t get_px() const noexcept {
          return m_data.bit.px0;
  }

  inline  std::int16_t get_px0() const noexcept {
          return m_data.bit.px0;
  }

  inline  void set_px0(int value) noexcept {
          m_data.bit.px0 = value;
  }

  inline  std::int16_t get_py() const noexcept {
          return m_data.bit.py0;
  }

  inline  std::int16_t get_py0() const noexcept {
          return m_data.bit.py0;
  }

  inline  void set_py0(int value) noexcept {
          m_data.bit.py0 = value;
  }

  inline  std::int16_t get_px1() const noexcept {
          return m_data.bit.px1;
  }

  inline  void set_px1(int value) noexcept {
          m_data.bit.px1 = value;
  }

  inline  std::int16_t get_py1() const noexcept {
          return m_data.bit.py1;
  }

  inline  void set_py1(int value) noexcept {
          m_data.bit.py1 = value;
  }

  inline  std::int16_t get_cx() const noexcept {
          return (m_data.bit.px0 + m_data.bit.px1) / 2;
  }

  inline  std::uint16_t get_sx() const noexcept {
          return m_data.bit.px1 - m_data.bit.px0;
  }

  inline  std::int16_t get_cy() const noexcept {
          return (m_data.bit.py0 + m_data.bit.py1) / 2;
  }

  inline  std::uint16_t get_sy() const noexcept {
          return m_data.bit.py1 - m_data.bit.py0;
  }

          void     assign(std::int16_t, std::int16_t, std::int16_t, std::int16_t) noexcept;
          void     assign(std::int16_t, std::int16_t, std::uint16_t, std::uint16_t) noexcept;
          bool     contains(std::int16_t, std::int16_t) const noexcept;
          bool     contains(const box2d_t&) const noexcept;
          bool     intersects(const box2d_t&) const noexcept;
          bool     intersects(const box2d_t&, box2d_t&) const noexcept;
          void     translate(std::int16_t, std::int16_t) noexcept;
          box2d_t  translated(std::int16_t, std::int16_t) const noexcept;
          void     translate(std::int16_t, std::int16_t, const box2d_t&) noexcept;
          void     compare(const box2d_t&, box2d_t&) const noexcept;
          std::uint16_t get_width() const noexcept;
          std::uint16_t get_height() const noexcept;
          void     release() noexcept;
          bool     is_empty() const noexcept;
          bool     is_valid() const noexcept;
          bool     operator==(const box2d_t&) const noexcept;
          bool     operator!=(const box2d_t&) const noexcept;
          box2d_t& operator=(const box2d_t&) noexcept;
          box2d_t& operator=(box2d_t&&) noexcept;
};

/* coord_t
   layout coordinate
*/
class coord_t
{
  std::int16_t m_value;

  public:
          coord_t() noexcept;
          coord_t(std::int16_t) noexcept;
          coord_t(const coord_t&) noexcept;
          coord_t(coord_t&&) noexcept;
          ~coord_t();

  inline  std::int16_t get() const noexcept {
          return m_value;
  }

  inline  void set(std::int16_t value) noexcept {
          m_value = value;
  }

  inline  operator std::int16_t() const noexcept {
          return m_value;
  }

          coord_t& operator=(const coord_t&) noexcept;
          coord_t& operator=(coord_t&&) noexcept;
};

/* anchor_t
   layout anchor
*/
class anchor_t
{
  coord_t* m_coord;
  coord_t  m_local;

  public:
          anchor_t() noexcept;
          anchor_t(std::int16_t) noexcept;
          anchor_t(const anchor_t&) noexcept;
          anchor_t(anchor_t&&) noexcept;
          ~anchor_t();

  inline  std::int16_t get() const noexcept {
          return m_coord->get();
  }

  inline  std::int16_t get_local() const noexcept {
          return m_local;
  }

  inline  void   set_local(std::int16_t value) noexcept {
          return m_local.set(value);
  }

          void   reset() noexcept;

  inline  std::int16_t operator+(std::int16_t rhs) noexcept {
          return get() + rhs;
  }

  inline  std::int16_t operator-(std::int16_t rhs) noexcept {
          return get() - rhs;
  }
  
  inline  operator std::int16_t() const noexcept {
          return get();
  }
  
          anchor_t& operator=(std::int16_t) noexcept;
          anchor_t& operator=(const anchor_t&) noexcept;
          anchor_t& operator=(anchor_t&&) noexcept;
};

class strut_t
{
  std::int16_t m_weight;
  std::int16_t m_size;
  anchor_t*    m_anchor_prev;
  anchor_t*    m_anchor_next;
  strut_t*     m_strut_next;

  public:
          strut_t() noexcept;
          strut_t(const strut_t&) noexcept;
          strut_t(strut_t&) noexcept;
          ~strut_t() noexcept;
          strut_t& operator=(const strut_t&) noexcept;
          strut_t& operator=(strut_t&&) noexcept;
};
/*namespace gfx*/ }
/*namespace dev*/ }
#endif
