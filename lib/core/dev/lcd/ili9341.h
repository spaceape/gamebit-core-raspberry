#ifndef dev_lcd_ili9341_h
#define dev_lcd_ili9341_h
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
#include <dev/lcd.h>
#include "gfx/gfx.h"
#include "gfx/scene.h"

namespace dev {

/* ili9341
   ili9341 device driver with 8 serial or bit parallel interface
*/
class ili9341: public dev::lcd, public gfx::dc
{
  gfx::scene      m_scene;
  int             m_display_sx;
  int             m_display_sy;
  std::uint8_t    m_support;
  std::uint8_t*   m_charmap;
  std::uint16_t*  m_palette;
  bool            m_ready:1;
  bool            m_running:1;

  protected:
          void  bus_acquire() noexcept;
          bool  gfx_load_charmap() noexcept;
          void  gfx_free_charmap() noexcept;
          bool  gfx_load_colourmap() noexcept;
          void  gfx_free_colourmap() noexcept;
          void  gfx_context_acquire() noexcept;
          void  gfx_context_release() noexcept;
          void  bus_release() noexcept;

  public:
          ili9341(int = 320, int = 240) noexcept;
          ili9341(const ili9341&) noexcept = delete;
          ili9341(ili9341&&) noexcept = delete;
  virtual ~ili9341();
          gfx::scene& get_device() noexcept;
          bool  resume() noexcept;
  virtual void  set_ras(std::uint16_t, std::uint16_t) noexcept override;
  virtual void  set_cas(std::uint16_t, std::uint16_t) noexcept override;
  virtual void  fill(std::uint16_t, long int) noexcept override;
  virtual void  fill(std::uint16_t*, long int) noexcept override;
  virtual void  fill(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t) noexcept override;
  virtual void  fill(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t, std::uint8_t*) noexcept override;
  virtual void  fill(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t*) noexcept override;
          bool  suspend() noexcept;
          void  render() noexcept;
          void  sync(float) noexcept;
          ili9341& operator=(const ili9341&) noexcept = delete;
          ili9341& operator=(ili9341&&) noexcept = delete;
};
/*namespace dev*/ }
#endif
