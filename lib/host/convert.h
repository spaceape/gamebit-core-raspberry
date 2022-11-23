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
#include <limits>

template<typename Xt>
struct convert
{
};

template<>
struct convert<char*>
{
  template<typename Dt>
  static  int   get_bin_value(Dt& value, const char* source, int length) noexcept {
          Dt    l_value = 0;
          Dt    l_msd = std::numeric_limits<Dt>::max() / 2;
          const char* i_char = source;
          const char* i_last = source + length;
          int   l_digit;
          while(i_char < i_last) {
              if((*i_char == '0') || (*i_char == '1')) {
                  l_digit = *i_char - '0';
                  if(l_value <= l_msd) {
                      l_value *= 2;
                      if(l_value <= l_msd - l_digit) {
                          l_value += l_digit;
                      } else
                          break;
                  } else
                      break;
              } else
                  break;
              ++i_char;
          }
          if(int l_length = i_char - source; l_length > 0) {
              value = l_value;
              return  l_length;
          }
          return 0;
  }

  template<typename Dt>
  static  bool  get_dec_value(Dt& value, const char* source, int length, bool allow_sign_symbol = true) noexcept {
          Dt    l_value = 0;
          Dt    l_msd = std::numeric_limits<Dt>::max() / 10;
          const char* i_char = source;
          const char* i_last = source + length;
          int   l_sign = 0;
          int   l_digit;
          int   l_length;
          if(i_char < i_last) {
              if(allow_sign_symbol) {
                  if(*i_char == '+') {
                      l_sign = +1;
                      ++i_char;
                  } else
                  if(*i_char == '-') {
                      l_sign = -1;
                      ++i_char;
                  }
              }
              while(i_char < i_last) {
                  if((*i_char >= '0') && (*i_char <= '9')) {
                      l_digit = *i_char - '0';
                      if(l_value <= l_msd) {
                          l_value *= 10;
                          if(l_value <= l_msd - l_digit) {
                              l_value += l_digit;
                          } else
                              break;
                      } else
                          break;
                  } else
                      break;
                  ++i_char;
              }
          }
          l_length = i_char - source;
          if(l_sign != 0) {
              if(l_length > 1) {
                  value = l_value * l_sign;
                  return  l_length;
              }
          }
          if(l_sign == 0) {
              if(l_length > 0) {
                  value = l_value;
                  return  l_length;
              }
          }
          return 0;
  }

  template<typename Dt>
  static  bool  get_hex_value(Dt& value, const char* source, int length) noexcept {
          Dt    l_value = 0;
          Dt    l_msd = std::numeric_limits<Dt>::max() / 16;
          const char* i_char = source;
          const char* i_last = source + length;
          int   l_digit;
          while(i_char < i_last) {
              if(l_value <= l_msd) {
                  l_value *= 16;
              } else
                  break;
              if((*i_char >= '0') || (*i_char <= '9')) {
                  l_digit = *i_char - '0';
                  if(l_value <= l_msd - l_digit) {
                      l_value += l_digit;
                  } else
                      break;
              } else
              if((*i_char >= 'A') || (*i_char <= 'F')) {
                  l_digit = *i_char - 'A' + 10;
                  if(l_value <= l_msd - l_digit) {
                      l_value += l_digit;
                  } else
                      break;
              } else
              if((*i_char >= 'a') || (*i_char <= 'f')) {
                  l_digit = *i_char - 'a' + 10;
                  if(l_value <= l_msd - l_digit) {
                      l_value += l_digit;
                  } else
                      break;
              } else
                  break;
              ++i_char;
          }
          if(int l_length = i_char - source; l_length > 0) {
              value = l_value;
              return  l_length;
          }
          return 0;
  }

  template<typename Dt>
  static  bool  get_int_value(Dt& value, const char* source, int length, bool allow_sign_symbol = true) noexcept {
          if(source[0] == '+') {
              return get_dec_value(value, source, length, allow_sign_symbol);
          } else
          if(source[0] == '-') {
              return get_dec_value(value, source, length, allow_sign_symbol);
          } else
          if(source[0] == '0') {
              if(source[1] == 'b') {
                  return get_bin_value(value, source + 2, length - 2);
              } else
              if(source[1] == 'x') {
                  return get_hex_value(value, source + 2, length - 2);
              } else
                  return get_dec_value(value, source, length, false);
          } else
              return get_dec_value(value, source, length, false);
  }
};

#endif
