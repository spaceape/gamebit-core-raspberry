#ifndef sys_rio_h
#define sys_rio_h
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

/* rio
   repeater io
*/
namespace sys {

class rio: public sys::ios
{
  ios*    m_io;

  public:
          rio() noexcept;
          rio(ios*) noexcept;
          rio(const rio&) noexcept;
          rio(rio&&) noexcept;
  virtual ~rio();

          void  set_source(ios*) noexcept;
  
  virtual int   get_char() noexcept override;
  virtual unsigned int  get_byte() noexcept override;

  virtual int   seek(int, int) noexcept override;
  virtual int   read(std::size_t) noexcept override;
  virtual int   read(std::size_t, char*) noexcept override;

  virtual int   put_char(char) noexcept override;
  virtual int   put_byte(unsigned char) noexcept override;
  virtual int   write(std::size_t, const char*) noexcept override;

  inline  ios*  get_io() noexcept {
          return m_io;
  }
 
  inline  void  set_io(ios* io) noexcept {
          m_io = io;
  }
 
  virtual int   get_size() noexcept override;

  virtual bool  is_seekable() const noexcept override;
  virtual bool  is_readable() const noexcept override;
  virtual bool  is_writable() const noexcept override;

          void  reset(bool = true) noexcept;
          void  release() noexcept;

          operator ios*() noexcept;
          operator bool() const noexcept;

          rio& operator=(const rio&) noexcept;
          rio& operator=(rio&&) noexcept;
};
/*namespace sys*/ }
#endif
