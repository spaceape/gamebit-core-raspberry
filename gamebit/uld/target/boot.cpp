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
#include "boot.h"
#include "hardware/regs/addressmap.h"
#include "pico/binary_info/defs.h"

namespace uld {

/* rom_size
   Raspberry Pico flash size, 2MB
*/
      constexpr std::size_t rom_size = 2 * 1024 * 1024;

      boot::boot()
{
}

      boot::~boot()
{
}

rtl_address_t boot::map(std::uint8_t, std::uint32_t addr, std::uint32_t size, std::uint32_t) noexcept
{
      rtl_address_t l_result;
      l_result.base = rsa_base_undef;
      l_result.data = nullptr;
      if(addr) {
          if((addr >= XIP_MAIN_BASE) &&
              (addr + size <= XIP_MAIN_BASE + rom_size)) {
              l_result.base = addr;
              l_result.data = reinterpret_cast<std::uint8_t*>(addr + (XIP_SRAM_BASE - XIP_MAIN_BASE));
          }
      }
      return l_result;
}

rtl_address_t boot::unmap(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept
{
      rtl_address_t l_result;
      l_result.base = rsa_base_undef;
      l_result.data = nullptr;
      return l_result;
}

/*namespace uld*/ }
