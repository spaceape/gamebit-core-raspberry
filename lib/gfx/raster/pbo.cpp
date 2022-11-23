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
#include <limits>
#include <functional>
#include <cstring>

namespace gfx {

      pbo::pbo() noexcept:
      m_ptr()
{
}

      pbo::pbo(int format, int sx, int sy) noexcept:
      pbo()
{
      reset(format, sx, sy);
}

      pbo::pbo(int format, int sx, int sy, std::uint8_t* data, std::size_t size) noexcept:
      pbo()
{
      reset(format, sx, sy, data, size);
}

      pbo::pbo(const pbo& copy) noexcept:
      m_ptr(copy.m_ptr)
{
}

      pbo::pbo(pbo&& copy) noexcept:
      m_ptr(std::move(copy.m_ptr))
{
      copy.release();
}

      pbo::~pbo()
{
      dispose();
}

/* ref
*/
      pbo::ref::ref() noexcept:
      m_format(1),
      m_sx(0),
      m_sy(0),
      m_data(nullptr),
      m_size(0),
      m_hooks(1)
{
}

      pbo::ref::ref(int format, int sx, int sy) noexcept:
      ref()
{
      reset(format, sx, sy);
}

      pbo::ref::ref(int format, int sx, int sy, std::uint8_t* data, std::size_t size) noexcept:
      ref()
{
      reset(format, sx, sy, data, size);
}

      pbo::ref::~ref()
{
      reset();
}

std::uint8_t* pbo::ref::get_data() const noexcept
{
      return m_data;
}

int   pbo::ref::get_size() const noexcept
{
      return m_size;
}

void  pbo::ref::reset() noexcept
{
      if(m_data) {
          gfx_cbo_dispose(m_data, m_size);
          m_data = nullptr;
      }
}

bool  pbo::ref::reset(int format, int sx, int sy) noexcept
{
      reset();
      if((sx < 0) || (sx > std::numeric_limits<std::int16_t>::max())) {
          return false;
      }
      if((sy < 0) || (sy > std::numeric_limits<std::int16_t>::max())) {
          return false;
      }
      m_size = get_data_size(format, sx, sy);
      m_data = gfx_pbo_reserve(m_size);
      if(m_data) {
          m_format = format;
          m_sx = sx;
          m_sy = sy;
          return true;
      }
      return true;
}

bool  pbo::ref::reset(int format, int sx, int sy, std::uint8_t* data, std::size_t size) noexcept
{
      reset();
      if((sx < 0) || (sx > std::numeric_limits<std::int16_t>::max())) {
          return false;
      }
      if((sy < 0) || (sy > std::numeric_limits<std::int16_t>::max())) {
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

pbo::ref*  pbo::ref::hook() noexcept
{
      m_hooks++;
      return this;
}

pbo::ref*  pbo::ref::drop() noexcept
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
      pbo::ptr::ptr() noexcept:
      m_ref(nullptr)
{
}

      pbo::ptr::ptr(ref* p) noexcept:
      m_ref(p)
{
}

      pbo::ptr::ptr(const ptr& copy) noexcept:
      m_ref(copy.clone())
{
      
}

      pbo::ptr::ptr(ptr&& copy) noexcept:
      m_ref(copy.release())
{
}

      pbo::ptr::~ptr()
{
      reset();
}

pbo::ref*  pbo::ptr::get() const noexcept
{
      return m_ref;
}

pbo::ref*  pbo::ptr::clone() const noexcept
{
      if(m_ref) {
          return m_ref->hook();
      }
      return nullptr;
}

void  pbo::ptr::reset() noexcept
{
      if(m_ref) {
          m_ref = m_ref->drop();
      }
}

pbo::ref*  pbo::ptr::release() noexcept
{
      ref* l_result = m_ref;
      m_ref = nullptr;
      return l_result;
}

pbo::ref*  pbo::ptr::operator->() const noexcept
{
      return m_ref;
}

      pbo::ptr::operator ref*() const noexcept
{
      return m_ref;
}

pbo::ptr&  pbo::ptr::operator=(std::nullptr_t rhs) noexcept
{
      reset();
      return *this;
}

pbo::ptr&  pbo::ptr::operator=(ref* rhs) noexcept
{
      if(m_ref) {
          if(m_ref != rhs) {
              m_ref = m_ref->drop();
          }
      }
      m_ref = rhs;
      return *this;
}

pbo::ptr&  pbo::ptr::operator=(const ptr& rhs) noexcept
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

pbo::ptr&  pbo::ptr::operator=(ptr&& rhs) noexcept
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

bool  pbo::reset() noexcept
{
      m_ptr.release();
      return true;
}

bool  pbo::reset(int format, int sx, int sy) noexcept
{
      return m_ptr = make_ptr(format, sx, sy);
}

bool  pbo::reset(int format, int sx, int sy, std::uint8_t* data, std::size_t size) noexcept
{
      return m_ptr = make_ptr(format, sx, sy, data, size);
}

void  pbo::dispose() noexcept
{
      m_ptr.reset();
}

void  pbo::release() noexcept
{
      m_ptr.release();
}

std::uint8_t* pbo::get_data_ptr() const noexcept
{
      if(m_ptr) {
          return m_ptr->get_data();
      }
      return nullptr;
}

int   pbo::get_data_size() const noexcept
{
      if(m_ptr) {
          return m_ptr->get_size();
      }
      return 0;
}

bool  pbo::is_defined() const noexcept
{
      return m_ptr;
}


      pbo::operator bool() const noexcept
{
      return m_ptr;
}

pbo&  pbo::operator=(const pbo& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          m_ptr = rhs.m_ptr;
      }
      return *this;
}

pbo&  pbo::operator=(pbo&& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          m_ptr = std::move(rhs.m_ptr);
      }
      return *this;
}

/*namespace gfx*/ }
