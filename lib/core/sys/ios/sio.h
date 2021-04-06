#ifndef sys_sio_h
#define sys_sio_h
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
#include <memory/fragment.h>

/* sio
   static data buffer
*/
class sio: public sys::ios
{
  fragment* m_resource;

  protected:
  char*   m_data_head;
  char*   m_read_iter;
  int     m_read_size;
  int     m_data_size;

  protected:
          void assign(const sio&) noexcept;
          void assign(sio&&) noexcept;

  public:
          sio(std::size_t = 0) noexcept;
          sio(fragment*, std::size_t = 0) noexcept;
          sio(const char*, std::size_t = 0) noexcept;
          sio(fragment*, const char*, std::size_t = 0) noexcept;
          sio(const sio&) noexcept;
          sio(sio&&) noexcept;
  virtual ~sio();

          char* get_ptr(int) noexcept;
          char* set_ptr(int, int) noexcept;
  
  virtual int  get_char() noexcept override;
  virtual unsigned int  get_byte() noexcept override;

          int  load(sys::ios*, std::size_t = 0) noexcept;
          int  load(const char*, std::size_t = 0) noexcept;

          int  save() noexcept;

  virtual int  seek(int, int) noexcept override;

  virtual int  read(std::size_t) noexcept override;
  virtual int  read(std::size_t, char*) noexcept override;

  virtual int  put_char(char) noexcept override;
  virtual int  put_byte(unsigned char) noexcept override;
  virtual int  write(std::size_t, const char*) noexcept override;

  virtual int  get_size() noexcept override;
          int  get_capacity() const noexcept;

  virtual bool is_seekable() const noexcept override;
  virtual bool is_readable() const noexcept override;
  virtual bool is_writable() const noexcept override;

          void restore(int) noexcept;

          bool reserve(std::size_t) noexcept;
          void drop() noexcept;
          void drop(int) noexcept;
          void clear() noexcept;  
          void reset(bool = true) noexcept;
          void release() noexcept;

          operator ios*() noexcept;
          operator bool() const noexcept;

          sio& operator=(const sio&) noexcept;
          sio& operator=(sio&&) noexcept;
};
#endif
