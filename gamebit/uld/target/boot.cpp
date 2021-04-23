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
#include <hardware/regs/xip.h>
#include <hardware/regs/addressmap.h>
#include <hardware/watchdog.h>
#include <hardware/structs/xip_ctrl.h>
#include <hardware/structs/psm.h>
#include <hardware/structs/watchdog.h>
#include <cstring>

// extern "C" std::uint32_t __stack;

namespace uld {

/* boot_size
   Raspberry Pico boot size, 512B
*/
      constexpr std::size_t boot_size = 512;

/* rom_size
   Raspberry Pico flash size, 2MB
*/
      constexpr std::size_t rom_size = 2 * 1024 * 1024;

      boot::boot():
      m_entry_point(0),
      m_xipctl_save(xip_ctrl_hw->ctrl)
{
      // disable XIP cache so we can write to it
      xip_ctrl_hw->ctrl = m_xipctl_save & ~XIP_CTRL_EN_RESET;
}

      boot::~boot()
{
      // restore the xip ctrl register
      xip_ctrl_hw->ctrl = m_xipctl_save;
}

rtl_fragment_t boot::map(std::uint8_t, std::uint32_t addr, std::uint32_t size, std::uint32_t) noexcept
{
      rtl_fragment_t l_result;
      l_result.action = rsa_act_none;
      l_result.base = rsa_base_undef;
      l_result.data = nullptr;
      if(addr) {
          if((addr >= XIP_MAIN_BASE) &&
              (addr + size <= XIP_MAIN_BASE + boot_size)) {
              l_result.action = rsa_act_none;
              l_result.base = addr;
              l_result.data = reinterpret_cast<std::uint8_t*>(addr + (XIP_SRAM_BASE - XIP_MAIN_BASE));
              // std::memset(l_result.data, 0, size);
          } else
          if(addr < XIP_NOALLOC_BASE) {
              l_result.action = rsa_act_keep;
              l_result.base = addr;
              l_result.data = reinterpret_cast<std::uint8_t*>(addr + (XIP_SRAM_BASE - XIP_MAIN_BASE));
          }
      }
      return l_result;
}

rtl_fragment_t boot::unmap(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept
{
      rtl_fragment_t l_result;
      l_result.base = rsa_base_undef;
      l_result.data = nullptr;
      return l_result;
}

void  boot::exec() noexcept
{
      // disable the watchdog, if running
      hw_clear_bits(std::addressof(watchdog_hw->ctrl), WATCHDOG_CTRL_ENABLE_BITS);

      hw_set_bits(
          std::addressof(watchdog_hw->ctrl),
          WATCHDOG_CTRL_PAUSE_DBG0_BITS | 
          WATCHDOG_CTRL_PAUSE_DBG1_BITS |
          WATCHDOG_CTRL_PAUSE_JTAG_BITS
      );

      // watchdog_hw->scratch[0] = 0;
      // watchdog_hw->scratch[1] = 0;
      // watchdog_hw->scratch[2] = 0;
      // watchdog_hw->scratch[3] = 0;
      watchdog_hw->scratch[4] = 0xb007c0d3;
      watchdog_hw->scratch[5] = 0x1500021b ^ -0xb007c0d3;
      watchdog_hw->scratch[6] = 0x20041000;
      watchdog_hw->scratch[7] = 0x1500021b;

      // // reset just about enough to make sure we boot into the image we've loaded and not flash
      // hw_clear_bits(std::addressof(psm_hw->wdsel), PSM_WDSEL_BITS);
      // hw_set_bits(std::addressof(psm_hw->wdsel),
      //     PSM_WDSEL_PROC0_BITS |
      //     PSM_WDSEL_PROC1_BITS
      // );

      // goodbye, world
      hw_set_bits(std::addressof(watchdog_hw->ctrl), WATCHDOG_CTRL_TRIGGER_BITS);
}

/*namespace uld*/ }
