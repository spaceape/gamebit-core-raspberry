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
#include "osc.h"
#include "bus.h"
#include <limits>

namespace dev {

      osc::osc() noexcept:
      m_bus(bus_null),
      m_function(fct_null),
      m_trigger_bit(0),
      m_release_bit(0),
      m_envelope_bit(0),
      m_loop_bit(0),
      m_period(0.0f),
      m_offset(0.0f),
      m_time(0),
      m_gain(127),
      m_hold(127),
      m_phase(0),
      m_param(0),
      m_attack(0),
      m_decay(0),
      m_sustain(0),
      m_release(0)
{
}

      osc::osc(const osc& copy) noexcept:
      m_bus(copy.m_bus),
      m_function(copy.m_function),
      m_trigger_bit(copy.m_trigger_bit),
      m_release_bit(copy.m_release_bit),
      m_envelope_bit(copy.m_envelope_bit),
      m_loop_bit(copy.m_loop_bit),
      m_period(copy.m_period),
      m_offset(copy.m_offset),
      m_time(copy.m_time),
      m_gain(copy.m_gain),
      m_hold(copy.m_hold),
      m_phase(copy.m_phase),
      m_param(copy.m_param),
      m_attack(copy.m_attack),
      m_decay(copy.m_decay),
      m_sustain(copy.m_sustain),
      m_release(copy.m_release)
{
}

      osc::osc(osc&& copy) noexcept:
      osc(copy)
{
      copy.reset();
}

      osc::~osc()
{
}

bool  osc::set(int shape, float tone, float time) noexcept
{
      reset();
      if(shape > fct_null) {
          if((tone > 0.0f) && (time > 0.0f)) {
              float l_period  = 1.0f / tone;
              int   l_sustain = time * tone;
              if((l_sustain > 0) && (l_sustain <= static_cast<int>(std::numeric_limits<std::uint16_t>::max()))) {
                  m_function = shape;
                  m_period = l_period;
                  m_sustain = l_sustain;
                  return true;
              }
          }
      }
      return false;
}

bool  osc::set(int shape, float tone, float attack, float decay, float sustain, float release) noexcept
{
      reset();
      if(shape > fct_null) {
          if(tone > 0.0f) {
              float l_period = 1.0f / tone;
              int   l_attack = attack * tone;
              if(l_attack > static_cast<int>(std::numeric_limits<std::uint16_t>::max())) {
                  l_attack = std::numeric_limits<std::uint16_t>::max();
              }
              int   l_decay = decay * tone;
              if(l_decay > static_cast<int>(std::numeric_limits<std::uint16_t>::max())) {
                  l_decay = std::numeric_limits<std::uint16_t>::max();
              }
              int   l_sustain = sustain * tone;
              if(l_sustain > static_cast<int>(std::numeric_limits<std::uint16_t>::max())) {
                  l_sustain = std::numeric_limits<std::uint16_t>::max();
              }
              int   l_release = release * tone;
              if(l_release > static_cast<int>(std::numeric_limits<std::uint16_t>::max())) {
                  l_release = std::numeric_limits<std::uint16_t>::max();
              }
              if(l_attack + l_decay + l_sustain > 0) {
                  m_function = shape;
                  m_period = l_period;
                  m_attack = l_attack;
                  m_decay = l_decay;
                  m_sustain = l_sustain;
                  m_release = l_release;
                  m_envelope_bit = true;
                  return true;
              }
          }
      }
      return false;
}

void  osc::trigger(bool hold) noexcept
{
      m_trigger_bit = true;
      m_loop_bit = hold;
}

void  osc::hold() noexcept
{
      trigger(true);
}

void  osc::release() noexcept
{
      m_time     = m_attack + m_decay + m_sustain;
      m_loop_bit = false;
}

void  osc::mute() noexcept
{
      m_time     = 0;
      m_loop_bit = 0;
      m_trigger_bit = 0;
}

std::int8_t osc::get_phase_angle() const noexcept
{
      return m_phase + m_offset * 256.0f / m_period;
}

std::int8_t osc::get_output() const noexcept
{
      std::int8_t l_phi    = get_phase_angle();
      std::int8_t l_envelope;
      std::int8_t l_result = 0;
      switch(m_function) {
          case fct_square:
              l_result = (l_phi & 0x80) ? -m_gain : +m_gain;
              break;
          case fct_pulse:
              l_result = (l_phi < m_param) ? -m_gain : +m_gain;
              break;
          case fct_saw:
              l_result = l_phi * m_gain / 128;
              break;
          case fct_triangle:
              switch(l_phi & 0xc0) {
                  case 0:
                      l_result = (l_phi & 0x3f) * m_gain / 64;
                      break;
                  case 0x40:
                      l_result = 127 - (l_phi & 0x3f) * m_gain / 64;
                      break;
                  case 0x80:
                      l_result = 0 - (l_phi & 0x3f) * m_gain / 64;
                      break;
                  case 0xc0:
                      l_result = (l_phi & 0x3f) * m_gain / 64 - 127;
                      break;
              };
              break;
          case fct_sin:
              // to approximate a sine wave take a triangle wave...
              switch(l_phi & 0xc0) {
                  case 0:
                      l_result = (l_phi & 0x3f) * m_gain / 64;
                      break;
                  case 0x40:
                      l_result = 127 - (l_phi & 0x3f) * m_gain / 64;
                      break;
                  case 0x80:
                      l_result = 0 - (l_phi & 0x3f) * m_gain / 64;
                      break;
                  case 0xc0:
                      l_result = (l_phi & 0x3f) * m_gain / 64 - 127;
                      break;
              };
              // ...and sum a half period, quarter amplitude triangle wave on top of it
              switch(l_phi & 0xa0) {
                  case 0:
                      l_result += (l_phi & 0x1f) * m_gain / 128;
                      break;
                  case 0x20:
                      l_result += 32 - (l_phi & 0x1f) * m_gain / 128;  
                      break;
                  case 0x80:
                      l_result -= (l_phi & 0x1f) * m_gain / 128;
                      break;
                  case 0xa0:
                      l_result -= 32 - (l_phi & 0x1f) * m_gain / 128;
                      break;
              }
              break;
      }
      if(m_envelope_bit) {
          // multiply with envelope
          if(m_time < m_attack) {
              l_envelope = m_time * m_gain / m_attack;
          } else
          if(m_time < m_attack + m_sustain) {
              l_envelope = m_gain;
          } else
          if(m_time < m_attack + m_sustain + m_decay) {
              l_envelope = m_gain - (m_time - m_attack - m_sustain) * m_gain / m_decay;
          } else
              l_envelope = 0;
          l_result = l_result * l_envelope / 128;
      }
      return l_result;
}

osc*  osc::get_ptr() noexcept
{
      return this;
}

void  osc::assign(const osc& rhs) noexcept
{
      m_bus = rhs.m_bus;
      m_function = rhs.m_function;
      m_trigger_bit = rhs.m_trigger_bit;
      m_release_bit = rhs.m_release_bit;
      m_envelope_bit = rhs.m_envelope_bit;
      m_loop_bit = rhs.m_loop_bit;
      m_period = rhs.m_period;
      m_offset = rhs.m_offset;
      m_time = rhs.m_time;
      m_gain = rhs.m_gain;
      m_hold = rhs.m_hold;
      m_phase = rhs.m_phase;
      m_param = rhs.m_param;
      m_attack = rhs.m_attack;
      m_decay = rhs.m_decay;
      m_sustain = rhs.m_sustain;
      m_release = rhs.m_release;
}

void  osc::reset() noexcept
{
      m_bus = bus_null;
      m_function = fct_null;
      m_envelope_bit = 0;
      m_gain = 127;
      m_hold = 127;
      m_phase = 0;
      m_attack = 0;
      m_decay = 0;
      m_sustain = 0;
      m_release = 0;
}

void  osc::sync(float dt) noexcept
{
      int  l_trigger_time;
      int  l_release_time;
      if(m_trigger_bit) {
          m_offset += dt;
          if(m_offset >= m_period) {
              l_trigger_time = m_attack + m_decay + m_sustain;
              m_time++;
              if(m_time >= l_trigger_time) {
                  if(m_loop_bit == true) {
                      m_time = l_trigger_time;
                  } else
                  if(m_loop_bit == false) {
                      l_release_time = l_trigger_time + m_release;
                      if(m_time < l_release_time) {
                          m_release_bit = true;
                      } else
                      if(m_time >= l_release_time) {
                          reset();
                          m_trigger_bit = false;
                          m_release_bit = false;
                      }
                  }
              }
              //m_offset -= m_period;
              m_offset = 0.0f;
          }
      }
}

osc&  osc::operator=(const osc& rhs) noexcept
{
      assign(rhs);
      return *this;
}

osc&  osc::operator=(osc&& rhs) noexcept
{
      assign(rhs);
      rhs.reset();
      return *this;
}

/*namespace dev*/ }
