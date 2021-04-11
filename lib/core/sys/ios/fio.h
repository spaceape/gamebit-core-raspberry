#ifndef sys_fio_h
#define sys_fio_h
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
#include <sys.h>
#include <sys/ios.h>
#include <sys/raw/drive.h>

/* fio
   file IO via the fs API
*/
namespace sys {

class fio: public ios
{
  drive*        m_drive;
  long int      m_mode;
  std::uint32_t m_address;
  long int      m_offset;
  long int      m_size;

  protected:
          void   assign(const fio&) noexcept;
          void   assign(fio&&) noexcept;

  public:
          fio() noexcept;
          fio(drive*, const char*, long int = O_RDWR, long int = 0777) noexcept;
          fio(const fio&) noexcept;
          fio(fio&&) noexcept;
  virtual ~fio();

  virtual int   get_char() noexcept override;
  virtual unsigned int  get_byte() noexcept override;

  virtual int   seek(int, int) noexcept override;
  virtual int   read(std::size_t) noexcept override;
  virtual int   read(std::size_t, char*) noexcept override;

  virtual int   put_char(char) noexcept override;
  virtual int   put_byte(unsigned char) noexcept override;

  virtual int   write(std::size_t, const char*) noexcept override;

  virtual int   get_size() noexcept override;

          bool  set_blocking(bool = true) noexcept;
          bool  set_nonblocking() noexcept;

  virtual bool  is_seekable() const noexcept override;
  virtual bool  is_readable() const noexcept override;
  virtual bool  is_writable() const noexcept override;

          void  reset() noexcept;
          void  release() noexcept;
          void  close(bool = true) noexcept;

          operator ios*() noexcept;
          operator bool() const noexcept;

          fio& operator=(const fio&) noexcept;
          fio& operator=(fio&&) noexcept;
};
/*namespace sys*/ }
#endif
