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
  public:
  static constexpr unsigned int opt_erase = 1u;
  static constexpr unsigned int opt_reloc = 8u;
  static constexpr unsigned int opt_alloc_bss = 16u;

  protected:
          bool  elf_import(object&, target&, unsigned int) noexcept;
          bool  elf_discard() noexcept;
          bool  elf_error(...) noexcept;

  public:
          linker() noexcept;
          linker(const linker&) noexcept = delete;
          linker(linker&&) noexcept = delete;
          ~linker();
          linker& operator=(const linker&) noexcept = delete;
          linker& operator=(linker&&) noexcept = delete;
};

/* loader
*/
class loader
{
  protected:
          bool  elf_import(object&, target&, unsigned int) noexcept;
          bool  elf_discard() noexcept;
          bool  elf_error(...) noexcept;

  public:
          loader() noexcept;
          loader(const loader&) noexcept = delete;
          loader(loader&&) noexcept = delete;
          ~loader();
          bool    load(const char*, target&, unsigned int) noexcept;
          loader& operator=(const loader&) noexcept = delete;
          loader& operator=(loader&&) noexcept = delete;
};
/*namespace elf32*/ }
/*namespace uld*/ }
#endif
