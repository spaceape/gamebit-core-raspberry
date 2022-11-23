#ifndef sys_ios_bio_h
#define sys_ios_bio_h
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
#include <mmi.h>
#include <sys/ios.h>

namespace sys {

/* bio
   buffered io
*/
class bio: public sys::ios
{
  ios*          m_io;
  fragment*     m_resource;

  protected:
  char*         m_data_head;
  int           m_file_pos;         // actual file pointer, before the last read operation
  int           m_read_pos;         // internal file pointer (buffer base)
  int           m_read_iter;        // current position, relative to m_read_pos
  int           m_read_size;        // size of the read data, relative to m_read_pos
  int           m_save_size;        // size of the data pending write
  char*         m_data_tail;
  char          m_eol;
  std::int8_t   m_lock_ctr;         // buffer lock count
  std::int16_t  m_grow_size;        // how many bytes the internal cache grows with
  bool          m_enable_bit:1;
  bool          m_commit_bit:1;     // commit the buffer before discarding

  protected:
          bool  load(int, bool) noexcept;
          void  unload() noexcept;

  public:
          bio(int = global::cache_large_max) noexcept;
          bio(ios*, int = global::cache_large_max) noexcept;
          bio(fragment*, ios*, int = global::cache_large_max) noexcept;
          bio(const bio&) noexcept;
          bio(bio&&) noexcept;
  virtual ~bio();

          void  set_source(ios*) noexcept;
  
  virtual int   seek(int, int) noexcept override;

          void  lock() noexcept;
          void  save(int&) noexcept;

  virtual int   get_char() noexcept override;
  virtual unsigned int  get_byte() noexcept override;
          char* get_line() noexcept;
          char* get_line(int&) noexcept;
  
          char* read() noexcept;
  virtual int   read(int) noexcept override;
  virtual int   read(int, char*) noexcept override;

  virtual int   put_char(char) noexcept override;
  virtual int   put_byte(unsigned char) noexcept override;
  virtual int   write(int, const char*) noexcept override;

  template<typename Xt>
  inline  int  write(int count, const Xt* data) noexcept {
          return write(count, reinterpret_cast<const char*>(data));
  }

          void  flush() noexcept;
          void  restore(int&) noexcept;
          void  commit() noexcept;
          void  unlock() noexcept;

          char* get_data() noexcept;

          ios*  get_io() noexcept;
          void  set_io(ios*) noexcept;

  virtual int   get_size() noexcept override;

  virtual bool  is_seekable() const noexcept override;
  virtual bool  is_readable() const noexcept override;
  virtual bool  is_writable() const noexcept override;
          bool  is_locked() const noexcept;

          bool  reserve(int) noexcept;
          void  reset(bool = true) noexcept;
          void  release() noexcept;

          int   get_capacity() const noexcept;

          operator bool() const noexcept;

          bio& operator=(const bio&) noexcept;
          bio& operator=(bio&&) noexcept;
};
/*namespace sys*/ }
#endif
