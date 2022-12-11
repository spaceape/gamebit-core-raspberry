/** 
    Copyright (c) 2022, wicked systems
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
#include "ili9341s.h"
#include "display/ili9341.h"
// #include "display/cm256x16.h"
// #include "display/cs8x8.h"
#include <hardware.h>

namespace gfx {
extern   surface*            g_surface_ptr;
extern   dc::mapping_base_t* g_mapping_ptr;
/*namespace gfx*/ }

namespace dev {

// default SPI baud rate for this driver
static constexpr unsigned int lcd_baud = 16777216;

// timeout
static constexpr unsigned int lcd_timeout_us = 32768;

static constexpr bool  lcd_use_crc = false;
static constexpr bool  lcd_use_dma = false;

      ili9341s::ili9341s(spi* bus, unsigned int cs_pin, unsigned int ds_pin, unsigned int reset_pin) noexcept:
      sys::device(type),
      gfx::sdi(),
      m_io(bus, ds_pin),
      m_ds_pin(ds_pin),
      m_reset_pin(reset_pin),
      m_attach_bit(false),
      m_enable_bit(false),
      m_ready_bit(false)
{
      m_io.set_rx_pattern(255);
      m_io.set_timeout(lcd_timeout_us);
      m_io.set_baud(lcd_baud);
      gpio_init(ds_pin);
      gpio_set_dir(ds_pin, GPIO_OUT);
      gpio_put(ds_pin, 1);
      gpio_init(reset_pin);
      gpio_set_dir(reset_pin, GPIO_OUT);
      gpio_put(reset_pin, 0);
      m_io.set_monitor(this);

      // m_dev_cmo.reset(gfx::cmo::fmt_rgb_565, 256, g_cm256x16_std_data, sizeof(g_cm256x16_std_data));
      // m_dev_cso.reset(gfx::cso::fmt_1bpp, 8, 8, g_cs8x8_std_data, sizeof(g_cs8x8_std_data));
}

      ili9341s::~ili9341s()
{
      dev_suspend();
}

void  ili9341s::bus_attach(spi*) noexcept
{
      m_attach_bit = true;
      dev_update();
}

void  ili9341s::bus_acquire(spi*) noexcept
{
}

void  ili9341s::bus_release(spi*) noexcept
{
}

void  ili9341s::bus_detach(spi*) noexcept
{
      m_attach_bit = false;
      dev_update();
}

void  ili9341s::dev_power_on() noexcept
{
      dev_send_cmd(ili9341_slpout);
      dev_send_cmd(ili9341_dispon);
}

void  ili9341s::dev_power_off() noexcept
{
      dev_send_cmd(ili9341_dispoff);
}

void  ili9341s::dev_hard_reset() noexcept
{
      gpio_clr_mask(1 << m_reset_pin);
      sleep_ms(64);
      gpio_set_mask(1 << m_reset_pin);
      sleep_ms(64);
}

void  ili9341s::dev_soft_reset() noexcept
{
      dev_send_cmd(ili9341_swrst);
}

void  ili9341s::dev_raset(std::uint16_t sr, std::uint16_t er)
{
      dev_send_cmd(ili9341_paset, sr, er);
}

void  ili9341s::dev_caset(std::uint16_t sc, std::uint16_t ec)
{
      dev_send_cmd(ili9341_caset, sc, ec);
}

void  ili9341s::dev_resume() noexcept
{
      if(m_ready_bit == false) {

          dev_hard_reset();
          dev_soft_reset();

          dev_send_cmd(0xef, 0x03, 0x80, 0x02);
          dev_send_cmd(0xcf, 0x00, 0xc1, 0x30);
          dev_send_cmd(0xed, 0x64, 0x03, 0x12, 0x81);
          dev_send_cmd(0xe8, 0x85, 0x00, 0x78);               // driver timing control A
          dev_send_cmd(0xcb, 0x39, 0x2c, 0x00, 0x34, 0x02);   // extended power control A
          dev_send_cmd(0xf7, 0x20);                           // pump ratio control
          dev_send_cmd(0xea, 0x00, 0x00);                     // driver timing control B
          dev_send_cmd(ili9341_pwctr1, 0x23);
          dev_send_cmd(ili9341_pwctr2, 0x10);
          dev_send_cmd(ili9341_vmctr1, 0x3e, 0x28);
          dev_send_cmd(ili9341_vmctr2, 0x86);

          dev_send_cmd(ili9341_madctl, 49);                   // RGB colour order

          dev_send_cmd(ili9341_pixfmt, 0x55);                 // pixel format == 16 bit per colour
          dev_send_cmd(ili9341_frmctr1, 0x00, 0x00);          // frame control: 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
          dev_send_cmd(ili9341_dfunctr, 0x08, 0x82, 0x27);
          dev_send_cmd(0xf2, 00);                             // 3gamma function disable
          dev_send_cmd(ili9341_gammaset, 0x01);
          dev_send_cmd(ili9341_gmctrp1, 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00);
          dev_send_cmd(ili9341_gmctrn1, 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f);

          dev_power_on();

          m_display_sx = 320;
          m_display_sy = 240;
          m_ready_bit = true;
      }
}

void  ili9341s::dev_suspend() noexcept
{
}

/* dev_update()
   update the device state according to the user and device set flags
*/
void  ili9341s::dev_update() noexcept
{
      bool l_state_old = m_ready_bit;
      bool l_state_new = m_enable_bit && m_attach_bit;
      if(l_state_new != l_state_old) {
          if(l_state_new == true) {
              dev_resume();
          } else
              dev_suspend();
      }
}

/* gfx_render()
   render current surface mapping, saved into the static `dc` members `s_surface_ptr` and `s_surface_mapping`
*/
void  ili9341s::gfx_render() noexcept
{
      if(auto
          p_mapping =  static_cast<mapping_t*>(gfx::g_mapping_ptr);
          p_mapping != nullptr) {
          std::uint8_t  l_format       = p_mapping->format;
          std::uint16_t l_render_flags = p_mapping->render_flags;
          std::uint16_t l_option_flags = p_mapping->option_flags;
          if(p_mapping->ready_bit) {
              if(p_mapping->render_flags != map_none) {
                  int  l_src_px0 = p_mapping->dx;
                  int  l_src_py0 = p_mapping->dy;
                  int  l_dst_px0 = p_mapping->px;
                  int  l_dst_py0 = p_mapping->py;
                  int  l_dst_px1 = l_dst_px0 + p_mapping->sx;
                  int  l_dst_py1 = l_dst_py0 + p_mapping->sy;
                  int  l_src_px1;
                  int  l_src_py1;
                  bool l_render_cbo = l_option_flags & surface::opt_request_tile_graphics;
                  bool l_render_pbo = l_option_flags & (surface::opt_request_raster_graphics | surface::opt_request_vector_graphics);
                  if(l_dst_px0 >= m_display_sx) {
                      return;
                  }
                  if(l_dst_px0 < 0) {
                      l_dst_px1 += l_dst_px0;
                      l_src_px0 -= l_dst_px0;
                      l_dst_px0 = 0;
                      if(l_dst_px1 <= 0) {
                          return;
                      }
                  }
                  if(l_dst_py0 >= m_display_sy) {
                      return;
                  }
                  if(l_dst_py0 < 0) {
                      l_dst_py1 += l_dst_py0;
                      l_src_py0 -= l_dst_py0;
                      l_dst_py0 = 0;
                      if(l_dst_py1 <= 0) {
                          return;
                      }
                  }
                  l_src_px1 = l_src_px0 + l_dst_px1 - l_dst_px0;
                  l_src_py1 = l_src_py0 + l_dst_py1 - l_dst_py0;
                  if(l_render_pbo) {
                  }
                  if(l_render_cbo) {
                  }
              }
          }
      }
}

bool  ili9341s::resume() noexcept
{
      m_enable_bit = true;
      dev_update();
      return m_ready_bit;
}

int   ili9341s::get_display_sx() const noexcept
{
      return m_display_sx;
}

int   ili9341s::get_display_sy() const noexcept
{
      return m_display_sy;
}

bool  ili9341s::suspend() noexcept
{
      m_enable_bit = false;
      dev_update();
      return m_ready_bit;
}

void  ili9341s::sync(int dt) noexcept
{
      gfx_sync(dt);
}

/*namespace dev*/ }
