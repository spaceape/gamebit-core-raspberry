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
#include <gamebit.h>

namespace uld {
namespace elf32 {

/* linker
*/
      linker::linker() noexcept
{
}

      linker::~linker()
{
}

bool  linker::elf_import(object&, target&, unsigned int) noexcept
{
      return false;
}

bool  linker::elf_discard() noexcept
{
      return false;
}

bool  linker::elf_error(...) noexcept
{
      return false;
}

/* loader
*/
      loader::loader() noexcept
{
}

      loader::~loader()
{
}

bool  loader::elf_import(object& source, target& target, unsigned int) noexcept
{
      auto l_shdr = source.get_section_header();
      int  l_scnt = l_shdr.get_count();
      for(int l_sh_index = 0; l_sh_index < l_scnt; l_sh_index++) {
          auto l_sh_ptr = l_shdr.get_ptr(l_sh_index);
          if(l_sh_ptr) {
              auto l_sh_type = l_sh_ptr->sh_type;
              auto l_sh_flags = l_sh_ptr->sh_flags;
              if(l_sh_type == SHT_PROGBITS) {
                  if(l_sh_flags & SHF_INFO_LINK) {
                      return elf_error(255, "Relocation not supported for section %d", l_sh_index);
                  }
                  if(l_sh_flags & SHF_ALLOC) {
                      rtl_fragment_t l_fragment;
                      if(l_sh_flags & SHF_WRITE) {
                          l_fragment = target.map(rsa_mode_rwa, l_sh_ptr->sh_addr, l_sh_ptr->sh_size, l_sh_ptr->sh_addralign);
                          if(l_fragment.base != rsa_base_undef) {
                              if(l_fragment.action & rsa_act_keep) {
                                  source.copy(l_fragment.data, l_sh_ptr->sh_offset, l_sh_ptr->sh_size);
                              }
                          }
                      } else
                      if(l_sh_flags & SHF_EXECINSTR) {
                          l_fragment = target.map(rsa_mode_rxa, l_sh_ptr->sh_addr, l_sh_ptr->sh_size, l_sh_ptr->sh_addralign);
                          if(l_fragment.base != rsa_base_undef) {
                              if(l_fragment.action & rsa_act_keep) {
                                  source.copy(l_fragment.data, l_sh_ptr->sh_offset, l_sh_ptr->sh_size);
                              }
                          }
                      } else
                      if(l_sh_flags < SHF_MERGE) {
                          l_fragment = target.map(rsa_mode_ra, l_sh_ptr->sh_addr, l_sh_ptr->sh_size, l_sh_ptr->sh_addralign);
                          if(l_fragment.base != rsa_base_undef) {
                              if(l_fragment.action & rsa_act_keep) {
                                  source.copy(l_fragment.data, l_sh_ptr->sh_offset, l_sh_ptr->sh_size);
                              }
                          }
                      }
                  }
              }
          } else
              return elf_error(255, "I/O error");
      }
      return true;
}

bool  loader::elf_discard() noexcept
{
      return false;
}

bool  loader::elf_error(...) noexcept
{
      return false;
}

bool  loader::load(const char* file, target& target, unsigned int options) noexcept
{
      sys::fio l_file = gamebit::open(file, O_RDONLY);
      if(l_file) {
          object l_object(l_file);
          if(l_object) {
              if(l_object.is_class(ELFCLASS32) &&
                  l_object.is_machine(EM_ARM) &&
                  l_object.is_lsb()) {
                  if(elf_import(l_object, target, options)) {
                      return true;
                  }
                  target.reset();
              }
          }
      }
      return false;
}

/*namespace elf32*/ }
/*namespace uld*/ }
