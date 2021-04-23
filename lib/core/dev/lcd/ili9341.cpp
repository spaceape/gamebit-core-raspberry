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
#include "ili9341.h"
#include "bus.h"
#include "ili9341.h"
#include "gfx/tile/font/std8x8s0.inc.cpp"
#include "gfx/tile/palette/rgb565.inc.cpp"
#include <cstring>

#define ILI9341_NOP         0x00
#define ILI9341_SWRST       0x01
#define ILI9341_RDDID       0x04
#define ILI9341_RDDST       0x09

#define ILI9341_RDMODE      0x0A
#define ILI9341_RDMADCTL    0x0B
#define ILI9341_RDPIXFMT    0x0C
#define ILI9341_RDIMGFMT    0x0A
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_SLPIN       0x10
#define ILI9341_SLPOUT      0x11
#define ILI9341_PTLON       0x12
#define ILI9341_NORON       0x13

#define ILI9341_INVOFF      0x20
#define ILI9341_INVON       0x21
#define ILI9341_GAMMASET    0x26
#define ILI9341_DISPOFF     0x28
#define ILI9341_DISPON      0x29

#define ILI9341_CASET       0x2A
#define ILI9341_PASET       0x2B
#define ILI9341_RAMWR       0x2C
#define ILI9341_RAMRD       0x2E

#define ILI9341_PTLAR       0x30
#define ILI9341_VSCRDEF     0x33
#define ILI9341_MADCTL      0x36
#define ILI9341_VSCRSADD    0x37
#define ILI9341_PIXFMT      0x3A

#define ILI9341_WRDISBV     0x51
#define ILI9341_RDDISBV     0x52
#define ILI9341_WRCTRLD     0x53

#define ILI9341_FRMCTR1     0xB1
#define ILI9341_FRMCTR2     0xB2
#define ILI9341_FRMCTR3     0xB3
#define ILI9341_INVCTR      0xB4
#define ILI9341_DFUNCTR     0xB6

#define ILI9341_PWCTR1      0xC0
#define ILI9341_PWCTR2      0xC1
#define ILI9341_PWCTR3      0xC2
#define ILI9341_PWCTR4      0xC3
#define ILI9341_PWCTR5      0xC4
#define ILI9341_VMCTR1      0xC5
#define ILI9341_VMCTR2      0xC7

#define ILI9341_RDID4       0xD3
#define ILI9341_RDINDEX     0xD9
#define ILI9341_RDID1       0xDA
#define ILI9341_RDID2       0xDB
#define ILI9341_RDID3       0xDC
#define ILI9341_RDIDX       0xDD

#define ILI9341_GMCTRP1     0xE0
#define ILI9341_GMCTRN1     0xE1

#define ILI9341_IFCTL       0xF6

#define ILI9341_MADCTL_MY   0x80
#define ILI9341_MADCTL_MX   0x40
#define ILI9341_MADCTL_MV   0x20
#define ILI9341_MADCTL_ML   0x10
#define ILI9341_MADCTL_RGB  0x00
#define ILI9341_MADCTL_BGR  0x08
#define ILI9341_MADCTL_MH   0x04

void  ili_nop() noexcept
{
      lcd_send(ILI9341_NOP);
}

void  ili_hard_reset() noexcept
{
      rst_set_l();
      sleep_ms(64);
      rst_set_h();
      sleep_ms(64);
}

void  ili_soft_reset() noexcept
{
      lcd_send(ILI9341_SWRST);
      sleep_ms(5);
}

void  ili_caset(uint16_t px0, uint16_t px1) noexcept
{
      lcd_send(ILI9341_CASET, px0, px1);
}

void  ili_raset(uint16_t py0, uint16_t py1) noexcept
{
      lcd_send(ILI9341_PASET, py0, py1);
}

void  ili_blit(uint16_t colour, long int count) noexcept
{
      cs_set_l();
      dc_set_l();
      lcd_put(ILI9341_RAMWR);
      dc_set_h();
      while(count) {
          lcd_put_byte((colour & 0xff00) >> 8);
          lcd_put_byte(colour & 0x00ff);
          --count;
      }
      cs_set_h();
}

void  ili_blit(uint8_t* image, uint16_t* palette, long int size) noexcept
{
      cs_set_l();
      dc_set_l();
      lcd_put(ILI9341_RAMWR);
      dc_set_h();
      if(true) {
          auto  l_iter = image;
          auto  l_last = image + size;
          std::uint16_t l_colour;
          while(l_iter < l_last) {
              l_colour = palette[*l_iter];
              lcd_put_byte((l_colour & 0xff00) >> 8);
              lcd_put_byte(l_colour & 0x00ff);
              ++l_iter;
          }
      }
      cs_set_h();
}

void  ili_blit(uint16_t* image, long int size) noexcept
{
      cs_set_l();
      dc_set_l();
      lcd_put(ILI9341_RAMWR);
      dc_set_h();
      if(true) {
          auto  l_iter = image;
          auto  l_last = image + size;
          while(l_iter < l_last) {
              lcd_put_byte((*l_iter & 0xff00) >> 8);
              lcd_put_byte(*l_iter & 0x00ff);
              ++l_iter;
          }
      }
      cs_set_h();
}

void  ili_partial_mode_on() noexcept
{
      lcd_send(ILI9341_PTLON);
}

void  ili_display_mode_on() noexcept
{
      lcd_send(ILI9341_NORON);
}

void  ili_set_partial_area(uint16_t sr, uint16_t er) noexcept
{
      lcd_send(ILI9341_PTLAR, sr, er);
}

namespace dev {

      ili9341::ili9341(int sx, int sy) noexcept:
      dc(),
      m_scene(sx, sy),
      m_display_sx(sx),
      m_display_sy(sy),
      m_support(0),
      m_ready(false),
      m_running(false)
{
      m_ready = true;
      resume();
}

      ili9341::~ili9341() noexcept
{
      suspend();
}

void  ili9341::bus_acquire() noexcept
{
      gpio_init_mask(bit_cs | bit_dc | bit_rst);
      gpio_set_dir_out_masked(bit_cs | bit_dc | bit_rst);
      gpio_set_mask(bit_cs | bit_dc | bit_rst);
  #if GPU_DRIVER == GPU_DRIVER_ILI9341S
      if((PIN_LCD_TX == 3) ||
          (PIN_LCD_TX == 7) ||
          (PIN_LCD_TX == 19)) {
          g_lcd_bus_spi = spi0;
      } else
      if((PIN_LCD_TX == 11) ||
          (PIN_LCD_TX == 15)) {
          g_lcd_bus_spi = spi1;
      } else
          g_lcd_bus_spi = nullptr;
      spi_init(g_lcd_bus_spi, spi_lcd_baud_auto);
      gpio_set_function(PIN_LCD_RX, GPIO_FUNC_SPI);
      gpio_set_function(PIN_LCD_TX, GPIO_FUNC_SPI);
      gpio_set_function(PIN_LCD_SCK, GPIO_FUNC_SPI);
  #endif
  #if GPU_DRIVER == GPU_DRIVER_ILI9341P
      gpio_init_mask(bit_rd | bit_wr | bit_data);
      gpio_set_dir_out_masked(bit_rd | bit_wr | bit_data);
      gpio_set_mask(bit_rd | bit_wr);
      g_lcd_bus_spi = nullptr;
  #endif
}

bool  ili9341::gfx_load_charmap() noexcept
{
      m_charmap = reinterpret_cast<std::uint8_t*>(malloc(s_charmap_size * 8));
      if(m_charmap) {
          std::memcpy(m_charmap, g_std8x8s0_data, sizeof(g_std8x8s0_data));
          return true;
      }
      return false;
}

void  ili9341::gfx_free_charmap() noexcept
{
      if(m_charmap) {
          free(m_charmap);
      }
}

bool  ili9341::gfx_load_colourmap() noexcept
{
      m_palette = reinterpret_cast<std::uint16_t*>(malloc(256 * sizeof(std::uint16_t)));
      if(m_palette) {
          std::memcpy(m_palette, g_rgb565_data, sizeof(g_rgb565_data));
          return true;
      }
      return false;
}

void  ili9341::gfx_free_colourmap() noexcept
{
      if(m_palette) {
          free(m_palette);
      }
}

void  ili9341::gfx_context_acquire() noexcept
{
      if(g_driver != this) {
          g_support = m_support;
          g_display_sx = m_display_sx;
          g_display_sy = m_display_sy;
          g_charmap = m_charmap;
          g_palette = reinterpret_cast<uint8_t*>(m_palette);
          g_driver = this;
      }
}

void  ili9341::gfx_context_release() noexcept
{
}

void  ili9341::bus_release() noexcept
{
}

gfx::scene& ili9341::get_device() noexcept
{
      return m_scene;
}

bool  ili9341::resume() noexcept
{
      if(m_ready == true) {
          if(m_running == false) {
              bus_acquire();
              ili_hard_reset();
              ili_soft_reset();

              lcd_send(0xef, 0x03, 0x80, 0x02);
              lcd_send(0xcf, 0x00, 0xc1, 0x30);
              lcd_send(0xed, 0x64, 0x03, 0x12, 0x81);
              lcd_send(0xe8, 0x85, 0x00, 0x78);               // driver timing control A
              lcd_send(0xcb, 0x39, 0x2c, 0x00, 0x34, 0x02);   // extended power control A
              lcd_send(0xf7, 0x20);                           // pump ratio control
              lcd_send(0xea, 0x00, 0x00);                     // driver timing control B
              lcd_send(ILI9341_PWCTR1, 0x23);
              lcd_send(ILI9341_PWCTR2, 0x10);
              lcd_send(ILI9341_VMCTR1, 0x3e, 0x28);
              lcd_send(ILI9341_VMCTR2, 0x86);

              lcd_send(ILI9341_MADCTL, 49);                   // RGB colour order

              lcd_send(ILI9341_PIXFMT, 0x55);                 // pixel format == 16 bit per colour
              lcd_send(ILI9341_FRMCTR1, 0x00, 0x00);          // frame control: 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
              lcd_send(ILI9341_DFUNCTR, 0x08, 0x82, 0x27);
              lcd_send(0xf2, 00);                             // 3gamma function disable
              lcd_send(ILI9341_GAMMASET, 0x01);
              lcd_send(ILI9341_GMCTRP1, 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00);
              lcd_send(ILI9341_GMCTRN1, 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f);
            
              lcd_send(ILI9341_SLPOUT);
              lcd_send(ILI9341_DISPON);
              if(m_scene.resume()) {
                  if(gfx_load_colourmap()) {
                      if(gfx_load_charmap()) {
                          m_running = true;
                      }
                  }
              }
          }
      }
      return m_running == true;
}

void  ili9341::set_ras(std::uint16_t py0, std::uint16_t py1) noexcept
{
      ili_raset(py0, py1);
}

void  ili9341::set_cas(std::uint16_t px0, std::uint16_t px1) noexcept
{
      ili_caset(px0, px1);
}

void  ili9341::fill(std::uint16_t colour, long int count) noexcept
{
      ili_blit(colour, count);
}

void  ili9341::fill(std::uint16_t* data, long int count) noexcept
{
      ili_blit(data, count);
}

void  ili9341::fill(std::uint16_t px0, std::uint16_t py0, std::uint16_t px1, std::uint16_t py1, std::uint16_t colour) noexcept
{
      long int l_count = (px1 - px0) * (py1 - py0);
      if(l_count > 0) {
          ili_caset(px0, px1 - 1);
          ili_raset(py0, py1 - 1);
          ili_blit(colour, l_count);
      }
}

void  ili9341::fill(std::uint16_t px0, std::uint16_t py0, std::uint16_t px1, std::uint16_t py1, std::uint8_t* data) noexcept
{
      long int l_count = (px1 - px0) * (py1 - py0);
      if(l_count > 0) {
          ili_caset(px0, px1 - 1);
          ili_raset(py0, py1 - 1);
          ili_blit(data, m_palette, l_count);
      }
}

void  ili9341::fill(std::uint16_t px0, std::uint16_t py0, std::uint16_t px1, std::uint16_t py1, std::uint16_t* data) noexcept
{
      long int l_count = (px1 - px0) * (py1 - py0);
      if(l_count > 0) {
          ili_caset(px0, px1 - 1);
          ili_raset(py0, py1 - 1);
          ili_blit(data, l_count);
      }
}

bool  ili9341::suspend() noexcept
{
      if(m_running == true) {
          m_scene.suspend();
          bus_release();
          gfx_free_colourmap();
          gfx_free_charmap();
          m_running = false;
      }
      return m_running == false;
}

void  ili9341::render() noexcept
{
      gfx_context_acquire();
      m_scene.render();
      gfx_context_release();
}

void  ili9341::sync(float dt) noexcept
{
      m_scene.sync(dt);
}

/*namespace dev*/ }
