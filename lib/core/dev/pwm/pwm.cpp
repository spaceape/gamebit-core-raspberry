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
#include <dev/pwm.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include "bus.h"

namespace dev {

static pwm*               g_pwm_head;
static pwm*               g_pwm_tail;
static unsigned int       g_pwm_slice;
static repeating_timer_t  g_pwm_timer;
static osc                g_osc_pool[osc_cnt];

      pwm::pwm() noexcept:
      p_next(nullptr),
      m_find(0),
      m_running(false)
{
      resume();
}

      pwm::pwm(const pwm&) noexcept
{
}

      pwm::pwm(pwm&&) noexcept
{
}

      pwm::~pwm()
{
      suspend();
}

/* pwm_sync()
   call sync() on all pwm instances
*/
void  pwm::pwm_sync() noexcept
{
      pwm*  l_pwm_iter = g_pwm_head;
      while(l_pwm_iter) {
          l_pwm_iter->sync();
          l_pwm_iter = l_pwm_iter->p_next;
      }
}

void  pwm::bus_acquire() noexcept
{
      gpio_set_function(PIN_PWM_L, GPIO_FUNC_PWM);
      gpio_set_function(PIN_PWM_R, GPIO_FUNC_PWM);
      gpio_init(PIN_DBG);
      gpio_set_dir(PIN_DBG, GPIO_OUT);

      g_pwm_slice = pwm_gpio_to_slice_num(PIN_PWM_L);
      pwm_set_wrap(g_pwm_slice, 255);
      pwm_set_enabled(g_pwm_slice, true);

      add_repeating_timer_us(-osc_sync_us, loop, nullptr, std::addressof(g_pwm_timer));
}

/* bus_render_mod()
   render all modulation oscillators
*/
void  pwm::bus_render_mod() noexcept
{
}

/* bus_render_snd()
   render all oscillators on the sound bus
*/
void  pwm::bus_render_snd() noexcept
{
      int l_chl_count = 0;
      int l_chl_value = 0;
      int l_chr_count = 0;
      int l_chr_value = 0;
      for(int i_osc = 0; i_osc < osc_cnt; i_osc++) {
          auto& l_osc = g_osc_pool[i_osc];
          if(l_osc.is_chl()) {
              l_chl_value += l_osc.get_output();
              l_chl_count++;
          }
          if(l_osc.is_chr()) {
              l_chr_value += l_osc.get_output();
              l_chr_count++;
          }
      }
      pwm_set_chan_level(g_pwm_slice, PWM_CHAN_A, 128 + (l_chl_value / l_chl_count));
      pwm_set_chan_level(g_pwm_slice, PWM_CHAN_B, 128 + (l_chr_value / l_chr_count));
}

/* bus_render_sub()
   render all oscillators on the lfo bus
*/
void  pwm::bus_render_sub() noexcept
{
}

/* bus_sync()
   call sync() all oscillators
*/
void  pwm::bus_sync() noexcept
{
      int i_osc;
      for(i_osc = 0; i_osc < osc_cnt; i_osc++) {
          g_osc_pool[i_osc].sync(osc_sync_dt);
      }
}

void  pwm::bus_release() noexcept
{
      cancel_repeating_timer(std::addressof(g_pwm_timer));
      pwm_set_chan_level(g_pwm_slice, PWM_CHAN_A, 128);
      pwm_set_chan_level(g_pwm_slice, PWM_CHAN_B, 128);
}

void  pwm::sync() noexcept
{
}

bool  pwm::loop(repeating_timer_t*) noexcept
{
      gpio_put(PIN_DBG, 1);
      pwm_sync();
      bus_sync();
      bus_render_mod();
      bus_render_snd();
      bus_render_sub();
      gpio_put(PIN_DBG, 0);
      return true;
}

osc*  pwm::get_osc() noexcept
{
      int  l_last = m_find;
      do {
          if(m_find == osc_cnt) {
              m_find = 0;
          }
          if(g_osc_pool[m_find].is_idle()) {
              return g_osc_pool[m_find].get_ptr();
          }
          m_find++;
      }
      while(m_find != l_last);
      return nullptr;
}

osc*  pwm::get_osc(int shape, float tone, float time) noexcept
{
      osc* l_osc = get_osc();
      if(l_osc) {
          l_osc->set(shape, tone, time);
          return l_osc;
      }
      return nullptr;
}

osc*  pwm::get_osc(int shape, float tone, float attack, float decay, float sustain, float release) noexcept
{
      osc* l_osc = get_osc();
      if(l_osc) {
          l_osc->set(shape, tone, attack, decay, sustain, release);
          return l_osc;
      }
      return nullptr;
}

void  pwm::resume() noexcept
{
      if(m_running == false) {
          if(g_pwm_tail) {
              g_pwm_tail->p_next = this;
          }
          // first pwm device in the list, acquire and setup the hardware resources
          if(g_pwm_head == nullptr) {
              bus_acquire();
              g_pwm_head = this;
          }
          g_pwm_tail = this;
          m_running = true;
      }
}

void  pwm::suspend() noexcept
{
      if(m_running == true) {
          pwm*  l_pwm_prev = nullptr;
          pwm*  l_pwm_iter = g_pwm_head;
          while(l_pwm_iter) {
              if(l_pwm_iter == this) {
                  if(l_pwm_prev != nullptr) {
                      l_pwm_prev->p_next = p_next;
                  } else
                  if(l_pwm_prev == nullptr) {
                      g_pwm_head = l_pwm_iter->p_next;
                  }
                  if(g_pwm_tail == this) {
                      g_pwm_tail = l_pwm_prev;
                  }
                  // no more pwm devices left in the list: release the hardware resources
                  if(g_pwm_head == nullptr) {
                      bus_release();
                  }
                  break;
              }
              l_pwm_prev = l_pwm_iter;
              l_pwm_iter = l_pwm_iter->p_next;
          }
          m_running = false;
      }
}

pwm&  pwm::operator=(const pwm&) noexcept
{
      return *this;
}

pwm&  pwm::operator=(pwm&&) noexcept
{
      return *this;
}

/*namespace dev*/ }
