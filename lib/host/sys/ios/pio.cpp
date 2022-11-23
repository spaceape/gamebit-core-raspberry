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
#include "pio.h"
#include "sio.h"
#include <cstring>
#include <limits>

namespace sys {

      constexpr char s_ident_0[] = "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
      constexpr char s_ident_1[] = "0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

      pio::pio() noexcept:
      ios(),
      m_io(nullptr),
      m_char(EOF)
{
}

      pio::pio(sys::ios* io) noexcept:
      ios(),
      m_io(io),
      m_char(EOF)
{
}

      pio::pio(sio& io) noexcept:
      pio(std::addressof(io))
{
}

      pio::pio(const pio& copy) noexcept:
      ios(copy),
      m_io(copy.m_io),
      m_char(copy.m_char)
{
}

      pio::pio(pio&& copy) noexcept:
      ios(std::move(copy)),
      m_io(copy.m_io),
      m_char(copy.m_char)
{
      copy.release();
}

      pio::~pio()
{
      reset(false);
}

int   pio::get_char() noexcept
{
      return m_io->get_char();
}

unsigned int pio::get_byte() noexcept
{
      return m_io->get_byte();
}

int   pio::seek(int offset, int whence) noexcept
{
      return m_io->seek(offset, whence);
}

int   pio::read(int count) noexcept
{
      return m_io->read(count);
}

int   pio::read(int count, char* memory) noexcept
{
      return m_io->read(count, memory);
}

/* peek()
    get the value stored in the character buffer
*/
bool  pio::peek(int& value) noexcept
{
      if(__builtin_expect(m_char == EOF, true)) {
          skip();
      }
      value = m_char;
      if(__builtin_expect(m_char != EOF, true)) {
          return true;
      }
      return false;
}

/* skip()
    extract the next value from the stream and store it into the character buffer
*/
void  pio::skip() noexcept
{
      m_char = get_char();
}

/* skip()
    skip and increment the counter
*/
void  pio::skip(int& counter) noexcept
{
      skip();
      counter++;
}

/* get_any()
*/
int   pio::get_any(int& value) noexcept
{
      if(peek(value)) {
          skip();
          return 1;
      }
      return 0;
}

/* get_space()
    skip over the spaces in the stream
*/
int   pio::get_space() noexcept
{
      int  l_char;
      int  l_count = 0;
      while(peek(l_char)) {
          if(l_char <= SPC) {
              skip(l_count);
          } else
              break;
      }
      return l_count;
}

/* get_keyword()
   read a string wrapped by spaces
*/
int   pio::get_keyword(const char* imm) noexcept
{
      int  l_char;
      bool l_pass  = true;
      int  l_count = 0;
      if(imm) {
          while(peek(l_char)) {
              if(l_char == SPC) {
                  l_pass = imm[l_count] == SPC;
                  break;
              } else
              if(l_char != imm[l_count]) {
                  l_pass = imm[l_count] == SPC;
                  break;
              } else
              if(l_char == imm[l_count]) {
                  skip(l_count);
              }
          }
          if(l_pass) {
              return l_count;
          }
      }
      return 0;
}

/* get_symbol()
   read a character symbol
*/
int   pio::get_symbol(char imm) noexcept
{
      int l_char;
      int l_imm = imm;
      if(imm) {
          peek(l_char);
          if(l_char == l_imm) {
              skip();
              return 1;
          }
      }
      return 0;
}

/* get_symbol()
   read a string terminated by any character not belonging to it
*/
int   pio::get_symbol(const char* imm) noexcept
{
      int  l_char;
      bool l_pass  = true;
      int  l_count = 0;
      if(imm) {
          while(peek(l_char)) {
              if(imm[l_count]) {
                  if(l_char != imm[l_count]) {
                      break;
                  } else
                  if(l_char == imm[l_count]) {
                      skip(l_count);
                  }
              } else
                  break;
          }
          if(l_pass) {
              return l_count;
          }
      }
      return 0;
}

/* get_ident()
   read idetifier into the buffer pointed to by <ident>, with a max length of <length>
*/
int   pio::get_ident(char* ident, int length) noexcept
{
      int  l_char;
      int  l_count = 0;
      if(length > 0) {
          if(peek(l_char)) {
              if(std::strchr(s_ident_0, l_char)) {
                  ident[l_count] = l_char;
                  skip(l_count);
              } else
                  return 0;
          } else
              return 0;
          while(peek(l_char)) {
              if(std::strchr(s_ident_1, l_char) == nullptr) {
                  if(l_count >= length) {
                      ident[0] = 0;
                      return 0;
                  }
                  ident[l_count] = 0;
                  return l_count;
              }
              if(l_count < length) {
                  if(l_count < length - 1) {
                      ident[l_count] = l_char;
                  } else
                      ident[l_count] = 0;
              }
              skip(l_count);
          }
      }
      return 0;
}

/* get_word()
   read string in between spaces into the buffer pointed to by <word>, with a max length of <length>
*/
int   pio::get_word(char* word, int length) noexcept
{
      int  l_char;
      int  l_count = 0;
      if(length > 0) {
          while(peek(l_char)) {
              if((l_char >= 0) && (l_char <= 32)) {
                  if(l_count >= length) {
                      word[0] = 0;
                      return 0;
                  }
                  word[l_count] = 0;
                  return l_count;
              }
              if(l_count < length) {
                  if(l_count < length - 1) {
                      word[l_count] = l_char;
                  } else
                      word[l_count] = 0;
              }
              skip(l_count);
          }
      }
      return 0;
}

/* get_bin_value()
    read the digits of a binary number and save the conversion result into <value>
*/
int   pio::get_bin_value(std::int32_t& value, int length) noexcept
{
      int  l_char;
      int  l_count = 0;
      int  l_value = 0;
      while(peek(l_char)) {
          if(l_count < length) {
              if(l_count <= static_cast<int>(sizeof(value)) * 8) {
                  if((l_char >= '0') && (l_char <= '1')) {
                      if(l_value > std::numeric_limits<std::int32_t>::max() / 2) {
                          return 0;
                      }
                      l_value *= 2;
                      l_value += l_char - '0';
                      l_count++;
                      skip();
                  } else
                      break;
              } else
                  return 0;
          } else
              return 0;
      }
      value = l_value;
      return  l_count;
}

/* get_dec_value()
    read the digits of a decimal number and save the conversion result into <value>
*/
int   pio::get_dec_value(std::int32_t& value) noexcept
{
      int  l_char;
      int  l_count = 0;
      int  l_value = 0;
      int  l_digit;
      while(peek(l_char)) {
          if((l_char >= '0') && (l_char <= '9')) {
              if(l_value < std::numeric_limits<std::int32_t>::max() / 10) {
                  l_digit = l_char - '0';
                  if(l_digit < std::numeric_limits<std::int32_t>::max() / 10 - l_value) {
                      l_value *= 10;
                      l_value += l_digit;
                  } else
                      return 0;
              } else
                  return 0;
              l_count++;
              skip();
          } else
              break;
      }
      value = l_value;
      return  l_count;
}

/* get_hex_value()
    read the digits of a hex number and save the conversion result into <value>
*/
int   pio::get_hex_value(std::int32_t& value, int length) noexcept
{
      int  l_char;
      int  l_count = 0;
      int  l_value = 0;
      while(peek(l_char)) {
          if(l_count < length) {
              if(l_count <= static_cast<int>(sizeof(value)) * 2) {
                  if((l_char >= '0') && (l_char <= '9')) {
                      if(l_value > std::numeric_limits<std::int32_t>::max() / 16) {
                          return 0;
                      }
                      l_value *= 16;
                      l_value += l_char - '0';
                      l_count++;
                      skip();
                  } else
                  if((l_char >= 'A') && (l_char <= 'F')) {
                      if(l_value > std::numeric_limits<std::int32_t>::max() / 16) {
                          return 0;
                      }
                      l_value *= 16;
                      l_value += 10 + l_char - 'A';
                      l_count++;
                      skip();
                  } else
                  if((l_char >= 'a') && (l_char <= 'f')) {
                      if(l_value > std::numeric_limits<std::int32_t>::max() / 16) {
                          return 0;
                      }
                      l_value *= 16;
                      l_value += 10 + l_char - 'a';
                      l_count++;
                      skip();
                  } else
                      break;
              } else
                  return 0;
          } else
              return 0;
      }
      value = l_value;
      return  l_count;
}

int   pio::get_int(std::int32_t& value) noexcept
{
      int  l_char;
      int  l_length;
      if(peek(l_char)) {
          if(l_char == '+') {
              skip();
              l_length = get_dec_value(value);
              if(l_length) {
                  return l_length + 1;
              }
          } else
          if(l_char == '-') {
              skip();
              l_length = get_dec_value(value);
              if(l_length) {
                  value *= -1;
                  return l_length + 1;
              }
          } else
          if(l_char == '0') {
              skip();
              if(peek(l_char)) {
                  if((l_char == 'b') || (l_char == 'B')) {
                      skip();
                      return get_bin_value(value);
                  } else
                  if((l_char == 'x') || (l_char == 'X')) {
                      skip();
                      return get_hex_value(value);
                  }
                  value = 0;
                  return 1;
              }
          } else
          if((l_char >= '1') && (l_char <= '9')) {
              return get_dec_value(value);
          }
      }
      value = 0;
      return 0;
}

int   pio::get_float(float&) noexcept
{
      return 0;
}

/* get_line()
   skip over everything until the end of the line
*/
int   pio::get_line() noexcept
{
      int  l_char;
      int  l_count = 0;
      while(peek(l_char)) {
          if(l_char != EOL) {
              skip(l_count);
          } else
              break;
      }
      return l_count;
}

/* get_line()
   skip over everything until the end of the line
*/
int   pio::get_line(char* line, int length) noexcept
{
      int  l_char;
      int  l_count;
      if(length > 0) {
          l_count = 0;
          while(peek(l_char)) {
              if(l_char >= SPC) {
                  if(l_count < length) {
                      if(l_count < length - 1) {
                          line[l_count] = l_char;
                      } else
                          line[l_count] = 0;
                  }
                  skip(l_count);
              } else
                  break;
          }
          return l_count;
      }
      return 0;
}

int   pio::put_char(char value) noexcept
{
      return m_io->put_char(value);
}

int   pio::put_byte(unsigned char value) noexcept
{
      return m_io->put_char(value);
}

int   pio::write(int size, const char* data) noexcept
{
      return m_io->write(size, data);
}

int   pio::get_size() noexcept
{
      return m_io->get_size();
}

bool  pio::is_seekable() const noexcept
{
      return m_io->is_seekable();
}

bool  pio::is_readable() const noexcept
{
      return m_io->is_readable();
}

bool  pio::is_writable() const noexcept
{
      return m_io->is_writable();
}

void  pio::reset(bool clear) noexcept
{
      if(clear) {
          release();
      }
}

void  pio::clear() noexcept
{
      m_char = EOF;
}

void  pio::release() noexcept
{
      m_io   = nullptr;
      m_char = EOF;
}

      pio::operator sys::ios*() noexcept
{
      return this;
}

      pio::operator bool() const noexcept
{
      return m_io;
}

pio&  pio::operator=(const pio& rhs) noexcept
{
      m_io = rhs.m_io;
      m_char = rhs.m_char;
      return *this;
}

pio&  pio::operator=(pio&& rhs) noexcept
{
      m_io = rhs.m_io;
      m_char = rhs.m_char;
      rhs.release();
      return *this;
}

/*namespace sys*/ }

