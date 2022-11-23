#ifndef sys_ios_pio_h
#define sys_ios_pio_h
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
#include <sys.h>
#include <sys/ios.h>

/* pio
   unbuffered io with a rudimentary lexer interface
*/
namespace sys {

class sio;

class pio: public ios
{
  ios*    m_io;
  int     m_char;

  public:
          pio() noexcept;
          pio(ios*) noexcept;
          pio(sio&) noexcept;
          pio(const pio&) noexcept;
          pio(pio&&) noexcept;
  virtual ~pio();

  virtual int   get_char() noexcept override;
  virtual unsigned int  get_byte() noexcept override;

  virtual int   seek(int, int) noexcept override;
  virtual int   read(int) noexcept override;
  virtual int   read(int, char*) noexcept override;

          bool  peek(int&) noexcept;
          void  skip() noexcept;
          void  skip(int&) noexcept;
          int   get_any(int&) noexcept;
          int   get_space() noexcept;
          int   get_keyword(const char*) noexcept;
          int   get_symbol(char) noexcept;
          int   get_symbol(const char*) noexcept;
          int   get_ident(char*, int) noexcept;
          int   get_word(char*, int) noexcept;
          int   get_bin_value(std::int32_t&, int = 32) noexcept;
          int   get_dec_value(std::int32_t&) noexcept;
          int   get_hex_value(std::int32_t&, int = 8) noexcept;
          int   get_int(std::int32_t&) noexcept;
          int   get_float(float&) noexcept;
          int   get_line() noexcept;
          int   get_line(char*, int) noexcept;

  virtual int   put_char(char) noexcept override;
  virtual int   put_byte(unsigned char) noexcept override;
  virtual int   write(int, const char*) noexcept override;

  virtual int   get_size() noexcept override;

  virtual bool  is_seekable() const noexcept override;
  virtual bool  is_readable() const noexcept override;
  virtual bool  is_writable() const noexcept override;

          void  clear() noexcept;
          void  reset(bool = true) noexcept;
          void  release() noexcept;

          operator ios*() noexcept;
          operator bool() const noexcept;

          pio&  operator=(const pio&) noexcept;
          pio&  operator=(pio&&) noexcept;
};
/*namespace sys*/ }
#endif
