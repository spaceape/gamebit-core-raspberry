#ifndef uld_object_h
#define uld_object_h
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
#include <elf.h>
#include <uld.h>
#include <sys/ios.h>
#include <traits.h>

namespace uld {

class object
{
  sys::ios*      m_io;
  long int       m_load_base;  // base offset in file
  unsigned int   m_class:8;    // ELF class: 32 vs 64 bit
  unsigned int   m_encoding:8; // object encoding
  std::uint16_t  m_type;       // object file type
  std::uint16_t  m_machine;    // object architecture
  std::uint32_t  m_flags;      // processor flags

  std::uint32_t  m_phoff;      // program header table file offset
  std::uint32_t	 m_phnum;      // program header table entry count
  std::uint32_t  m_shoff;      // section header table file offset
  std::uint32_t  m_shnum;      // section header table entry count
  std::uint32_t  m_shstrndx;	 // section header string table index
  std::uint32_t  m_entry;      // entry point virtual address

  std::size_t    m_phentsize;  // program header table entry size
  std::size_t    m_shentsize;  // section header table entry size

  // cache lines: there is a lot of cross referencing going on while processing ELF objects, having a set of a few cache
  // buffers pointing at different locations in the object file will bring a great performance benefit;
  // the number of buffers and their size are specified here:
  static constexpr int s_cache_lines = 4;
  static constexpr int s_cache_size = 120;
  static_assert(s_cache_size > sizeof(Elf64_Ehdr), "Cache size should be at least as large as an ELF header");

  public:
  /* strtab
     provides access to the object's string table
  */
  class strtab
  {
    object* m_object;

    public:
            strtab(object*) noexcept;
            strtab(const strtab&) noexcept;
            strtab(strtab&&) noexcept;
            ~strtab();
            strtab& operator=(const strtab&) noexcept;
            strtab& operator=(strtab&&) noexcept;
  };

  /* idxtab
     base class for ELF tables (header, symbol, relocation tables)
  */
  template<typename Xt>
  class idxtab
  {
    object*       m_object;
    long int      m_offset;
    long int      m_count;

    public:
    using   entry_type = typename std::remove_cv<Xt>::type;
    static  constexpr long int entry_size = sizeof(entry_type);

    public:
    inline  idxtab() noexcept:
            m_object(nullptr),
            m_offset(0),
            m_count(0) {
    }

    inline  idxtab(object* object, long int offset, long int count) noexcept:
            m_object(object),
            m_offset(offset),
            m_count(count) {
    }

    inline  idxtab(const idxtab& copy) noexcept:
            m_object(copy.m_object),
            m_offset(copy.m_offset),
            m_count(copy.m_count) {
    }

    inline  idxtab(idxtab&& copy) noexcept:
            m_object(copy.m_object),
            m_offset(copy.m_offset),
            m_count(copy.m_count) {
    }

    inline  ~idxtab() {
    }

    inline  entry_type* get_ptr(long int index) noexcept {
            if(index < m_count) {
                return reinterpret_cast<entry_type*>(m_object->bin_cache_read(m_offset + index * entry_size, entry_size));
            }
            return nullptr;
    }

    inline  long int get_count() const noexcept {
            return m_count;
    }

    inline  long int get_entry_size() const noexcept {
            return entry_size;
    }

    inline  void    reset(object* object, long int offset, long int count) noexcept {
            m_object = object;
            m_offset = offset;
            m_count = count;
    }

    inline  void    release() noexcept {
            m_object = nullptr;
            m_offset = 0;
            m_count = 0;
    }

    inline  idxtab& operator=(const idxtab& rhs) noexcept {
            m_object = rhs.m_object;
            m_offset = rhs.m_offset;
            m_count = rhs.m_count;
            return *this;
    }

    inline  idxtab& operator=(idxtab&& rhs) noexcept {
            m_object = rhs.m_object;
            m_offset = rhs.m_offset;
            m_count = rhs.m_count;
            rhs.release();
            return *this;
    }
  };

  class phdr: public idxtab<Elf32_Phdr>
  {
    public:
            phdr() noexcept;
            phdr(object*, long int, long int) noexcept;
            phdr(const phdr&) noexcept;
            phdr(phdr&&) noexcept;
            ~phdr();
            phdr& operator=(const phdr&) noexcept;
            phdr& operator=(phdr&&) noexcept;
  };

  class shdr: public idxtab<Elf32_Shdr>
  {
    public:
            shdr(object*, long int, long int) noexcept;
            shdr(const shdr&) noexcept;
            shdr(shdr&&) noexcept;
            ~shdr();
            shdr& operator=(const shdr&) noexcept;
            shdr& operator=(shdr&&) noexcept;
  };

  class symtab
  {
  };

  class reltab
  {
  };

  struct {
    std::int32_t  m_offset;
    std::int32_t  m_size;
    char*         m_data;
  } m_cache[s_cache_lines];

  private:
          void    bin_cache_init() noexcept;
          bool    bin_cache_acquire(long int) noexcept;
          char*   bin_cache_read(long int, std::size_t) noexcept;
          bool    bin_cache_copy(std::uint8_t*, long int, std::size_t) noexcept;
          void    bin_cache_free() noexcept;

  public:
          object(sys::ios*, long int = 0) noexcept;
          object(const object&) noexcept;
          object(object&&) noexcept;
          ~object();
          void    assign(const object&) noexcept;
          bool    load(sys::ios*, long int = 0) noexcept;
          phdr    get_program_header() noexcept;
          shdr    get_section_header() noexcept;
          strtab  get_strtab() noexcept;
          symtab  get_symtab() noexcept;
          bool    copy(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept;
          bool    reset() noexcept;
          void    release() noexcept;
          bool    is_class(unsigned int) const noexcept;
          unsigned int get_class() const noexcept;
          bool    is_machine(unsigned int) const noexcept;
          unsigned int get_machine() const noexcept;
          bool    is_lsb() const noexcept;
          bool    is_msb() const noexcept;
          operator bool() const noexcept;
          object& operator=(const object&) noexcept;
          object& operator=(object&&) noexcept;
};

/*namespace uld*/ }
#endif
