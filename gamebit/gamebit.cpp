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
#include <config.h>
#include <version.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"

void  help() noexcept
{
}

bool  gamebit_initialise(unsigned int) noexcept
{
      return true;
}


bool  gamebit_dispose(unsigned int) noexcept
{
      return true;
}

bool  gamebit_init_all() noexcept
{
      return gamebit_initialise(0xffffffff);
}

void  panic() noexcept
{
      // go, blinky!
      // hard reset to go out of this routine
      gpio_init(PICO_DEFAULT_LED_PIN);
      gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
      while(true) {
          gpio_put(PICO_DEFAULT_LED_PIN, 1);
          sleep_ms(250);
          gpio_put(PICO_DEFAULT_LED_PIN, 0);
          sleep_ms(250);
      }
}

bool  post() noexcept;

int   main(int, char**)
{
      stdio_init_all();
      printf("gamebit v0.0.1\n");
      if(post()) {
          io  l_filesystem;
      }
      // normally not reached
      printf("Bye.\n");
      panic();
      return 0;
}
