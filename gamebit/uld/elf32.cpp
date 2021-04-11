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
#include "elf32.h"
#include "object.h"
#include <io.h>

namespace uld {
namespace elf32 {

/* linker
*/
      linker::linker() noexcept:
      m_segment_table(),
      m_symbol_table()
{
}

      linker::~linker()
{
}

bool  linker::elf_import(object& source, target& target, unsigned int options) noexcept
{
      int  l_fail = 0;
      auto l_shdr = source.get_shdr();
      for(int l_sh_index = 0; l_sh_index < l_shdr.get_count(); l_sh_index++) {
          auto l_sh_ptr = l_shdr.get_ptr(l_sh_index);
          if(l_sh_ptr) {
              auto l_sh_type = l_sh_ptr->sh_type;
              auto l_sh_flags = l_sh_ptr->sh_flags;
              if(l_sh_type == SHT_PROGBITS) {
                  if(l_sh_flags & SHF_ALLOC) {
                      // auto l_xxx = rtl_reserve(l_sh_flags, l_sh_ptr->sh_size, l_sh_ptr->sh_addralign);
                      if(l_sh_flags & SHF_WRITE) {
                          // if(l_xxx != nullptr) {
                          //     // copy();
                          // } else
                          //     error(1, "failed to allocate space for section %d", l_sh_index, l_sh_ptr);
                      } else
                      if(l_sh_flags & SHF_EXECINSTR) {
                      }
                  }
                  if(l_sh_flags & SHF_INFO_LINK) {
                  }
              } else
              if(l_sh_type == SHT_NOBITS) {
                  if(l_sh_flags & SHF_ALLOC) {
                  }
              } else
              if(l_sh_type == SHT_REL) {
              } else
              if(l_sh_type == SHT_RELA) {
              }
          }
      }
      return false;
}

bool  linker::elf_discard() noexcept
{
      return false;
}

/* loader
*/
      loader::loader() noexcept:
      linker()
{
}

      loader::~loader()
{
}

bool  loader::load(const char* file, target& target, unsigned int options) noexcept
{
      sys::fio l_file = io::open(file, O_RDONLY);
      if(l_file) {
          object l_object(l_file);
          if(l_object) {
              if(l_object.is_class(ELFCLASS32) &&
                  l_object.is_machine(EM_ARM) &&
                  l_object.is_lsb()) {
                  return elf_import(l_object, target, options);
              }
          }
      }
      return false;
}

//       linker::linker() noexcept:
//       m_memory_page(0),
//       m_memory_index(0),
//       m_symbol_page(0),
//       m_symbol_index(0)
// {
// }

//       linker::~linker()
// {
// }

// /* rtl_is_mapped()
//    check if the given virtual address is mapped anywhere in our segment list
// */
// bool  linker::rtl_is_mapped(std::uint8_t type, std::uint32_t address) const noexcept
// {
//      int  l_page_index = 0;
//       while(l_page_index < static_cast<int>(m_memory_list.size())) {
//           auto& l_page = m_memory_list[l_page_index];
//           int   l_segm_index = 0;
//           while(l_segm_index < static_cast<int>(m_memory_list.size())) {
//               auto& l_segm = l_page[l_segm_index];
//               if(l_segm.m_type) {
//                   if(l_segm.m_type & type) {
//                       if((address >= l_segm.m_address.m_base) &&
//                           (address < l_segm.m_address.m_base + memory_page_size)) {
//                           return true;
//                       }
//                   }
//               }
//               l_segm_index++;
//           }
//           l_page_index++;
//       }
//       return false;
// }

// segment_page_t& linker::rtl_get_page(int index) noexcept
// {
//       if(index >= static_cast<int>(m_memory_list.size())) {
//           m_memory_list.emplace_back().reserve(memory_segm_max);
//       }
//       return m_memory_list[index];
// }

// rtl_segment_t&  linker::rtl_get_segment(segment_page_t& page, int index) noexcept
// {
//       if(index >= static_cast<int>(page.size())) {
//           auto& l_segm = page.emplace_back();
//           l_segm.m_type = 0;
//         //l_segm.m_used = 0;
//         //l_segm.m_address.m_base = 0;
//         //l_segm.m_address.m_data = nullptr;
//       }
//       return page[index];
// }



// rtl_address_t linker::seg_reserve(std::uint8_t type, std::uint32_t size, std::uint32_t alignment) noexcept
// {
//       rtl_address_t l_result;
//       if(size) {
//           if(m_memory_list.size() != 0) {
//               int  l_page_index = m_memory_page;
//               while(l_page_index < std::numeric_limits<short int>::max()) {
//                   int  l_segment_index = m_memory_index;
//                   while(l_segment_index < std::numeric_limits<short int>::max()) {
//                       auto& l_segment    = m_memory_list[l_page_index][l_segment_index];
//                       int   l_used_bytes = l_segment.m_used;
//                       int   l_free_bytes = memory_page_size - l_used_bytes;
//                       if((l_used_bytes == 0) ||
//                           (l_segment.m_type & stl_func_flags) == (type & stl_func_flags)) {
//                           if(l_free_bytes > size) {
//                               if(l_used_bytes == 0) {
//                                   l_segment.m_address.m_data = reinterpret_cast<std::uint8_t*>(malloc(memory_page_size));
//                                   if(l_segment.m_address.m_data) {
//                                       l_result.m_address = l_segment.m_address.m_address + l_used_bytes;
//                                       l_result.m_data    = l_segment.m_address.m_data + l_used_bytes;
//                                   } else
//                                       elf_error(1, "failed to allocate space for segment %.4x:%.4x", l_page_index, l_segment_index);
//                                   break;
//                               }
//                               break;
//                           } else
//                           if(l_free_bytes < memory_page_size / 64) {
//                               m_memory_page = l_page_index;
//                               m_memory_index = l_segment_index;
//                           }
//                       }
//                       l_segment_index++;
//                   }
//                   l_page_index++;
//               }
//           } else
//           if(m_memory_list.size() == 0) {
//               m_memory_list.reserve(256 / sizeof(segment_page_t));
//               m_memory_list.emplace_back(1024 / sizeof(rtl_segment_t));
//               return seg_reserve(type, size, alignment);
//           }
//       }
//       return l_result;
// }

// rtl_address_t linker::rtl_dispose(std::uint8_t, std::uint32_t, std::uint32_t) noexcept
// {
//       return rtl_address_t();
// }

// /* make_region()
//    create a memory region for the given rtl_type_* and set its base address
// */
// bool  linker::make_region(std::uint8_t type, std::uint32_t base) noexcept
// {
//       int  i_page = m_memory_page;
//       while(i_page < memory_page_max) {
//           int   i_segment = m_memory_index;
//           auto& l_page = rtl_get_page(i_page);
//           while(i_segment < memory_segm_max) {
//               auto& l_segment = rtl_get_segment(l_page, i_segment);
//               if((l_segment.m_type == 0) ||
//                   (l_segmment.m_used == 0)) {
//                   l_segment.m_type = type;
//                   l_segment.m_used = 0
//                   l_segment.m_address.m_base = base;
//                   l_segment.m_address.m_data = nullptr;
//               }
//               i_segment++;
//           }
//           i_page++;
//       }
//       return false;
// }

// /* drop_region()
// */
// bool  linker::drop_region(std::uint8_t) noexcept
// {
// }

// bool  linker::import(object& bin) noexcept
// {
//       int  l_fail = 0;
//       auto l_shdr = bin.get_shdr();
//       for(int l_sh_index = 0; l_sh_index < l_shdr.get_count(); l_sh_index++) {
//           auto l_sh_ptr = l_shdr.get_ptr(l_sh_index);
//           if(l_sh_ptr) {
//               auto l_sh_type = l_sh_ptr->sh_type;
//               auto l_sh_flags = l_sh_ptr->sh_flags;
//               if(l_sh_type == SHT_PROGBITS) {
//                   if(l_sh_flags & SHF_ALLOC) {
//                       auto l_xxx = rtl_reserve(l_sh_flags, l_sh_ptr->sh_size, l_sh_ptr->sh_addralign);
//                       if(l_sh_flags & SHF_WRITE) {
//                           if(l_xxx != nullptr) {
//                               // copy();
//                           } else
//                               error(1, "failed to allocate space for section %d", l_sh_index, l_sh_ptr);
//                       } else
//                       if(l_sh_flags & SHF_EXECINSTR) {
//                       }
//                   }
//                   if(l_sh_flags & SHF_INFO_LINK) {
//                   }
//               } else
//               if(l_sh_type == SHT_NOBITS) {
//                   if(l_sh_flags & SHF_ALLOC) {
//                   }
//               } else
//               if(l_sh_type == SHT_REL) {
//               } else
//               if(l_sh_type == SHT_RELA) {
//               }
//           }
//       }
//       return false;
// }

// bool  linker::error(unsigned int, const char*, ...) noexcept
// {
//       return false;
// }

/*namespace elf32*/ }
/*namespace uld*/ }
