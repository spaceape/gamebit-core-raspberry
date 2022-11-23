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
#include "blackops.h"
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/stdio_uart.h>

#include "hardware.h"
#include <sys.h>
#include <sys/adt.h>
#include <sys/adt/node.h>
#include <sys/adt/device.h>
#include <sys/adt/directory.h>
#include <sys/adt/tree.h>
#include "controller.h"

#include <config.h>
#include <version.h>

int   main()
{
      if(bool
          l_init_success = initialise();
          l_init_success == true) {
          if(auto
              l_stdio = dev::uart::get_from_adt(hw_controller_io);
              l_stdio != nullptr) {
              controller  l_controller(l_stdio);
              if(l_controller.resume()) {
                  absolute_time_t l_time_abs = get_absolute_time();
                  absolute_time_t l_time_prev;
                  while(true) {
                      l_time_prev = l_time_abs;
                      l_time_abs  = get_absolute_time();
                      l_controller.sync(l_time_prev);
                  }
                  return true;
              }
          }
      }
      return false;

      dispose();
      // not normally reached
      panic();
      return 0;
}
