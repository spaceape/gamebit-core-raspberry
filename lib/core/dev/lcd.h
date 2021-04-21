#ifndef dev_lcd_h
#define dev_lcd_h
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
#include <dev.h>

namespace dev {

/* lcd
   abstract base class for display devices
*/
class lcd
{
  public:
          lcd() noexcept;
          lcd(const lcd&) noexcept;
          lcd(lcd&&) noexcept;
  virtual ~lcd();
  virtual void  set_ras(std::uint16_t, std::uint16_t) noexcept = 0;
  virtual void  set_cas(std::uint16_t, std::uint16_t) noexcept = 0;
  virtual void  fill(std::uint16_t, long int) noexcept = 0;
  virtual void  fill(std::uint16_t*, long int) noexcept = 0;
  virtual void  fill(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t) noexcept = 0;
  virtual void  fill(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t, std::uint8_t*) noexcept = 0;
  virtual void  fill(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t*) noexcept = 0;
          lcd&  operator=(const lcd&) noexcept;
          lcd&  operator=(lcd&&) noexcept;
};

/*namespace dev*/ }
#endif
