#ifndef core_char_h
#define core_char_h
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
#include "global.h"
#include "compare.h"
#include "memory.h"
#include <cstring>
#include <cstdarg>

template<typename At>
class char_base
{
  protected:
  At            m_allocator;
  char*         m_value;
  unsigned int  m_size;
  unsigned int  m_length;

  public:
  inline  char_base() noexcept:
          m_allocator(),
          m_value(nullptr),
          m_size(0),
          m_length(0) {
  }

  inline  char_base(const At& allocator) noexcept:
          m_allocator(allocator),
          m_value(nullptr),
          m_size(0),
          m_length(0) {
  }

          char_base(const char_base&) = default;
          char_base(char_base&&) = default;
          char_base& operator=(const char_base&) = default;
          char_base& operator=(char_base&&) = default;
};

/* char_ptr
   string class with variable size static buffer
*/
template<size_t Size = global::cache_small_max, typename At = heap>
class char_ptr: public char_base<At>
{
  //size of members
  static constexpr size_t memb_size = sizeof(char_base<At>);

  //structure size
  static constexpr size_t data_size = Size;

  static_assert(data_size < std::numeric_limits<short int>::max(), "text_ptr requires Size smallar than 32768");
  static_assert(data_size > memb_size, "text_ptr requires Size larger than char_base");

  using  base_type = char_base<At>;

  public:
  static constexpr size_t text_size = data_size - memb_size;

  protected:
  char    m_data[text_size];

  private:
          char*  reserve(size_t length) noexcept {
          if(length < std::numeric_limits<unsigned int>::max() - 1) {
              size_t l_size = length + 1;
              if(l_size <  text_size) {
                  dispose();
                  base_type::m_value = std::addressof(m_data[0]);
                  base_type::m_length = length;
                  return base_type::m_value;
              } else
              if(l_size <= base_type::m_size) {
                  base_type::m_length = length;
                  return base_type::m_value;
              } else
              if(1) {
                  char*   l_new_ptr;
                  size_t  l_new_size = l_size;
                  if(l_new_size % global::cache_small_max) {
                      l_new_size -= l_new_size % global::cache_small_max;
                      l_new_size += global::cache_small_max;
                  }
                  if(l_new_size < std::numeric_limits<unsigned int>::max()) {
                      l_new_ptr = reinterpret_cast<char*>(base_type::m_allocator.allocate(l_new_size));
                      if(l_new_ptr) {
                          dispose();
                          base_type::m_value = l_new_ptr;
                          base_type::m_size = l_new_size;
                          base_type::m_length = length;
                          return l_new_ptr;
                      }
                  }
              }
          }
          return nullptr;
  }

  inline  void dispose(bool reset = true) noexcept {
          if(base_type::m_value) {
              if(base_type::m_value != m_data) {
                  base_type::m_allocator.deallocate(base_type::m_value, base_type::m_size);
              }
              if(reset) {
                  base_type::m_value = nullptr;
                  base_type::m_size  = 0;
                  base_type::m_length = 0;
              }
          }
  }

  protected:
          void assign(const char* value, size_t length = 0) noexcept {
          if(value) {
              if(value[0]) {
                  if(value != base_type::m_value) {
                      if(length == 0) {
                          length = std::strlen(value);
                      }
                      
                      if(auto l_text = reserve(length); l_text != nullptr) {
                          std::strncpy(l_text, value, length + 1);
                      } else
                          dispose();
                  }
              } else
                  dispose();
          } else
              dispose();
  }

  inline  void assign(const char_ptr& copy) noexcept {
          assign(copy.m_value, copy.m_length);
  }

  inline  void assign(char_ptr&& copy) noexcept {
          if(copy.m_value != base_type::m_value) {
              if(copy.m_value != copy.m_data) {
                  dispose();
                  base_type::m_value = copy.m_value;
                  base_type::m_size = copy.m_size;
                  base_type::m_length = copy.m_length;
                  copy.release();
              } else
                  assign(copy.m_value, copy.m_length);
          }
  }

  public:
          char_ptr() noexcept:
          base_type() {
  }

          char_ptr(const At& allocator) noexcept:
          base_type(allocator) {
  }

  inline  char_ptr(const char* p, const At& allocator = At()) noexcept:
          char_ptr(allocator) {
          reset(p);
  }

  inline  char_ptr(const char_ptr& copy) noexcept:
          char_ptr() {
          assign(copy);
  }

  inline  char_ptr(char_ptr&& copy) noexcept:
          char_ptr() {
          assign(std::move(copy));
  }

  inline  ~char_ptr() {
          dispose(false);
  }

  inline  const char*  get() const noexcept {
          if(base_type::m_value) {
              if(base_type::m_value[0]) {
                  return base_type::m_value;
              } else
                  return nullptr;
          } else
              return nullptr;
  }

  inline  const char*  fmt(const char* fmt, ...) noexcept {
          va_list va;
          va_start(va, fmt);
          auto l_result = fmt_v(fmt, va);
          va_end(va);
          return l_result;
  }

  inline  const char*  fmt_v(const char* fmt, va_list va) noexcept {
          int     l_size;
          va_list vc;
          va_copy(vc, va);
          l_size = vsnprintf(nullptr, 0, fmt, va);
          if(l_size) {
              auto l_data = reserve(l_size);
              if(l_data) {
                  vsnprintf(l_data, l_size + 1, fmt, vc);
              }
          }
          va_end(vc);
          return get();
  }

  inline  unsigned int get_length() const noexcept {
          return base_type::m_length;
  }

  inline  void reset(const char* value, std::size_t length = 0) noexcept {
          return assign(value, length);
  }

  inline  void release() noexcept {
          if(base_type::m_value) {
              base_type::m_value = nullptr;              base_type::m_size  = 0;
          }
          base_type::m_length = 0;
  }

  inline  bool operator>(const char* rhs) const noexcept {
          return compare(base_type::m_value, rhs) > 0;
  }

  inline  bool operator>=(const char* rhs) const noexcept {
          return compare(base_type::m_value, rhs) >= 0;
  }

  inline  bool operator<(const char* rhs) const noexcept {
          return compare(base_type::m_value, rhs) < 0;
  }

  inline  bool operator<=(const char* rhs) const noexcept {
          return compare(base_type::m_value, rhs) <= 0;
  }

  template<size_t OtherSize, typename OtherAt>
  inline  bool operator==(char_ptr<OtherSize, OtherAt>& rhs) const noexcept {
          if(base_type::m_value != rhs.m_value) {
              if(base_type::m_length == rhs.m_length) {
                  return std::strncmp(base_type::m_value, rhs.m_value, rhs.m_length) == 0;
              } else
                  return false;
          } else
              return false;
  }

  inline  bool operator==(const char* rhs) const noexcept {
          return compare(base_type::m_value, rhs) == 0;
  }

  template<size_t OtherSize, typename OtherAt>
  inline  bool operator!=(char_ptr<OtherSize, OtherAt>& rhs) const noexcept {
          return operator==<OtherSize, OtherAt>(rhs) == false;
  }

  inline  bool operator!=(const char* rhs) const noexcept {
          return compare(base_type::m_value, rhs) != 0;
  }

  inline  operator bool() const noexcept {
          return base_type::m_value && base_type::m_value[0];
  }

  inline  operator const char*() const noexcept {
          return get();
  }

  inline  char_ptr& operator=(const char* rhs) noexcept {
          assign(rhs);
          return *this;
  }

  inline  char_ptr& operator=(const char_ptr& rhs) noexcept {
          assign(rhs);
          return *this;
  }

  inline  char_ptr& operator=(char_ptr&& rhs) noexcept {
          assign(std::move(rhs));
          return *this;
  }
};
#endif
