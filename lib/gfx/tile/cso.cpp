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
#include "cso.h"
#include <limits>
#include <cstring>

namespace gfx {

      cso::cso() noexcept:
      m_ptr()
{
}

      cso::cso(unsigned int format, int sx, int sy) noexcept:
      cso()
{
      reset(format, sx, sy);
}

      cso::cso(unsigned int format, int sx, int sy, std::uint8_t* data, std::size_t size) noexcept:
      cso()
{
      reset(format, sx, sy, data, size);
}

      cso::cso(const cso& copy) noexcept:
      m_ptr(copy.m_ptr)
{
}

      cso::cso(cso&& copy) noexcept:
      m_ptr(std::move(copy.m_ptr))
{
      copy.release();
}

      cso::~cso()
{
      dispose();
}

/* ref
*/
      cso::ref::ref() noexcept:
      m_format(1),
      m_sx(0),
      m_sy(0),
      m_data(nullptr),
      m_size(0),
      m_hooks(1)
{
}

      cso::ref::ref(unsigned int format, int sx, int sy) noexcept:
      ref()
{
      reset(format, sx, sy);
}

      cso::ref::ref(unsigned int format, int sx, int sy, std::uint8_t* data, std::size_t size) noexcept:
      ref()
{
      reset(format, sx, sy, data, size);
}

      cso::ref::~ref()
{
      reset();
}

unsigned int cso::ref::get_format() const noexcept
{
      return m_format;
}

int   cso::ref::get_glyph_sx() const noexcept
{
      return m_sx;
}

int   cso::ref::get_glyph_sy() const noexcept
{
      return m_sy;
}

std::uint8_t* cso::ref::get_data() const noexcept
{
      return m_data;
}

int   cso::ref::get_size() const noexcept
{
      return m_size;
}

void  cso::ref::reset() noexcept
{
      if(m_data) {
          gfx_cso_dispose(m_data, m_size);
          m_data = nullptr;
      }
}

bool  cso::ref::reset(unsigned int format, int sx, int sy) noexcept
{
      reset();
      if((sx < glyph_sx_min) || (sx > glyph_sx_max)) {
          return false;
      }
      if((sy < glyph_sy_min) || (sy > glyph_sy_max)) {
          return false;
      }
      if((format != fmt_1bpp) && (format != fmt_8bpp)) {
          return false;
      }
      m_size = get_data_size(format, sx, sy);
      m_data = gfx_cso_reserve(m_size);
      if(m_data) {
          m_format = format;
          m_sx = sx;
          m_sy = sy;
          return true;
      }
      return true;
}

bool  cso::ref::reset(unsigned int format, int sx, int sy, std::uint8_t* data, std::size_t size) noexcept
{
      reset();
      if((sx < glyph_sx_min) || (sx > glyph_sx_max)) {
          return false;
      }
      if((sy < glyph_sy_min) || (sy > glyph_sy_max)) {
          return false;
      }
      if((format != fmt_1bpp) && (format != fmt_8bpp)) {
          return false;
      }
      if(data) {
          if(size <= static_cast<std::size_t>(std::numeric_limits<int>::max())) {
              std::size_t l_size_min = get_data_size(format, sx, sy);
              if(size >= l_size_min) {
                  m_format = format;
                  m_sx = sx;
                  m_sy = sy;
                  m_data = data;
                  m_size = size;
                  return  true;
              }
          }
      }
      return false;
}

cso::ref*  cso::ref::hook() noexcept
{
      m_hooks++;
      return this;
}

cso::ref*  cso::ref::drop() noexcept
{
      if(m_hooks) {
          m_hooks--;
          if(m_hooks == 0) {
              delete this;
          }
      }
      return nullptr;
}

/* ptr
*/
      cso::ptr::ptr() noexcept:
      m_ref(nullptr)
{
}

      cso::ptr::ptr(ref* p) noexcept:
      m_ref(p)
{
}

      cso::ptr::ptr(const ptr& copy) noexcept:
      m_ref(copy.clone())
{
      
}

      cso::ptr::ptr(ptr&& copy) noexcept:
      m_ref(copy.release())
{
}

      cso::ptr::~ptr()
{
      reset();
}

cso::ref*  cso::ptr::get() const noexcept
{
      return m_ref;
}

cso::ref*  cso::ptr::clone() const noexcept
{
      if(m_ref) {
          return m_ref->hook();
      }
      return nullptr;
}

void  cso::ptr::reset() noexcept
{
      if(m_ref) {
          m_ref = m_ref->drop();
      }
}

cso::ref*  cso::ptr::release() noexcept
{
      ref* l_result = m_ref;
      m_ref = nullptr;
      return l_result;
}

cso::ref*  cso::ptr::operator->() const noexcept
{
      return m_ref;
}

      cso::ptr::operator ref*() const noexcept
{
      return m_ref;
}

cso::ptr&  cso::ptr::operator=(std::nullptr_t rhs) noexcept
{
      reset();
      return *this;
}

cso::ptr&  cso::ptr::operator=(ref* rhs) noexcept
{
      if(m_ref) {
          if(m_ref != rhs) {
              m_ref = m_ref->drop();
          }
      }
      m_ref = rhs;
      return *this;
}

cso::ptr&  cso::ptr::operator=(const ptr& rhs) noexcept
{
      if(rhs.m_ref != m_ref) {
          if(m_ref != nullptr) {
              m_ref->drop();
          }
          if(rhs.m_ref != nullptr) {
              m_ref = rhs.clone();
          } else
              m_ref = nullptr;
      }
      return *this;
}

cso::ptr&  cso::ptr::operator=(ptr&& rhs) noexcept
{
      if(rhs.m_ref != m_ref) {
          if(m_ref != nullptr) {
              m_ref->drop();
          }
          if(rhs.m_ref != nullptr) {
              m_ref = rhs.release();
          } else
              m_ref = nullptr;
      }
      return *this;
}

bool  cso::reset(unsigned int format, int sx, int sy) noexcept
{
      if((sx > 0) &&
          (sy > 0)) {
          return m_ptr = make_ptr(format, sx, sy);
      }
      else {
          dispose();
          return true;
      }
}

bool  cso::reset(unsigned int format, int sx, int sy, std::uint8_t* data, std::size_t size) noexcept
{
      if((sx > 0) &&
          (sy > 0) &&
          (data != nullptr) &&
          (size > 0)) {
          return m_ptr = make_ptr(format, sx, sy, data, size);
      }
      else {
          dispose();
          return true;
      }
}

void  cso::dispose() noexcept
{
      m_ptr.reset();
}

void  cso::release() noexcept
{
      m_ptr.release();
}

      cso::operator bool() const noexcept
{
      return m_ptr;
}

cso&  cso::operator=(const cso& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          m_ptr = rhs.m_ptr;
      }
      return *this;
}

cso&  cso::operator=(cso&& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          m_ptr = std::move(rhs.m_ptr);
      }
      return *this;
}

/*namespace gfx*/ }
