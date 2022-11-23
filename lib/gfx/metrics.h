#ifndef gfx_metrics_h
#define gfx_metrics_h
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
          box2d_t(const box2d_t&) noexcept;
          box2d_t(box2d_t&&) noexcept;
          ~box2d_t();

  inline  std::int16_t get_px0() const noexcept {
          return m_data.bit.px0;
  }

  inline  std::int16_t get_px() const noexcept {
          return m_data.bit.px0;
  }

  inline  std::int16_t get_py0() const noexcept {
          return m_data.bit.py0;
  }

  inline  std::int16_t get_py() const noexcept {
          return m_data.bit.py0;
  }

  inline  std::int16_t get_px1() const noexcept {
          return m_data.bit.px1;
  }

  inline  std::int16_t get_py1() const noexcept {
          return m_data.bit.py1;
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

          void     set_edges(std::int16_t, std::int16_t, std::int16_t, std::int16_t) noexcept;
          void     set_bounds(std::int16_t, std::int16_t, std::uint16_t, std::uint16_t) noexcept;
          bool     contains(std::int16_t, std::int16_t) const noexcept;
          bool     contains(const box2d_t&) const noexcept;
          void     intersect(const box2d_t&) noexcept;
          bool     intersects(const box2d_t&) const noexcept;
          bool     intersects(const box2d_t&, box2d_t&) const noexcept;
          box2d_t  intersected(const box2d_t&) const noexcept;
          void     assign(const box2d_t) noexcept;
          void     translate(std::int16_t, std::int16_t) noexcept;
          void     translate(const box2d_t&, std::int16_t, std::int16_t) noexcept;
          box2d_t  translated(std::int16_t, std::int16_t) const noexcept;
          void     compare(const box2d_t&, box2d_t&) const noexcept;
          std::uint16_t get_width() const noexcept;
          std::uint16_t get_height() const noexcept;

          void     reset() noexcept;
          void     release() noexcept;
          bool     is_empty() const noexcept;
          bool     is_valid() const noexcept;

  inline  box2d_t  operator&(const box2d_t& rhs) {
          return intersected(rhs);
  }

  inline  box2d_t&  operator&=(const box2d_t& rhs) {
          intersect(rhs);
          return *this;
  }

  inline  /*bool*/ operator bool() const {
          return is_valid();
  }
          
          bool     operator==(const box2d_t&) const noexcept;
          bool     operator!=(const box2d_t&) const noexcept;

          box2d_t& operator=(const box2d_t&) noexcept;
          box2d_t& operator=(box2d_t&&) noexcept;
};
/*namespace gfx*/ }
#endif
