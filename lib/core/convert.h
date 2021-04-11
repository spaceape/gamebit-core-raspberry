#ifndef core_convert_h
#define core_convert_h
/** 
    Copyright (c) 2018, wicked systems
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
#include "traits.h"
#include <stdlib.h>
#include <string.h>

template<typename Dt>
class convert
{
  public:
          convert() noexcept = default;
          convert(const convert&) noexcept = default;
          convert(convert&&) noexcept = default;

  template<typename St>
  static bool  get_value(Dt& dst, St&& src) noexcept(std::is_nothrow_convertible_v<St, Dt>) {
          if constexpr (std::is_convertible_v<St, Dt>) {
              dst  = std::move(src);
              return true;
          }
          return false;
  }

  static bool  get_value(int32_t& dst, const char* src) noexcept {
          if(src) {
              if(src[0]) {
                  long int l_cvt_value = strtol(src, nullptr, 0);
                  if(l_cvt_value >= std::numeric_limits<int32_t>::min()) {
                      dst = l_cvt_value;
                      return true;
                  } else
                  if(l_cvt_value <= std::numeric_limits<int32_t>::max()) {
                      dst = l_cvt_value;
                      return true;
                  } else
                      return false;
              } else
                  return false;
          } else
              return false;
  }

  static bool  get_value(uint32_t& dst, const char* src) noexcept {
          if(src) {
              if(src[0]) {
                  unsigned long int l_cvt_value = strtoul(src, nullptr, 0);
                  if(l_cvt_value <= std::numeric_limits<uint32_t>::max()) {
                      dst = l_cvt_value;
                      return true;
                  } else
                      return false;
              } else
                  return false;
          } else
              return false;
  }

  static bool  get_value(int64_t& dst, const char* src) noexcept {
          if(src) {
              if(src[0]) {
                  long long int l_cvt_value = strtoll(src, nullptr, 0);
                  if(l_cvt_value >= std::numeric_limits<int64_t>::min()) {
                      dst = l_cvt_value;
                      return true;
                  } else
                  if(l_cvt_value <= std::numeric_limits<int64_t>::max()) {
                      dst = l_cvt_value;
                      return true;
                  } else
                      return false;
              } else
                  return false;
          } else
              return false;
  }

  static bool  get_value(uint64_t& dst, const char* src) noexcept {
          if(src) {
              if(src[0]) {
                  unsigned long long int l_cvt_value = strtoull(src, nullptr, 0);
                  if(l_cvt_value <= std::numeric_limits<uint64_t>::max()) {
                      dst = l_cvt_value;
                      return true;
                  } else
                      return false;
              } else
                  return false;
          } else
              return false;
  }

  static bool  get_value(float& dst, const char* src) noexcept {
          if(src) {
              if(src[0]) {
                  dst = strtof(src, nullptr);
                  return true;
              } else
                  return false;
          } else
              return false;
  }

  static bool  get_value(double& dst, const char* src) noexcept {
          if(src) {
              if(src[0]) {
                  dst = strtod(src, nullptr);
                  return true;
              } else
                  return false;
          } else
              return false;
  }

  static bool  get_value(char* dst, const char* src) noexcept {
          if(src) {
              dst  = const_cast<char*>(src);
              return true;
          } else
              return false;
  }

  static bool  get_value(char* dst, char* src) noexcept {
          if(src) {
              dst  = src;
              return true;
          } else
              return false;
  }

          convert& operator=(const convert&) noexcept = default;
          convert& operator=(convert&&) noexcept = default;
};

template<>
class convert<char*>
{
  public:
  static constexpr size_t size_max = 64;
  
  public:
          convert() noexcept = default;
          convert(const convert&) noexcept = default;
          convert(convert&&) noexcept = default;

  template<typename St>
  static  int   get_value(char*, St&&) noexcept {
          return 0;
  }

  template<char Fmt = 'd', size_t Digits = 0>
  static  int   get_value(char* dst, int value, std::size_t size = size_max) noexcept {
          if constexpr (Fmt == 'd' || Fmt == 'i')  {
              return std::snprintf(dst, size, "%.*d", Digits, value);
          } else
          if constexpr (Fmt == 'o')  {
              return std::snprintf(dst, size, "%.*o", Digits, value);
          } else
          if constexpr (Fmt == 'x')  {
              return std::snprintf(dst, size, "%.*x", Digits, value);
          } else
          if constexpr (Fmt == 'X')  {
              return std::snprintf(dst, size, "%.*X", Digits, value);
          } else
              return 0;
  }

  template<char Fmt = 'd', size_t Digits = 0>
  static  char* from_value(char* dst, int value, std::size_t size = size_max) noexcept {
          if(get_value(dst, value, size)) {
              return dst;
          }
          return nullptr;
  }

  template<char Fmt = 'l', size_t Digits = 0>
  static  int   get_value(char* dst, long int value, std::size_t size = size_max) noexcept {
          if constexpr (Fmt == 'd' || Fmt == 'i')  {
              return std::snprintf(dst, size, "%.*ld", Digits, value);
          } else
          if constexpr (Fmt == 'o')  {
              return std::snprintf(dst, size, "%.*lo", Digits, value);
          } else
          if constexpr (Fmt == 'x')  {
              return std::snprintf(dst, size, "%.*lx", Digits, value);
          } else
          if constexpr (Fmt == 'X')  {
              return std::snprintf(dst, size, "%.*lX", Digits, value);
          } else
              return 0;
  }

  template<char Fmt = 'l', size_t Digits = 0>
  static  char* from_value(char* dst, long int value, std::size_t size = size_max) noexcept {
          if(get_value(dst, value, size)) {
              return dst;
          }
          return nullptr;
  }

  template<char Fmt = 'f', int Digits = 0, int Precision = 4>
  static  int   get_value(char* dst, double value, std::size_t size = size_max) noexcept {
          if constexpr (Fmt == 'a')  {
              return std::snprintf(dst, size, "%*.*a", Digits, Precision, value);
          } else
          if constexpr (Fmt == 'A')  {
              return std::snprintf(dst, size, "%*.*A", Digits, Precision, value);
          } else
          if constexpr (Fmt == 'e')  {
              return std::snprintf(dst, size, "%*.*e", Digits, Precision, value);
          } else
          if constexpr (Fmt == 'E')  {
              return std::snprintf(dst, size, "%*.*E", Digits, Precision, value);
          } else
          if constexpr (Fmt == 'f')  {
              return std::snprintf(dst, size, "%*.*f", Digits, Precision, value);
          } else
          if constexpr (Fmt == 'F')  {
              return std::snprintf(dst, size, "%*.*F", Digits, Precision, value);
          } else
          if constexpr (Fmt == 'g')  {
              return std::snprintf(dst, size, "%*.*g", Digits, Precision, value);
          } else
          if constexpr (Fmt == 'G')  {
              return std::snprintf(dst, size, "%*.*G", Digits, Precision, value);
          } else
              return 0;
  }

  template<char Fmt = 'f', int Digits = 0, int Precision = 4>
  static  char* from_value(char* dst, double value, std::size_t size = size_max) noexcept {
          if(get_value(dst, value, size)) {
              return dst;
          }
          return nullptr;
  }

  static  int   get_value(char* dst, const char* src, size_t size = 0) noexcept {
          if(dst) {
              if(src) {
                  if(src[0]) {
                      if(size) {
                          if(auto l_result = std::strncpy(dst, src, size); (l_result == dst) && (l_result[size] == src[size])) {
                              return size + 1;
                          }
                      } else
                      if(auto l_result = std::strcpy(dst, src); (l_result != nullptr) && (l_result[0] == 0)) {
                          return l_result - dst + 1;
                      }
                  } else
                  if(true) {
                      dst[0] = 0;
                      return 1;
                  }
              }
          }
          return 0;
  }

          convert& operator=(const convert&) noexcept = default;
          convert& operator=(convert&&) noexcept = default;
};
#endif
