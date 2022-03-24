#ifndef dev_pwm_osc_h
#define dev_pwm_osc_h
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
#include <hardware.h>

namespace dev {

class osc
{
  std::int8_t     m_bus;
  std::int8_t     m_function;
  std::int8_t     m_output;
  std::uint8_t    m_trigger_bit:1;    // oscillator has been triggeered
  std::uint8_t    m_release_bit:1;    // oscillator has been released
  std::uint8_t    m_envelope_bit:1;   // envelope enabled
  std::uint8_t    m_loop_bit:1;

  float           m_period;           // oscillation period
  float           m_offset;           // current point within period
  std::int32_t    m_time;             // oscillator time since start (in periods)

  public:
  std::int8_t     m_gain;
  std::int8_t     m_hold;
  std::int8_t     m_phase;
  std::int8_t     m_param;

  std::uint16_t   m_attack;           // envelope attack, in periods
  std::uint16_t   m_decay;            // envelope decay, in periods
  std::uint16_t   m_sustain;          // envelope sustain, in periods
  std::uint16_t   m_release;          // envelope release, in periods

  public:
  static constexpr std::int8_t  osc_min = 0;
  static constexpr std::int8_t  osc_max = 127;
  static constexpr std::int8_t  bus_null = -1;
  static constexpr std::int8_t  bus_chl = -2;   // left channel bus
  static constexpr std::int8_t  bus_chr = -3;   // right channel bus
  static constexpr std::int8_t  bus_sub = -4;   // low frequency bus
  static constexpr std::int8_t  bus_com = -5;   // common bus - copy output to both left and right channel 

  static constexpr std::uint8_t fct_null = 0;
  static constexpr std::uint8_t fct_square = 1;
  static constexpr std::uint8_t fct_pulse = 2;
  static constexpr std::uint8_t fct_saw = 3;
  static constexpr std::uint8_t fct_triangle = 4;
  static constexpr std::uint8_t fct_sin = 5;
  static constexpr std::uint8_t fct_wave = 6;

  public:
          osc() noexcept;
          osc(const osc&) noexcept;
          osc(osc&&) noexcept;
          ~osc();

          bool  set(int, float, float) noexcept;
          bool  set(int, float, float, float, float, float) noexcept;

  inline  bool  has_bus(std::int8_t bus) const noexcept {
          return m_bus == bus;
  }

  inline  std::int8_t get_bus() const noexcept {
          return m_bus;
  }

  inline  void   set_bus(std::int8_t bus) noexcept {
          m_bus = bus;
  }

  inline  bool  is_chl() const noexcept {
          return (m_bus == bus_chl) || (m_bus == bus_com);
  }

  inline  bool  is_chr() const noexcept {
          return (m_bus == bus_chr) || (m_bus == bus_com);
  }

  inline  bool  is_sub() const noexcept {
          return m_bus == bus_sub;
  }

  inline  bool  is_busy() const noexcept {
          return m_trigger_bit == true;
  }

  inline  bool  is_idle() const noexcept {
          return m_trigger_bit == false;
  }

          void  trigger(bool = false) noexcept;
          void  hold() noexcept;
          void  release() noexcept;
          void  mute() noexcept;

          std::int8_t get_phase_angle() const noexcept;
          std::int8_t get_output() const noexcept;
          osc*  get_ptr() noexcept;
          void  assign(const osc&) noexcept;
          void  reset() noexcept;
          void  sync(float) noexcept;
          osc&  operator=(const osc&) noexcept;
          osc&  operator=(osc&&) noexcept;
};

/*namsepace dev*/ }
#endif
