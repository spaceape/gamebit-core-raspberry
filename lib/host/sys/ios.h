#ifndef sys_ios_h
#define sys_ios_h
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
#include <os.h>
#include <sys.h>
#include "fmt.h"

namespace sys {

class ios;
class bio;
class pio;

/* ios
   abstract base class for data streams;
*/
class ios
{
  inline  int lsb_get() noexcept {
          return 0;
  }

  inline  int lsb_put() noexcept {
          return 0;
  }

  inline  int msb_get() noexcept {
          return 0;
  }

  inline  int msb_put() noexcept {
          return 0;
  }

   public:
          ios() noexcept;
          ios(const ios&) noexcept;
          ios(ios&&) noexcept;
  virtual ~ios();

  /* get_char()
     extract signed char from the stream and return its value or -1 if unable to
  */
  virtual int  get_char() noexcept = 0;

  /* put_char()
     place a signed char onto the stream
  */
  virtual int  put_char(char) noexcept = 0;

  /* get_byte()
     extract unsigned char from the stream and return its value or UINT_MAX if unable to
  */
  virtual unsigned int get_byte() noexcept = 0;

  /* put_byte()
     place an unsigned signed char onto the stream
  */
  virtual int  put_byte(std::uint8_t) noexcept = 0;

  /* put_str()
     place an object of given <type> in little endian onto the stream
  */
  inline  int  put_string(const char* value, int length = 0) noexcept {   
          int  l_length = length;
          if(length == 0) {
              if(value) {
                  l_length = std::strlen(value);
              } else
                  l_length = 0;
          } else
              l_length = length;
          return write(l_length, value);
  }

  template<typename... Args>
  inline  int  lsb_get(char& value, Args&&... next) noexcept {
          value = get_char();
          return  lsb_get(std::forward<Args>(next)...) + 1;
  }

  template<typename... Args>
  inline  int  lsb_put(char value, Args&&... next) noexcept {
          put_char(value);
          return  lsb_put(std::forward<Args>(next)...) + 1;
  }

  template<typename... Args>
  inline  int  msb_get(char& value, Args&&... next) noexcept {
          value = get_char();
          return  msb_get(std::forward<Args>(next)...) + 1;
  }

  template<typename... Args>
  inline  int  msb_put(char value, Args&&... next) noexcept {
          put_char(value);
          return  msb_put(std::forward<Args>(next)...) + 1;
  }

  /* lsb_get<char>
     get one char from stream in little endian
  */
  template<typename... Args>
  inline  int  lsb_get(unsigned char& value, Args&&... next) noexcept {
          value = get_byte();
          return  lsb_get(std::forward<Args>(next)...) + 1;
  }

  template<typename... Args>
  inline  int  lsb_put(unsigned char value, Args&&... next) noexcept {
          put_char(value);
          return  lsb_put(std::forward<Args>(next)...) + 1;
  }

  /* msb_get<char>
     get one char from stream in big_endian
  */
  template<typename... Args>
  inline  int  msb_get(unsigned char& value, Args&&... next) noexcept {
          value = get_byte();
          return  msb_get(std::forward<Args>(next)...) + 1;
  }

  template<typename... Args>
  inline  int  msb_put(unsigned char value, Args&&... next) noexcept {
          put_char(value);
          return  lsb_put(std::forward<Args>(next)...) + 1;
  }

  /* lsb_get<type>
     get an object of given <type> in little endian; this is meant for arithmetic types,
     use read() and alike for structs.
  */
  template<typename Xt, typename... Args>
  inline  int  lsb_get(Xt* value, Args&&... next) noexcept {    
          int  l_size = sizeof(Xt);
          auto l_data = reinterpret_cast<char*>(value);
          if constexpr (os::is_lsb) {
              for(int l_index = 0; l_index < l_size; l_index++) {
                  l_data[l_index] = get_byte();
              }
          } else
          if constexpr (os::is_msb) {
              for(int l_index = l_size - 1; l_index >= 0; l_index--) {
                  l_data[l_index] = get_byte();
              }
          }
          return lsb_get(std::forward<Args>(next)...) + l_size;
  }

  template<typename... Args>
  inline  int  lsb_put(const char* value, Args&&... next) noexcept {    
          return put_string(value) + lsb_put(std::forward<Args>(next)...);
  }

  /* msb_get<type>
     get an object of given <type> in big endian; this is meant for arithmetic types,
     use read() and alike for structs.
  */
  template<typename Xt, typename... Args>
  inline  int  msb_get(Xt* value, Args&&... next) noexcept {    
          int  l_size = sizeof(Xt);
          auto l_data = reinterpret_cast<char*>(value);
          if constexpr (os::is_lsb) {
              for(int l_index = l_size - 1; l_index >= 0; l_index--) {
                  l_data[l_index] = get_byte();
              }
          } else
          if constexpr (os::is_msb) {
              for(int l_index = 0; l_index < l_size; l_index++) {
                  l_data[l_index] = get_byte();
              }
          }
          return msb_get(std::forward<Args>(next)...) + l_size;
  }

  template<typename... Args>
  inline  int  msb_put(const char* value, Args&&... next) noexcept {
          return put_string(value) + msb_put(std::forward<Args>(next)...);
  }

  template<typename Xt, typename... Args>
  inline  int  lsb_get(Xt& value, Args&&... next) noexcept {
          return lsb_get(std::addressof(value), std::forward<Args>(next)...);
  }

  /* lsb_put<type>
  */
  template<typename Xt, typename... Args>
  inline  int  lsb_put(Xt& value, Args&&... next) noexcept {    
          int  l_size = sizeof(Xt);
          auto l_data = reinterpret_cast<char*>(std::addressof(value));
          if constexpr (os::is_lsb) {
                write(l_size, l_data);
          } else
          if constexpr (os::is_msb) {
              for(int l_index = l_size - 1; l_index >= 0; l_index--) {
                  put_byte(l_data[l_index]);
              }
          }
          return lsb_put(std::forward<Args>(next)...) + l_size;
  }

  template<typename Xt, typename... Args>
  inline  int  msb_get(Xt& value, Args&&... next) noexcept {
          return msb_get(std::addressof(value), std::forward<Args>(next)...);
  }

  /* msb_put<type>
  */
  template<typename Xt, typename... Args>
  inline  int  msb_put(Xt& value, Args&&... next) noexcept {
          int  l_size = sizeof(Xt);
          auto l_data = reinterpret_cast<char*>(std::addressof(value));
          if constexpr (os::is_lsb) {
              for(int l_index = l_size - 1; l_index >= 0; l_index--) {
                  put_byte(l_data[l_index]);
              }
          } else
          if constexpr (os::is_msb) {
              write(l_size, l_data);
          }
          return msb_put(std::forward<Args>(next)...) + l_size;
  }

  template<typename Xt, typename... Args>
  inline  int  get(Xt& value, Args&&... next) noexcept {
          if constexpr (os::is_lsb) {
              return lsb_get(value, std::forward<Args>(next)...);
          } else
              return msb_get(value, std::forward<Args>(next)...);
  }

  template<typename Xt, typename... Args>
  inline  int  put(Xt& value, Args&&... next) noexcept {
          if constexpr (os::is_lsb) {
              return lsb_put(value, std::forward<Args>(next)...);
          } else
              return msb_put(value, std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  int  put(char value, Args&&... next) noexcept {
          return put_char(value) + put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  int  put(unsigned char value, Args&&... next) noexcept {
          return  put_byte(value) + put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  int  put(char* value, Args&&... next) noexcept {
          return  put_string(value) + put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  int  put(const char* value, Args&&... next) noexcept {
          return  put_string(value) + put(std::forward<Args>(next)...);
  }

  template<char option, typename... Args>
  inline  int  put(const fmt::p<option>& value, Args&&... next) noexcept {
          return put_string(value) + put(std::forward<Args>(next)...);
  }

  template<char option, typename... Args>
  inline  int  put(fmt::p<option>&& value, Args&&... next) noexcept {
          return put_string(value) + put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  int  put(const fmt::x& value, Args&&... next) noexcept {
          return put_string(value) + put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  int  put(fmt::x&& value, Args&&... next) noexcept {
          return put_string(value) + put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  int  put(const fmt::X& value, Args&&... next) noexcept {
          return put_string(value) + put(std::forward<Args>(next)...);
  }

  template<typename... Args>
  inline  int  put(fmt::X&& value, Args&&... next) noexcept {
          return put_string(value) + put(std::forward<Args>(next)...);
  }

  virtual int seek(int, int) noexcept = 0;
   
  inline  int  save() noexcept {
          if(is_seekable()) {
              return seek(0, SEEK_CUR);
          }
          return -1;
  }

  inline  bool save(int& offset) noexcept {
          if(is_seekable()) {
              offset = save();
              return true;
          }
          offset = -1;
          return false;
  }

  virtual int  read(int) noexcept = 0;
  virtual int  read(int, char*) noexcept = 0;

  template<typename Xt>
  inline  int  read(int count, Xt* data) noexcept {
          return read(count, reinterpret_cast<char*>(data));
  }
  
  virtual int  write(int, const char*) noexcept = 0;
  
  template<typename Xt>
  inline  int  write(int count, const Xt* data) noexcept {
          return write(count, reinterpret_cast<const char*>(data));
  }
  
  inline  int  pad(int count, int value = 0) noexcept {
          int  l_result = 0;
          if(count) {
              int l_copy_bytes = static_cast<int>(sizeof(int));
              while(count >= l_copy_bytes) {
                  l_result += put(value);
                  count    -= l_copy_bytes;
              }
              if(count > 0) {
                  char* l_bytes = reinterpret_cast<char*>(std::addressof(value));
                  for(int i_char = 0; i_char < l_copy_bytes; i_char++) {
                      put_char(l_bytes[i_char]);
                  }
              }
          }
          return l_result;
  }

  inline  bool restore(int& offset) noexcept {
          if(is_seekable()) {
              if(seek(offset, SEEK_SET) >= 0) {
                  return true;
              }
          }
          return false;
  }

  virtual bool is_seekable() const noexcept = 0;
          bool is_serial() const noexcept;
  virtual bool is_readable() const noexcept = 0;
  virtual bool is_writable() const noexcept = 0;

  virtual int  get_size() noexcept = 0;

          ios& operator=(const ios&) noexcept;
          ios& operator=(ios&&) noexcept;
};

/*namespace sys*/ }
#endif
