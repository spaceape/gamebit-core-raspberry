#ifndef uld_elf32_h
#define uld_elf32_h
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
#include <uld.h>
#include "target.h"

namespace uld {
namespace elf32 {

/* linker
*/
class linker
{
  protected:
  static constexpr unsigned int opt_allow_reloc = 1u;

  protected:
  segment_table_t m_segment_table;
  symbol_table_t  m_symbol_table;

  protected:
          bool elf_import(object&, target&, unsigned int) noexcept;
          bool elf_discard() noexcept;

  public:
          linker() noexcept;
          linker(const linker&) noexcept = delete;
          linker(linker&&) noexcept = delete;
          ~linker();
          linker& operator=(const linker&) noexcept = delete;
          linker& operator=(linker&&) noexcept = delete;
};

/*
*/
class loader: public linker
{
  public:
          loader() noexcept;
          loader(const loader&) noexcept = delete;
          loader(loader&&) noexcept = delete;
          ~loader();
          bool    load(const char*, target&, unsigned int) noexcept;
          loader& operator=(const loader&) noexcept = delete;
          loader& operator=(loader&&) noexcept = delete;
};

// /* base class for runtime environment setup
// */
// class linker
// {
//   public:
//   using address_t      = ems_address_t;
//   using segment_t      = ems_segment_t;
//   using symbol_t       = ems_symbol_t;

//   private:
//   using segment_page_t = std::vector<segment_t>;
//   using segment_list_t = std::vector<segment_page_t>;
//   using symbol_page_t  = std::vector<symbol_t>;
//   using symbol_list_t  = std::vector<symbol_page_t>;
//   using symbol_dict_t  = std::vector<symbol_t*>;

//   segment_list_t  m_memory_list;
//   symbol_list_t   m_symbol_list;
//   symbol_dict_t   m_symbol_map;
//   short int       m_memory_page;
//   short int       m_memory_index;
//   short int       m_symbol_page;
//   short int       m_symbol_index;

//   private:
//           void          seg_make() noexcept;
          
//           bool            rtl_is_mapped(std::uint8_t, std::uint32_t) const noexcept;
//           segment_page_t& rtl_get_page(int) noexcept;
//           rtl_segment_t&  rtl_get_segment(int, int) noexcept;
//           rtl_segment_t&  rtl_get_segment(segment_page_t&, int) noexcept;

//           // std::uint32_t ldr_seg_reserve(rtl_segment_t&) noexcept;
//           // std::uint32_t ldr_seg_dispose(rtl_segment_t&) noexcept;
//           rtl_address_t seg_reserve(std::uint8_t, std::uint32_t, std::uint32_t) noexcept;
//           rtl_address_t seg_dispose(std::uint8_t, std::uint32_t, std::uint32_t) noexcept;

//           void          seg_free() noexcept;

//           void  rtl_make_symbol() noexcept;
//           void  rtl_free_symbol() noexcept;

//   public:
//   static constexpr std::size_t memory_page_size = 1024;

//   // memory_page_max: maximum number of segment lists (pages) allowed
//   static constexpr int         memory_page_max = std::numeric_limits<int>::max();

//   // memory_segment_max: maximum number of segments allowed in a page;
//   // currently set such that each segment list reserves 1K of RAM
//   static constexpr int         memory_segm_max = 1024 / sizeof(rtl_segment_t);

//   protected:
//           bool  make_region(std::uint8_t, std::uint32_t) noexcept;
//           bool  drop_region(std::uint8_t) noexcept;
//           bool  import(object&) noexcept;
//           bool  reset() noexcept;
//           bool  error(unsigned int, const char*, ...) noexcept;

//   public:
//           linker() noexcept;
//           linker(const linker&) noexcept = delete;
//           linker(linker&&) noexcept = delete;
//           ~linker();
//           // bool get_error_bits() noexcept;
//           linker& operator=(const linker&) noexcept = delete;
//           linker& operator=(linker&&) noexcept = delete;
// };

/*namespace elf32*/ }
/*namespace uld*/ }
#endif
