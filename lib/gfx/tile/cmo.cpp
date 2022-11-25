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
#include "cmo.h"
#include <os.h>
#include <util.h>
#include <limits>
#include <cstring>

namespace gfx {

      constexpr int cmo_hooks_max = 16383;

      cmo::cmo() noexcept:
      m_ptr()
{
}

      cmo::cmo(unsigned int format, int count) noexcept:
      cmo()
{
      reset(format, count);
}

      cmo::cmo(unsigned int format, int count, std::uint8_t* data, std::size_t size) noexcept:
      cmo()
{
      reset(format, count, data, size);
}

      cmo::cmo(const cmo& copy) noexcept:
      m_ptr(copy.m_ptr)
{
}

      cmo::cmo(cmo&& copy) noexcept:
      m_ptr(std::move(copy.m_ptr))
{
      copy.release();
}

      cmo::~cmo()
{
      dispose();
}

/* ref
*/
      cmo::ref::ref() noexcept:
      m_format(fmt_argb_8888),
      m_count(0),
      m_data(nullptr),
      m_size(0),
      m_hooks(1),
      m_dynamic(false)
{
}

      cmo::ref::ref(unsigned int format, int count) noexcept:
      ref()
{
      reset(format, count);
}

      cmo::ref::ref(unsigned int format, int count, std::uint8_t* data, std::size_t size) noexcept:
      ref()
{
      reset(format, count, data, size);
}

      cmo::ref::~ref()
{
      reset();
}

auto  cmo::ref::get_format() const noexcept -> unsigned int
{
      return m_format;
}

int   cmo::ref::get_colour_count() const noexcept
{
      return m_count;
}

void  cmo::ref::reset() noexcept
{
      if(m_data) {
          if(m_dynamic) {
              gfx_cmo_dispose(m_data, m_size);
          }
          m_data = nullptr;
          m_dynamic = false;
      }
}

bool  cmo::ref::reset(unsigned int format, int count) noexcept
{
      if((count >= palette_size_min) &&
          (count <= palette_size_max)) {
          reset();
          m_size = get_round_value(get_palette_size(format, count), 8);
          m_data = gfx_cmo_reserve(m_size);
          if(m_data != nullptr) {
              m_format = format;
              m_count = count;
              m_dynamic = true;
              return true;
          }
      }
      return false;
}

bool  cmo::ref::reset(unsigned int format, int count, std::uint8_t* data, std::size_t size) noexcept
{
      if((count >= palette_size_min) &&
          (count <= palette_size_max)) {
          reset();
          if(data) {
              std::size_t l_size_min = get_palette_size(format, count);
              if(size >= l_size_min) {
                  if(size <= static_cast<std::size_t>(std::numeric_limits<short int>::max())) {
                      m_format = format;
                      m_count = count;
                      m_data = data;
                      m_size = size;
                      m_dynamic = false;
                      return  true;
                  }
              }
          }
      }
      return false;
}

std::uint8_t* cmo::ref::get_data_ptr() const noexcept
{
      return m_data;
}

std::uint8_t* cmo::ref::get_data_at(int index) const noexcept
{
      if((index >= 0) &&
          (index < m_count)) {
          return m_data + index * get_colour_size(m_format);
      }
      return nullptr;
}

int   cmo::ref::get_data_size() const noexcept
{
      return m_size;
}

cmo::ref*  cmo::ref::hook() noexcept
{
      if(m_hooks < cmo_hooks_max) {
          m_hooks++;
          return this;
      }
      return nullptr;
}

cmo::ref*  cmo::ref::drop() noexcept
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

      cmo::ptr::ptr() noexcept:
      m_ref(nullptr)
{
}

      cmo::ptr::ptr(ref* p) noexcept:
      m_ref(p)
{
}

      cmo::ptr::ptr(const ptr& copy) noexcept:
      m_ref(copy.clone())
{
      
}

      cmo::ptr::ptr(ptr&& copy) noexcept:
      m_ref(copy.release())
{
}

      cmo::ptr::~ptr()
{
      reset();
}

cmo::ref*  cmo::ptr::get() const noexcept
{
      return m_ref;
}

cmo::ref*  cmo::ptr::clone() const noexcept
{
      if(m_ref) {
          return m_ref->hook();
      }
      return nullptr;
}

void  cmo::ptr::reset() noexcept
{
      if(m_ref) {
          m_ref = m_ref->drop();
      }
}

cmo::ref*  cmo::ptr::release() noexcept
{
      ref* l_result = m_ref;
      m_ref = nullptr;
      return l_result;
}

cmo::ref*  cmo::ptr::operator->() const noexcept
{
      return m_ref;
}

      cmo::ptr::operator ref*() const noexcept
{
      return m_ref;
}

cmo::ptr&  cmo::ptr::operator=(std::nullptr_t rhs) noexcept
{
      reset();
      return *this;
}

cmo::ptr&  cmo::ptr::operator=(ref* rhs) noexcept
{
      if(m_ref) {
          if(m_ref != rhs) {
              m_ref = m_ref->drop();
          }
      }
      m_ref = rhs;
      return *this;
}

cmo::ptr&  cmo::ptr::operator=(const ptr& rhs) noexcept
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

cmo::ptr&  cmo::ptr::operator=(ptr&& rhs) noexcept
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

int   cmo::get_colour_size(unsigned int format) noexcept
{
      return 1 + (format & fmt_size_bits);
}

int   cmo::get_palette_size(unsigned int format, int count) noexcept
{
      return count * get_colour_size(format);
}

auto  cmo::get_format() const noexcept -> unsigned int
{
      if(m_ptr) {
          return m_ptr->get_format();
      }
      return fmt_undef;
}

bool  cmo::has_format(unsigned int format) const noexcept
{
      if(m_ptr) {
          return m_ptr->get_format() == format;
      }
      return format == fmt_undef;
}

int   cmo::get_colour_count() const noexcept
{
      if(m_ptr) {
          return m_ptr->get_colour_count();
      }
      return 0;
}

bool  cmo::has_colour_count(int count) const noexcept
{
      if(m_ptr) {
          return m_ptr->get_colour_count() == count;
      }
      return 0;
}

bool  cmo::reset(unsigned int format, int count) noexcept
{
      return m_ptr = make_ptr(format, count);
}

bool  cmo::reset(unsigned int format, int count, std::uint8_t* data, std::size_t size) noexcept
{
      return m_ptr = make_ptr(format, count, data, size);
}

bool  cmo::set_uint8(int, std::uint8_t) noexcept
{
      return false;
}

bool  cmo::set_uint32(int index, std::uint8_t a, std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept
{
      auto p_colour = m_ptr->get_data_at(index);
      if(p_colour) {
          auto l_format = m_ptr->get_format();
          switch(l_format) {
            case fmt_rgb_232:
            case fmt_rgb_565:
            case fmt_rgb_888:
            case fmt_xrgb_0222:
            case fmt_xrgb_0444:
            case fmt_xrgb_0888:
            case fmt_argb_2222:
            case fmt_argb_4444:
                return false;
            case fmt_argb_8888:
                if constexpr (os::is_lsb) {
                    p_colour[0] = r;
                    p_colour[1] = g;
                    p_colour[2] = b;
                    p_colour[3] = a;
                    return true;
                } else
                if constexpr (os::is_msb) {
                    p_colour[0] = a;
                    p_colour[1] = b;
                    p_colour[2] = g;
                    p_colour[3] = r;
                    return true;
                }
                return false;
            case fmt_argb_float:
                return false;
            default:
                return false;
          }
      }
      return false;
}

bool  cmo::set_uint32(int index, std::uint32_t value) noexcept
{
      auto p_colour = m_ptr->get_data_at(index);
      if(p_colour) {
          auto l_format = m_ptr->get_format();
          switch(l_format) {
            case fmt_rgb_232:
            case fmt_rgb_565:
            case fmt_rgb_888:
            case fmt_xrgb_0222:
            case fmt_xrgb_0444:
            case fmt_xrgb_0888:
            case fmt_argb_2222:
            case fmt_argb_4444:
                return false;
            case fmt_argb_8888:
                if constexpr (os::is_lsb) {
                    p_colour[0] = (value & 0xff000000) >> 24;
                    p_colour[1] = (value & 0x00ff0000) >> 16;
                    p_colour[2] = (value & 0x0000ff00) >> 8;
                    p_colour[3] = (value & 0x000000ff);
                } else
                if constexpr (os::is_msb) {
                    p_colour[0] = (value & 0x000000ff);
                    p_colour[1] = (value & 0x0000ff00) >> 8;
                    p_colour[2] = (value & 0x00ff0000) >> 16;
                    p_colour[3] = (value & 0xff000000) >> 24;
                };
                return true;
            case fmt_argb_float:
                return false;
            default:
                return false;
          }
      }
      return false;
}

void  cmo::dispose() noexcept
{
      m_ptr.reset();
}

void  cmo::release() noexcept
{
      m_ptr.release();
}

      cmo::operator bool() const noexcept
{
      return m_ptr;
}

cmo&  cmo::operator=(const cmo& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          m_ptr = rhs.m_ptr;
      }
      return *this;
}

cmo&  cmo::operator=(cmo&& rhs) noexcept
{
      if(this != std::addressof(rhs)) {
          m_ptr = std::move(rhs.m_ptr);
      }
      return *this;
}

/*namespace gfx*/ }
