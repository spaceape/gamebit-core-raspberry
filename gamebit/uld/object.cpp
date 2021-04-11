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
#include "object.h"

namespace uld {

      object::object(sys::ios* io, long int base_offset) noexcept:
      m_io(nullptr),
      m_load_base(0),
      m_class(0)
{
      bin_cache_init();
      load(io, base_offset);
}

      object::object(const object& copy) noexcept:
      m_io(copy.m_io),
      m_class(copy.m_class)
{
      bin_cache_init();
}

      object::object(object&& copy) noexcept:
      m_io(copy.m_io),
      m_class(copy.m_class)
{
      bin_cache_init();
}

      object::~object()
{
      bin_cache_free();
}

      object::strtab::strtab(object*) noexcept
{
}

      object::strtab::strtab(const strtab&) noexcept
{
}

      object::strtab::strtab(strtab&&) noexcept
{
}

      object::strtab::~strtab() noexcept
{
}

object::strtab& object::strtab::operator=(const strtab&) noexcept
{
      return *this;
}

object::strtab& object::strtab::operator=(strtab&&) noexcept
{
      return *this;
}

      object::shdr::shdr(object* object, long int offset, long int count) noexcept:
      idxtab(object, offset, count)
{
}

      object::shdr::shdr(const shdr& copy) noexcept:
      idxtab(copy)
{
}

      object::shdr::shdr(shdr&& copy) noexcept:
      idxtab(std::move(copy))
{
}

      object::shdr::~shdr()
{
}

object::shdr& object::shdr::operator=(const shdr& rhs) noexcept
{
      idxtab::operator=(rhs);
      return *this;
}

object::shdr& object::shdr::operator=(shdr&& rhs) noexcept
{
      idxtab::operator=(std::move(rhs));
      return *this;
}

/* bin_cache_init()
   initialise all cache lines (without allocating them any memory yet)
*/
void  object::bin_cache_init() noexcept
{
      for(int l_index = 0; l_index < s_cache_lines; l_index++) {
          m_cache[l_index].m_offset = 0;
          m_cache[l_index].m_size = 0;
          m_cache[l_index].m_data = nullptr;
      }
}

/* bin_cache_acquire()
   obtain a free cache line or just the least used one
*/
bool  object::bin_cache_acquire(long int offset) noexcept
{
      int l_index = 0;
      // iterate through the cache records and find either an empty or the least used cache line,
      // stored at the last position
      while(l_index < s_cache_lines - 1) {
          if(m_cache[l_index].m_data) {
              if(m_cache[l_index].m_offset <= offset) {
                  if(m_cache[l_index].m_offset + m_cache[l_index].m_size >= offset) {
                      break;
                  }
              }
          } else
              break;
          l_index++;
      }
      // place the found cache line at the beginning of the list, move the others towards the end
      int l_found = l_index;
      if(l_found > 0) {
          auto  l_line = m_cache[l_found];
          while(l_index > 0) {
              if(m_cache[l_index - 1].m_data) {
                  m_cache[l_index] = m_cache[l_index - 1];
              } else
                  m_cache[l_index].m_data = nullptr;
              l_index--;
          }
          m_cache[0]  = l_line;
      }
      // prepare the found cache line
      if(m_cache[0].m_data == nullptr) {
          m_cache[0].m_data = static_cast<char*>(malloc(s_cache_size));
      }
      return m_cache[0].m_data;
}

/* bin_cache_read()
   read data into a cache line and return a pointer to read buffer; <size> indicates the minimum size we're
   interested in
*/
char* object::bin_cache_read(long int offset, std::size_t size) noexcept
{
      if(size) {
          if(size <= s_cache_size) {
              if(bin_cache_acquire(offset)) {
                  char*    l_read_ptr = m_cache[0].m_data;
                  long int l_read_size = 0;
                  long int l_read_offset = m_cache[0].m_offset - offset;
                  long int l_min_size = size;
                  if(l_read_offset >= 0) {
                      if(l_read_offset < m_cache[0].m_size) {
                          l_read_ptr = m_cache[0].m_data + l_read_offset;
                          if(l_min_size <= m_cache[0].m_size - l_read_offset) {
                              l_read_size = l_min_size;
                          }
                      }
                  }
                  if(l_read_size == 0) {
                      if(m_io->seek(offset, SEEK_SET) >= 0) {
                          l_read_size = m_io->read(size, l_read_ptr);
                          if(l_read_size > 0) {
                              m_cache[0].m_offset = offset;
                              m_cache[0].m_size   = l_read_size;
                              m_cache[0].m_data   = l_read_ptr;
                              return l_read_ptr;
                          }
                      }
                  }
              }
          }
      }
      return nullptr;
}

void  object::bin_cache_free() noexcept
{
      for(int l_index = 0; l_index < s_cache_lines; l_index++) {
          if(m_cache[l_index].m_data) {
              free(m_cache[l_index].m_data);
              m_cache[l_index].m_size = 0;
              m_cache[l_index].m_data = nullptr;
          }
      }
}

void  object::assign(const object& other) noexcept
{
      if(std::addressof(other) != this) {
          reset();
          m_io = other.m_io;
          m_class = other.m_class;
      }
}

bool  object::load(sys::ios* io, long int base_offset) noexcept
{
      reset();
      m_io = io;
      m_load_base = base_offset;
      if(m_io) {
          char* l_elf_id = bin_cache_read(m_load_base, EI_NIDENT);
          if(l_elf_id) {
              m_encoding = l_elf_id[EI_DATA];
              if((l_elf_id[EI_MAG0] == ELFMAG0) &&
                  (l_elf_id[EI_MAG1] == ELFMAG1) &&
                  (l_elf_id[EI_MAG2] == ELFMAG2) &&
                  (l_elf_id[EI_MAG3] == ELFMAG3)) {
                  if(l_elf_id[EI_CLASS] == ELFCLASS32) {
                      auto l_elf_hdr = reinterpret_cast<Elf32_Ehdr*>(bin_cache_read(m_load_base, sizeof(Elf32_Ehdr)));
                      if(l_elf_hdr) {
                          if(l_elf_hdr->e_version == EV_CURRENT) {
                              m_type = l_elf_hdr->e_type;
                              m_machine = l_elf_hdr->e_machine;
                              m_flags = l_elf_hdr->e_flags;
                              m_phoff = l_elf_hdr->e_phoff;
                              m_phnum = l_elf_hdr->e_phnum;
                              m_shoff = l_elf_hdr->e_shoff;
                              m_shnum = l_elf_hdr->e_shnum;
                              m_shstrndx = l_elf_hdr->e_shstrndx;
                              m_entry = l_elf_hdr->e_entry;
                              m_phentsize = l_elf_hdr->e_phentsize;
                              m_shentsize = l_elf_hdr->e_shentsize;
                              m_class = ELFCLASS32;
                          }
                      }
                  } else
                  if(l_elf_id[EI_CLASS] == ELFCLASS64) {
                      // no ELF64 support yet
                  }
              }
          }
      }
      return m_class;
}

object::shdr   object::get_shdr() noexcept
{
      return shdr(this, m_load_base + m_shoff, m_shnum);
}

object::strtab object::get_strtab() noexcept
{
      return strtab(this);
}

object::symtab object::get_symtab() noexcept
{
      return symtab();
}

bool  object::reset() noexcept
{
      if(m_class) {
          bin_cache_free();
          m_class = 0;
      }
      return true;
}

void  object::release() noexcept
{
      bin_cache_free();
}

bool  object::is_class(unsigned int value) const noexcept
{
      return m_class == value;
}

unsigned int object::get_class() const noexcept
{
      return m_class;
}

bool  object::is_machine(unsigned int value) const noexcept
{
      return m_machine == value;
}

unsigned int object::get_machine() const noexcept
{
      return m_machine;
}

bool  object::is_lsb() const noexcept
{
      return m_encoding == ELFDATA2LSB;
}

bool  object::is_msb() const noexcept
{
      return m_encoding == ELFDATA2MSB;
}

      object::operator bool() const noexcept
{
      return m_class;
}

object& object::operator=(const object& rhs) noexcept
{
      assign(rhs);
      return *this;
}

object& object::operator=(object&& rhs) noexcept
{
      assign(rhs);
      rhs.release();
      return *this;
}

/*namespace uld*/ }