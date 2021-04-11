#ifndef memory_pool_h
#define memory_pool_h
/** 
    Copyright (c) 2020, wicked systems
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
#include "pool_base.h"
#include <stdarg.h>
#include <string.h>

namespace mem {

/* pool
   memory pool
   Xt - data type
   Rt - resource type (default: heap)
*/
template<typename Xt, typename Rt>
class pool: public pool_base<Xt, Rt>
{
  public:
  using  base_type     = pool_base<Xt, Rt>;
  using  node_type     = typename base_type::node_type;
  using  resource_type = typename base_type::resource_type;

  public:
  inline  pool() noexcept:
          base_type() {
  }

  inline  pool(const resource_type& resource) noexcept:
          base_type(resource) {
  }

          pool(const pool& copy) noexcept = delete;
          pool(pool&& copy) noexcept = delete;

  inline  node_type*  at(off_t index) const noexcept {
          return  base_type::at(index);
  }

  inline  node_type*  get_head() const noexcept {
          if(base_type::m_tail > base_type::m_head) {
              return base_type::m_head;
          } else
              return nullptr;
  }
  
  template<typename... Args>
  inline  node_type*  raw_get(Args&&... args) noexcept {
          return  base_type::raw_get(std::forward<Args>(args)...);
  }

  inline  node_type*  raw_unget() noexcept {
          return  base_type::raw_unget();
  }

  inline  node_type*  get_last() const noexcept {
          if(base_type::m_tail > base_type::m_head) {
              return base_type::m_tail - 1;
          } else
              return nullptr;
  }

  inline  node_type*  get_tail() const noexcept {
          if(base_type::m_tail > base_type::m_head) {
              return base_type::m_tail;
          } else
              return nullptr;
  }

  inline  bool reserve(std::size_t count) noexcept {
          return base_type::reserve(count);
  }

  inline  void clear() noexcept {
          base_type::clear();
  }

          pool& operator=(const pool&) noexcept = delete;
          pool& operator=(pool&&) noexcept = delete;
};

/* pool<char>
   memory pool specialization for char
*/
template<typename Rt>
class pool<char, Rt>: public pool_base<char, Rt>
{
  public:
  using  base_type     = pool_base<char, Rt>;
  using  node_type     = typename base_type::node_type;
  using  resource_type = typename base_type::resource_type;

  public:
  inline  pool() noexcept:
          base_type() {
  }

  inline  pool(const resource_type& resource) noexcept:
          base_type(resource) {
  }

  inline  pool(std::size_t size_min, std::size_t size_max, std::size_t size_alloc = global::cache_large_max) noexcept:
          base_type(size_min, size_max, size_alloc) {
  }

  inline  pool(const resource_type& resource, std::size_t size_min, std::size_t size_max, std::size_t size_alloc = global::cache_large_max) noexcept:
          base_type(resource, size_min, size_max, size_alloc) {
  }

          pool(const pool& copy) noexcept = delete;
          pool(pool&& copy) noexcept = delete;

  inline  auto   save() const noexcept -> std::size_t {
          return base_type::m_tail - base_type::m_head;
  }

  inline  void   save(std::size_t& offset) const noexcept {
          offset = save();
  }

  inline  char*  ptr_get(const char* src, std::size_t length = 0) noexcept {
          auto   l_offset = base_type::get_tail_pos();
          if(src) {
              if(src[0]) {
                  if(length == 0) {
                      length = std::strlen(src);
                  }
                  base_type::raw_get_array(length + 1);
                  if(char* l_dst = base_type::at(l_offset); l_dst != nullptr) {
                      std::strncpy(l_dst, src, length + 1);
                  }
                  base_type::raw_unget(1);
              }
          }
          return base_type::at(l_offset);
  }

  inline  char*  raw_get(std::size_t count) noexcept {
          return base_type::raw_get_array(count);
  }
  
  inline  char*  ptr_fmt_v(const char* fmt, va_list va) noexcept {
          va_list l_vc;
          int    l_length;
          auto   l_offset = base_type::get_tail_pos();
          if(fmt) {
              va_copy(l_vc, va);
              l_length = std::vsnprintf(nullptr, 0, fmt, va);
              if(l_length) {
                  base_type::raw_get_array(l_length + 1);
                  if(char* l_dst = base_type::at(l_offset); l_dst != nullptr) {
                      std::vsnprintf(l_dst, l_length + 1, fmt, l_vc);
                  }
                  base_type::raw_unget(1);
              }
              va_end(l_vc);
          }
          return base_type::at(l_offset);
  }

  inline  char*  ptr_fmt(const char* fmt, ...) noexcept {
          char*   l_result;
          va_list l_va;
          va_start(l_va, fmt);
          l_result = ptr_fmt_v(fmt, l_va);
          va_end(l_va);
          return  l_result;
  }

  inline  char*  raw_unget(std::size_t count) noexcept {
          return base_type::raw_unget(count);
  }
  
  inline  char*  ptr_unget(std::size_t count) noexcept {
          return base_type::raw_unget(count);
  }

  inline  auto   get_length(std::size_t offset) const noexcept {
          return base_type::get_tail_pos() - offset;
  }

  inline  char*  restore(std::size_t offset) noexcept {
          auto   l_unwind = get_length(offset);
          if(l_unwind) {
              base_type::raw_unget(l_unwind);
          }
          return base_type::at(offset);
  }

  inline  bool   reserve(std::size_t count) noexcept {
          return base_type::reserve(count);
  }

          pool& operator=(const pool&) noexcept = delete;
          pool& operator=(pool&&) noexcept = delete;
};

/*namespace mem*/ }
#endif
