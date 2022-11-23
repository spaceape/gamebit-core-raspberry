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
#include "metrics.h"

namespace gfx {

/* pos2d_t
*/
      pos2d_t::pos2d_t() noexcept:
      m_px(0),
      m_py(0)
{
}

      pos2d_t::pos2d_t(std::uint16_t px, std::uint16_t py) noexcept:
      m_px(px),
      m_py(py)
{
}

      pos2d_t::pos2d_t(const pos2d_t& copy) noexcept:
      m_px(copy.m_px),
      m_py(copy.m_py)
{
}

      pos2d_t::pos2d_t(pos2d_t&& copy) noexcept:
      m_px(copy.m_px),
      m_py(copy.m_py)
{
}

      pos2d_t::~pos2d_t()
{
}

pos2d_t& pos2d_t::operator=(const pos2d_t& rhs) noexcept
{
      m_px = rhs.m_px;
      m_py = rhs.m_py;
      return *this;
}

pos2d_t& pos2d_t::operator=(pos2d_t&& rhs) noexcept
{
      m_px = rhs.m_px;
      m_py = rhs.m_py;
      return *this;
}

/* size2d_t
*/
      size2d_t::size2d_t() noexcept:
      m_sx(0),
      m_sy(0)
{
}

      size2d_t::size2d_t(std::uint16_t sx, std::uint16_t sy) noexcept:
      m_sx(sx),
      m_sy(sy)
{
}

      size2d_t::size2d_t(const size2d_t& copy) noexcept:
      m_sx(copy.m_sx),
      m_sy(copy.m_sy)
{
}

      size2d_t::size2d_t(size2d_t&& copy) noexcept:
      size2d_t(copy)
{
}

      size2d_t::~size2d_t()
{
}

size2d_t& size2d_t::operator=(const size2d_t& rhs) noexcept
{
      m_sx = rhs.m_sx;
      m_sy = rhs.m_sy;
      return *this;
}

size2d_t& size2d_t::operator=(size2d_t&& rhs) noexcept
{
      m_sx = rhs.m_sx;
      m_sy = rhs.m_sy;
      return *this;
}

/* box2d_t
*/
      box2d_t::box2d_t() noexcept:
      m_data{.vec ={0, 0, 0, 0}}
{
}

      box2d_t::box2d_t(std::int16_t px0, std::int16_t py0, std::int16_t px1, std::int16_t py1) noexcept:
      m_data{.bit ={px0, py0, px1, py1}}
{
}

      box2d_t::box2d_t(const box2d_t& copy) noexcept:
      m_data{copy.m_data.value}
{
}

      box2d_t::box2d_t(box2d_t&& copy) noexcept:
      m_data{copy.m_data.value}
{
}

      box2d_t::~box2d_t()
{
}

void  box2d_t::set_edges(std::int16_t px0, std::int16_t py0, std::int16_t px1, std::int16_t py1) noexcept
{
      m_data.bit.px0 = px0;
      m_data.bit.py0 = py0;
      m_data.bit.px1 = px1;
      m_data.bit.py1 = py1;
}

void  box2d_t::set_bounds(std::int16_t px, std::int16_t py, std::uint16_t sx, std::uint16_t sy) noexcept
{
      m_data.bit.px0 = px;
      m_data.bit.py0 = py;
      m_data.bit.px1 = px + sx;
      m_data.bit.py1 = py + sy;
}

bool  box2d_t::contains(std::int16_t px, std::int16_t py) const noexcept
{
      if((px >= get_px0()) && (px <= get_px1())) {
          if((py >= get_py0()) && (py <= get_py1())) {
              return true;
          }
      }
      return false;
}

bool  box2d_t::contains(const box2d_t& box) const noexcept
{
      if((box.get_px0() >= get_px0()) && (box.get_px0() <= get_px1())) {
          if((box.get_py0() >= get_py0()) && (box.get_py0() <= get_py1())) {
              return true;
          }
      }
      return false;
}

void  box2d_t::intersect(const box2d_t& box) noexcept
{
      int l_px0;
      if(get_px0() > box.get_px0()) {
          l_px0 = get_px0();
      } else
          l_px0 = box.get_px0();

      int l_py0;
      if(get_py0() > box.get_py0()) {
          l_py0 = get_py0();
      } else
          l_py0 = box.get_py0();

      int l_px1;
      if(get_px1() < box.get_px1()) {
          l_px1 = get_px1();
      } else
          l_px1 = box.get_px1();

      int l_py1;
      if(get_py1() < box.get_py1()) {
          l_py1 = get_py1();
      } else
          l_py1 = box.get_py1();

      if(l_px0 < l_px1) {
          if(l_py0 < l_py1) {
              set_edges(l_px0, l_py0, l_px1, l_py1);
          }
      }
}

bool  box2d_t::intersects(const box2d_t& box) const noexcept
{
      if((box.get_px1() > get_px0()) && (box.get_py1() > get_px0())) {
          if((box.get_px0() < get_px1()) && (box.get_py0() < get_py1())) {
              return true;
          }
      }
      return false;
}

bool  box2d_t::intersects(const box2d_t& box, box2d_t& result) const noexcept
{
      int l_px0;
      if(get_px0() > box.get_px0()) {
          l_px0 = get_px0();
      } else
          l_px0 = box.get_px0();

      int l_py0;
      if(get_py0() > box.get_py0()) {
          l_py0 = get_py0();
      } else
          l_py0 = box.get_py0();

      int l_px1;
      if(get_px1() < box.get_px1()) {
          l_px1 = get_px1();
      } else
          l_px1 = box.get_px1();

      int l_py1;
      if(get_py1() < box.get_py1()) {
          l_py1 = get_py1();
      } else
          l_py1 = box.get_py1();

      if(l_px0 < l_px1) {
          if(l_py0 < l_py1) {
              result.set_edges(l_px0, l_py0, l_px1, l_py1);
              return true;
          }
      }
      return false;
}

box2d_t box2d_t::intersected(const box2d_t& rhs) const noexcept
{
      box2d_t l_result;
      intersects(rhs, l_result);
      return l_result;
}

void  box2d_t::assign(const box2d_t rhs) noexcept
{
      m_data.value = rhs.m_data.value;
}

void  box2d_t::translate(std::int16_t dx, std::int16_t dy) noexcept
{
      m_data.bit.px0 += dx;
      m_data.bit.py0 += dy;
      m_data.bit.px1 += dx;
      m_data.bit.py1 += dy;
}

void  box2d_t::translate(const box2d_t& src, std::int16_t dx, std::int16_t dy) noexcept
{
      m_data.bit.px0 = src.m_data.bit.px0 + dx;
      m_data.bit.py0 = src.m_data.bit.py0 + dy;
      m_data.bit.px1 = src.m_data.bit.px1 + dx;
      m_data.bit.py1 = src.m_data.bit.py1 + dy;
}

box2d_t box2d_t::translated(std::int16_t dx, std::int16_t dy) const noexcept
{
      return box2d_t(
          static_cast<std::int16_t>(m_data.bit.px0 + dx),
          static_cast<std::int16_t>(m_data.bit.py0 + dy),
          static_cast<std::int16_t>(m_data.bit.px1 + dx),
          static_cast<std::int16_t>(m_data.bit.py1 + dy)
      );
}

void  box2d_t::compare(const box2d_t& rhs, box2d_t& result) const noexcept
{
      result.m_data.bit.px0 = m_data.bit.px0 - rhs.m_data.bit.px0;
      result.m_data.bit.py0 = m_data.bit.py0 - rhs.m_data.bit.py0;
      result.m_data.bit.px1 = m_data.bit.px1 - rhs.m_data.bit.px1;
      result.m_data.bit.py1 = m_data.bit.py1 - rhs.m_data.bit.py1;
}

std::uint16_t box2d_t::get_width() const noexcept
{
      if(m_data.bit.px1 > m_data.bit.px0) {
          return m_data.bit.px1 - m_data.bit.px0;
      } else
          return m_data.bit.px0 - m_data.bit.px1;
}

std::uint16_t box2d_t::get_height() const noexcept
{
      if(m_data.bit.py1 > m_data.bit.py0) {
          return m_data.bit.py1 - m_data.bit.py0;
      } else
          return m_data.bit.py0 - m_data.bit.py1;
}

void  box2d_t::reset() noexcept
{
      m_data.value = 0;
}

void  box2d_t::release() noexcept
{
      m_data.value = 0;
}

bool  box2d_t::is_empty() const noexcept
{
      return is_valid() == false;
}

bool  box2d_t::is_valid() const noexcept
{
      return (get_sx() > 0) && (get_sy() > 0);
}

bool  box2d_t::operator==(const box2d_t& rhs) const noexcept
{
      return m_data.value == rhs.m_data.value;
}

bool  box2d_t::operator!=(const box2d_t& rhs) const noexcept
{
      return m_data.value != rhs.m_data.value;
}

box2d_t& box2d_t::operator=(const box2d_t& rhs) noexcept
{
      m_data.value = rhs.m_data.value;
      return *this;
}

box2d_t& box2d_t::operator=(box2d_t&& rhs) noexcept
{
      m_data.value = rhs.m_data.value;
      return *this;
}

/*namespace gfx*/ }
