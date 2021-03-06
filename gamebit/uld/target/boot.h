#ifndef uld_target_boot_h
#define uld_target_boot_h
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
#include <uld/target.h>
#include "runtime.h"

namespace uld {

/* boot
   Raspberry Pico boot target
*/
class boot: public target
{
  std::uint32_t    m_entry_point;
  std::uint32_t    m_xipctl_save;
  public:
          boot();
          boot(const boot&) noexcept = delete;
          boot(boot&&) noexcept = delete;
  virtual ~boot();
  virtual rtl_fragment_t map(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept override;
  virtual rtl_fragment_t unmap(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept override;
          void  exec() noexcept;
          boot& operator=(const boot&) noexcept = delete;
          boot& operator=(boot&&) noexcept = delete;
};

/*namespace uld*/ }
#endif
