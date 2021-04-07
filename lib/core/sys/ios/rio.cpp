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
#include "rio.h"

namespace sys {

      rio::rio() noexcept:
      ios(),
      m_io(nullptr)
{
}

      rio::rio(sys::ios* io) noexcept:
      ios(),
      m_io(io)
{
}

      rio::rio(const rio& copy) noexcept:
      ios(copy),
      m_io(copy.m_io)
{
}

      rio::rio(rio&& copy) noexcept:
      ios(std::move(copy)),
      m_io(copy.m_io)
{
      copy.release();
}

      rio::~rio()
{
      reset(false);
}

void  rio::set_source(ios* io) noexcept
{
      if(m_io != io) {
          m_io = io;
      }
}

int   rio::get_char() noexcept
{
      return m_io->get_char();
}

unsigned int rio::get_byte() noexcept
{
      return m_io->get_byte();
}

int   rio::seek(int offset, int whence) noexcept
{
      return m_io->seek(offset, whence);
}

int   rio::read(std::size_t count) noexcept
{
      return m_io->read(count);
}

int   rio::read(std::size_t count, char* memory) noexcept
{
      return m_io->read(count, memory);
}

int   rio::put_char(char value) noexcept
{
      return m_io->put_char(value);
}

int   rio::put_byte(unsigned char value) noexcept
{
      return m_io->put_char(value);
}

int   rio::write(std::size_t size, const char* data) noexcept
{
      return m_io->write(size, data);
}

int   rio::get_size() noexcept
{
      return m_io->get_size();
}

bool  rio::is_seekable() const noexcept
{
      return m_io->is_seekable();
}

bool  rio::is_readable() const noexcept
{
      return m_io->is_readable();
}

bool  rio::is_writable() const noexcept
{
      return m_io->is_writable();
}

void  rio::reset(bool clear) noexcept
{
      if(clear) {
          release();
      }
}

void  rio::release() noexcept
{
      m_io   = nullptr;
}

      rio::operator sys::ios*() noexcept
{
      return this;
}

      rio::operator bool() const noexcept
{
      return m_io;
}

rio& rio::operator=(const rio& rhs) noexcept
{
      m_io = rhs.m_io;
      return *this;
}

rio& rio::operator=(rio&& rhs) noexcept
{
      m_io = rhs.m_io;
      rhs.release();
      return *this;
}

/*namespace sys*/ }

