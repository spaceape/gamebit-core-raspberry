#ifndef dev_pwm_h
#define dev_pwm_h
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
#include <vector>

namespace dev {

class osc;

/* pwm
*/
class pwm
{
  pwm*    p_next;
  int     m_find:8;     // accelerator to find unused oscillator
  bool    m_running:1;

  protected:
  static  void  pwm_sync() noexcept;
  static  void  bus_acquire() noexcept;
  static  void  bus_render_mod() noexcept;
  static  void  bus_render_snd() noexcept;
  static  void  bus_render_sub() noexcept;
  static  void  bus_sync() noexcept;
  static  void  bus_release() noexcept;

  protected:
          void  sync() noexcept;
  static  bool  loop(repeating_timer_t*) noexcept;
  public:
          pwm() noexcept;
          pwm(const pwm&) noexcept;
          pwm(pwm&&) noexcept;
          ~pwm();
          osc*  get_osc() noexcept;
          osc*  get_osc(int, float, float) noexcept;
          osc*  get_osc(int, float, float, float, float, float) noexcept;
          void  resume() noexcept;
          void  suspend() noexcept;
          pwm&  operator=(const pwm&) noexcept;
          pwm&  operator=(pwm&&) noexcept;
};

/*namespace dev*/ }
#endif
