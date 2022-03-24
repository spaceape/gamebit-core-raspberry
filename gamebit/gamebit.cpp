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
#include "gamebit.h"
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <dev/bd.h>
#include <dev/bd/spio.h>
#include <dev/bd/drive.h>
#include <dev/bd/fat/part.h>
#include <config.h>
#include <version.h>

static unsigned int s_init;

namespace gamebit {

bool  initialise(unsigned int flags) noexcept
{
      unsigned int l_init = flags & (~s_init);
      if(l_init & if_power_hard) {
          gpio_set_dir(23, GPIO_OUT);
          gpio_put(23, 1);
      } else
      if(l_init & if_stdio_usb) {
          stdio_init_all();
      } else
      if(l_init & if_stdio_uart) {
          stdio_uart_init();
      }
      if(l_init & if_filesystem) {
          sd0 = new(std::nothrow) dev::spio();
          if(sd0) {
              part0 = new(std::nothrow) dev::fat::part(sd0, 0);
              if(part0) {
                  s_init |= if_filesystem;
              }
          }
      }
      return true;
}

sys::fio open(const char* name, long int mode, long int permissions) noexcept
{
      return sys::fio(part0, name, mode, permissions);
}

sys::fio close(sys::fio&) noexcept
{
      return sys::fio();
}

dev::drive*  get_default_disk() noexcept
{
      return part0;
}

dev::drive*  get_default_cache() noexcept
{
      return cache;
}

bool  dispose(unsigned int flags) noexcept
{
      unsigned int l_free = flags & s_init;
      if(l_free & if_filesystem) {
          if(part0) {
              delete part0;
              part0 = nullptr;
          }
          if(sd0) {
              delete sd0;
              sd0  = nullptr;
          }
          s_init ^= if_filesystem;
      }
      return true;
}

/*namespace gamebit*/ }
