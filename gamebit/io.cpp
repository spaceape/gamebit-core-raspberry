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
#include "io.h"
#include "gamebit.h"
#include <dev/spio.h>
#include <sys/fat/part.h>
#include "hardware/spi.h"

static io*  s_io_instance;
static int  s_io_hooks    = 0;

      io::io() noexcept

{
      // initialise the filesystem
      if(s_io_hooks == 0) {
          sd0 = new(std::nothrow) dev::spio(spi1);
          if(sd0) {
              part0 = new(std::nothrow) sys::fat::part(sd0, 0);
              if(part0) {
              }
          }
          s_io_instance = this;
      }
      s_io_hooks++;
}

      io::io(const io&) noexcept
{
}

      io::io(io&&) noexcept
{
}

      io::~io() noexcept
{
      s_io_hooks--;
      if(s_io_hooks == 0) {
          if(part0) {
              delete part0;
              part0 = nullptr;
          }
          if(sd0) {
              delete sd0;
              sd0  = nullptr;
          }
          s_io_instance = nullptr;
      }
}

sys::fio io::open(const char* name, long int mode, long int permissions) noexcept
{
      return sys::fio(part0, name, mode, permissions);
}

sys::fio io::close(sys::fio&) noexcept
{
      return sys::fio();
}

sys::drive* io::get_default_drive() noexcept
{
      return part0;
}

io&   io::operator=(const io&) noexcept
{
      return *this;
}

io&   io::operator=(io&&) noexcept
{
      return *this;
}
