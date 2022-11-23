#ifndef bops_dev_ili9341s_h
#define bops_dev_ili9341s_h
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
#include <blackops.h>
#include <sys.h>
#include <gfx.h>
#include <gfx/support/sdi.h>
#include "spi.h"
#include <hardware/spi.h>

namespace dev {

/* ili9341s
   device for the ILI9341 and compatible graphics chips on the SPI bus
*/
class ili9341s: public sys::device, public gfx::sdi, public spi::monitor
{
  spio          m_io;
  std::uint8_t  m_ds_pin;
  std::uint8_t  m_reset_pin;

  short int     m_display_sx;
  short int     m_display_sy;

  bool          m_attach_bit:1;
  bool          m_enable_bit:1;
  bool          m_ready_bit:1;

  protected:
  virtual void  bus_attach(spi*) noexcept override;
  virtual void  bus_acquire(spi*) noexcept override;
  virtual void  bus_release(spi*) noexcept override;
  virtual void  bus_detach(spi*) noexcept override;

          void  dev_power_on() noexcept;
          void  dev_power_off() noexcept;
          void  dev_hard_reset() noexcept;
          void  dev_soft_reset() noexcept;
          void  dev_raset(std::uint16_t, std::uint16_t);
          void  dev_caset(std::uint16_t, std::uint16_t);
          void  dev_resume() noexcept;
          void  dev_suspend() noexcept;
          void  dev_update() noexcept;

  inline  void  bus_send_raw() noexcept
  {
  }

  template<typename... Args>
  inline  void  bus_send_raw(std::uint8_t data, Args&&... next) noexcept
  {
          m_io.send(data);
          bus_send_raw(std::forward<Args&&>(next)...);
  }

  template<typename... Args>
  inline  void  bus_send_raw(std::uint16_t data, Args&&... next) noexcept
  {
          std::uint8_t l_bytes[2];
          if constexpr (os::is_lsb) {
              l_bytes[0] = (data & 0xff00) >> 8;
              l_bytes[1] = (data & 0x00ff);
          } else
          if constexpr (os::is_msb) {
              l_bytes[0] = (data & 0x00ff);
              l_bytes[1] = (data & 0xff00) >> 8;
          }
          m_io.send(l_bytes, 2);
          bus_send_raw(std::forward<Args&&>(next)...);
  }

  template<typename... Args>
  inline  void  bus_send_raw(int data, Args&&... next) noexcept
  {
          m_io.send(static_cast<std::uint8_t>(data & 0xff));
          bus_send_raw(std::forward<Args&&>(next)...);
  }

  template<typename... Args>
  inline  void  bus_send_raw(std::uint8_t* data, int size) noexcept
  {
          m_io.send(data, size);
  }

  template<typename... Args>
  inline  void  dev_send_raw(Args&&... next) noexcept {
          m_io.acquire();
          bus_send_raw(std::forward<Args&&>(next)...);
          m_io.release();
  }

  template<typename... Args>
  inline  void  dev_send_cmd(std::uint8_t cmd, Args&&... next) noexcept {
          m_io.acquire();
          gpio_clr_mask(1 << m_ds_pin);
          m_io.send(cmd);
          gpio_set_mask(1 << m_ds_pin);
          bus_send_raw(std::forward<Args&&>(next)...);
          m_io.release();
  }

  virtual void gfx_render() noexcept override;

  public:
          ili9341s(spi*, unsigned int, unsigned int, unsigned int) noexcept;
          ili9341s(const ili9341s&) noexcept = delete;
          ili9341s(ili9341s&&) noexcept = delete;
  virtual ~ili9341s();

          bool  resume() noexcept;

  virtual int   get_display_sx() const noexcept override;
  virtual int   get_display_sy() const noexcept override;

          bool  suspend() noexcept;

  static  ili9341s* get_from_adt(const char* path) noexcept {
          return cast_from_adt<ili9341s>(path);
  }

  virtual void  sync(int) noexcept override;

          ili9341s& operator=(const ili9341s&) noexcept = delete;
          ili9341s& operator=(ili9341s&&) noexcept = delete;
};

/*namespace dev*/ }
#endif
