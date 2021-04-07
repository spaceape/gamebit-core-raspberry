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
#include "fio.h"
#include <sys/fs/raw/drive.h>

namespace sys {

      fio::fio() noexcept:
      m_drive(nullptr),
      m_mode(0),
      m_address(0),
      m_offset(-1),
      m_size(0)
{
}

      fio::fio(sys::drive* drive, const char* name, long int mode, long int permissions) noexcept:
      fio()
{
      sys::fsi_t l_index;
      if(drive) {
          l_index = drive->get_fsi(name, mode, permissions);
          if(l_index.m_address) {
              //xxx
              m_drive = drive;
              m_mode = mode & 0x0000ffff;
              m_address = l_index.m_address;
              m_offset = 0;
              m_size = l_index.m_size;
          }
      }
}

      fio::fio(const fio& copy) noexcept:
      fio()
{
      assign(copy);
}

      fio::fio(fio&& copy) noexcept:
      fio()
{
      assign(std::move(copy));
}

      fio::~fio()
{
      close(false);
}

void  fio::assign(const fio& copy) noexcept
{
      if(this != std::addressof(copy)) {
          close();
          if(copy.m_drive) {
              m_drive = copy.m_drive;
              m_mode = copy.m_mode;
              m_address = copy.m_address;
              m_offset = copy.m_offset;
              m_size = copy.m_size;
          } else
              close(true);
      }
}

void  fio::assign(fio&& copy) noexcept
{
      if(this != std::addressof(copy)) {
          close();
          if(copy.m_drive) {
              m_drive = copy.m_drive;
              m_mode = copy.m_mode;
              m_address = copy.m_address;
              m_offset = copy.m_offset;
              m_size = copy.m_size;
              copy.release();
          } else
              close(true);
      }
}

int  fio::get_char() noexcept
{
      // really inefficient, avoid using
      std::uint8_t* l_data;
      if(m_drive) {
          if(m_offset < m_size - 1) {
              if(m_drive->get_raw(l_data, m_address, m_offset, 1u)) {
                  m_offset++;
                  return static_cast<char>(l_data[0]);
              } else
                  return EOF;
          } else
              return EOF;
      } else
          return EOF;
}

unsigned int fio::get_byte() noexcept
{
      // just like get_char() - really inefficient, avoid using
      std::uint8_t* l_data;
      if(m_drive) {
          if(m_offset < m_size - 1) {
              if(m_drive->get_raw(l_data, m_address, m_offset, 1u)) {
                  m_offset++;
                  return l_data[0];
              } else
                  return EOF;
          } else
              return EOF;
      } else
          return EOF;
}

int   fio::seek(int offset, int whence) noexcept
{
      long int l_offset;
      if(whence == SEEK_SET) {
          l_offset = offset;
      } else
      if(whence == SEEK_CUR) {
          if(m_offset > 0) {
              if(offset > std::numeric_limits<int>::max() - m_offset) {
                  return -1;
              }
          } else
          if(m_offset < 0) {
              if(offset < std::numeric_limits<int>::min() - m_offset) {
                  return -1;
              }
          }
          l_offset = m_offset + offset;
      } else
      if(whence == SEEK_END) {
          if(offset > std::numeric_limits<int>::max() - m_size) {
              return -1;
          }
          l_offset = m_size + offset;
      } else
          return -1;

      if(l_offset < 0) {
          m_offset = 0;
      } else
      if(l_offset > m_size - 1) {
          m_offset = m_size - 1;
      } else
          m_offset = l_offset;
      return m_offset;
}

int   fio::read(std::size_t count) noexcept
{
      return seek(count, SEEK_CUR);
}

int   fio::read(std::size_t count, char* memory) noexcept
{
      char*         l_copy_ptr = memory;
      std::uint8_t* l_read_ptr;
      std::size_t   l_read_left;
      std::size_t   l_read_size;
      std::size_t   l_copy_size = 0;
      if(m_drive) {
          if(m_offset < m_size - 1) {
              // compute maximum read size and correct if it exceeds the file boundaries
              if(m_offset + count <= m_size) {
                  l_read_left = count;
              } else
                  l_read_left = m_size - m_offset;
              // read data and copy into the memory buffer
              while(l_read_left) {
                  l_read_size = m_drive->get_raw(l_read_ptr, m_address, m_offset, l_read_left);
                  if(l_read_size) {
                      std::memcpy(l_copy_ptr, l_read_ptr, l_read_size);
                      l_copy_ptr += l_read_size;
                      l_copy_size += l_read_size;
                      m_offset += l_read_size;
                      if(l_read_size < l_read_left) {
                          l_read_left -= l_read_size;
                      } else
                          l_read_left = 0;
                  } else
                      return 0;
              }
              return l_copy_size;
          }
      }
      return 0;
}

int   fio::put_char(char) noexcept
{
      return 0;
}

int   fio::put_byte(unsigned char) noexcept
{
      return 0;
}

int   fio::write(std::size_t, const char*) noexcept
{
      return 0;
}

int   fio::get_size() noexcept
{
      return m_size;
}

bool  fio::set_blocking(bool) noexcept
{
      return true;
}

bool  fio::set_nonblocking() noexcept
{
      return false;
}

bool  fio::is_seekable() const noexcept
{
      return true;
}

bool  fio::is_readable() const noexcept
{
      return true;
}

bool  fio::is_writable() const noexcept
{
      return (m_mode & O_WRONLY) || (m_mode & O_RDWR);
}

void  fio::reset() noexcept
{
      close(true);
}

void  fio::release() noexcept
{
      m_drive = nullptr;
      m_mode = 0;
      m_address = 0;
      m_offset = -1;
      m_size = 0;
}

void  fio::close(bool reset) noexcept
{
      if(m_drive) {
          m_drive = nullptr;
          if(reset) {
              m_mode = 0;
              m_address = 0;
              m_offset = -1;
              m_size = 0;
          }
      }
}

      fio::operator sys::ios*() noexcept
{
      return this;
}

      fio::operator bool() const noexcept
{
      return m_drive;
}

fio&  fio::operator=(const fio& rhs) noexcept
{
      assign(rhs);
      return *this;
}

fio&  fio::operator=(fio&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}

/*namespace sys*/ }
