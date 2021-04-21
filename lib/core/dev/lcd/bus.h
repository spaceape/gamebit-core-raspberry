#ifndef dev_lcd_ilibus_h
#define dev_lcd_ilibus_h
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
#include <hardware/gpio.h>
#include <hardware/spi.h>

static spi_inst_t* g_lcd_bus_spi;

static constexpr unsigned int bit_cs   =   1 << PIN_LCD_CS;
static constexpr unsigned int bit_dc   =   1 << PIN_LCD_DC;
static constexpr unsigned int bit_rd   =   1 << PIN_LCD_RD;
static constexpr unsigned int bit_wr   =   1 << PIN_LCD_WR;
static constexpr unsigned int bit_rst  =   1 << PIN_LCD_RST;
static constexpr unsigned int bit_data = 255 << PIN_LCD_D0;

void  nop() noexcept
{
      asm volatile("nop\n");
}

inline void  nop(int count) noexcept
{
      while(count > 0) {
          nop();
          count--;
      }
}

inline void  cs_set_l() noexcept
{
      gpio_clr_mask(bit_cs);
}

inline void  cs_set_h() noexcept
{
      gpio_set_mask(bit_cs);
}

inline void  rd_set_l() noexcept
{
      gpio_clr_mask(bit_rd);
}

inline void  rd_set_h() noexcept
{
      gpio_set_mask(bit_rd);
}

inline void  wr_set_l() noexcept
{
      gpio_clr_mask(bit_wr);
}

inline void  wr_set_h() noexcept
{
      gpio_set_mask(bit_wr);
}

inline void  dc_set_l() noexcept
{
      gpio_clr_mask(bit_dc);
}

inline void  dc_set_h() noexcept
{
      gpio_set_mask(bit_dc);
}

inline void  rst_set_l() noexcept
{
      gpio_clr_mask(bit_rst);
}

inline void  rst_set_h() noexcept
{
      gpio_set_mask(bit_rst);
}

inline void  lcd_get() noexcept
{
}

template<typename... Args>
inline void  lcd_get(nullptr_t port, uint8_t& data, Args&... args) noexcept
{
  #if GPU_DRIVER == GPU_DRIVER_ILI9341S
  #endif
  #if GPU_DRIVER == GPU_DRIVER_ILI9341P
      rd_set_l();
      data = (gpio_get_all() & bit_data) >> PIN_LCD_D0;
      rd_set_h();
  #endif
      lcd_get(args...);
}

inline void  lcd_put() noexcept
{
}

inline void  lcd_put_byte(uint8_t data) noexcept
{
  #if GPU_DRIVER == GPU_DRIVER_ILI9341S
      spi_write_blocking(g_lcd_bus_spi, std::addressof(data), sizeof(data));
  #endif
  #if GPU_DRIVER == GPU_DRIVER_ILI9341P
      wr_set_l();
      gpio_set_mask(data << PIN_LCD_D0);
      wr_set_h();
  #endif
      gpio_clr_mask(bit_data);
}

template<typename... Args>
inline void  lcd_put(uint8_t data, Args&&... args) noexcept
{
      lcd_put_byte(data);
      lcd_put(std::forward<Args>(args)...);
}

template<typename... Args>
inline void  lcd_put(uint16_t data, Args&&... args) noexcept
{
      lcd_put_byte((data & 0xff00) >> 8);
      lcd_put_byte((data & 0x00ff));
      lcd_put(std::forward<Args>(args)...);
}

template<typename... Args>
inline void  lcd_put(int data, Args&&... args) noexcept
{
      lcd_put_byte(data);
      lcd_put(std::forward<Args>(args)...);
}

template<typename... Args>
void  lcd_recv(uint8_t cmd, Args&... args) noexcept
{
      cs_set_l();
      dc_set_l();
      lcd_put(cmd);
      dc_set_h();
      gpio_set_dir_in_masked(bit_data);
      lcd_get(args...);
      gpio_set_dir_out_masked(bit_data);
      cs_set_h();
}

template<typename... Args>
void  lcd_send(uint8_t cmd, Args&&... args) noexcept
{
      cs_set_l();
      dc_set_l();
      lcd_put(cmd);
      dc_set_h();
      lcd_put(std::forward<Args>(args)...);
      cs_set_h();
}
#endif
