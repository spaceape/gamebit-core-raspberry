#include <stdio.h>
#include <pico/stdlib.h>
#include "lcd-config.h"
#include "lcd-setup.h"
#include <cstdlib>
#include <utility>

static constexpr unsigned int bit_cs   = 1 << TFT_CS;
static constexpr unsigned int bit_dc   = 1 << TFT_DC;

static constexpr unsigned int bit_rd   = 1 << TFT_RD;
static constexpr unsigned int bit_wr   = 1 << TFT_WR;
static constexpr unsigned int bit_rst  = 1 << TFT_RST;

static constexpr unsigned int bit_data = 0x0000ff00;

void  nop() noexcept
{
      asm volatile("nop\n");
}

void  nop(int wait) noexcept
{
      while(wait > 0) {
          nop();
          wait--;
      }
}

void  cs_set_l() noexcept
{
      nop(2);
      gpio_clr_mask(bit_cs);
      nop(2);
}

void  cs_set_h() noexcept
{
      nop(2);
      gpio_set_mask(bit_cs);
      nop(2);
}

void  rd_set_l() noexcept
{
      nop(2);
      gpio_clr_mask(bit_rd);
      nop(2);
}

void  rd_set_h() noexcept
{
      nop(2);
      gpio_set_mask(bit_rd);
      nop(2);
}

void  wr_set_l() noexcept
{
      nop(2);
      gpio_clr_mask(bit_wr);
      nop(2);
}

void  wr_set_h() noexcept
{
      nop(2);
      gpio_set_mask(bit_wr);
      nop(2);
}

void  dc_set_l() noexcept
{
      nop(2);
      gpio_clr_mask(bit_dc);
      nop(2);
}

void  dc_set_h() noexcept
{
      nop(2);
      gpio_set_mask(bit_dc);
      nop(2);
}

void  rst_set_l() noexcept
{
      nop(2);
      gpio_clr_mask(bit_rst);
      nop(16);
}

void  rst_set_h() noexcept
{
      nop(16);
      gpio_set_mask(bit_rst);
      nop(2);
}

void  tft_get() noexcept
{
}

template<typename... Args>
void  tft_get(uint8_t& data, Args&&... args) noexcept
{
      rd_set_l();
      data = (gpio_get_all() & bit_data) >> 8;
      rd_set_h();
      nop(2);
      tft_get(std::forward<Args>(args)...);
}

void  tft_put() noexcept
{
}

template<typename... Args>
void  tft_put(uint8_t data, Args&&... args) noexcept
{
      wr_set_l();
      gpio_set_mask(data << 8);
      wr_set_h();
      gpio_clr_mask(bit_data);
      tft_put(std::forward<Args>(args)...);
}

template<typename Xt, typename... Args>
void  tft_put(Xt&& data, Args&&... args) noexcept
{
      size_t   l_size = sizeof(Xt);
      uint8_t* l_base = reinterpret_cast<uint8_t*>(std::addressof(data));
      uint8_t* l_iter = l_base + l_size;
      uint8_t* l_prev;
      while(l_iter > l_base) {
          l_prev = l_iter - 1;
          tft_put(*l_prev);
          l_iter = l_prev;
          nop();
      }
}

template<typename Xt, typename... Args>
void  tft_put(Xt* data, size_t size, Args&&... args) noexcept
{
      while(size) {
          tft_put(*data);
          data++;
          size--;
      }
}

template<typename... Args>
void  tft_recv(uint8_t cmd, Args&&... args) noexcept
{
      cs_set_l();
      dc_set_l();
      tft_put(cmd);
      dc_set_h();
      gpio_set_dir_in_masked(bit_data);
      tft_get(std::forward<Args>(args)...);
      gpio_set_dir_out_masked(bit_data);
      nop(2);
      cs_set_h();
}

template<typename... Args>
void  tft_send(uint8_t cmd, Args&&... args) noexcept
{
      cs_set_l();
      dc_set_l();
      tft_put(cmd);
      dc_set_h();
      tft_put(std::forward<Args>(args)...);
      nop(2);
      cs_set_h();
}

void  tft_nop() noexcept
{
      tft_send(TFT_NOP);
}

void  tft_swrst() noexcept
{
      tft_send(TFT_SWRST);
      sleep_ms(5);
}

void  tft_get_info() noexcept
{
      unsigned char p0;
      unsigned char p1;
      unsigned char p2;
      unsigned char p3;
      tft_recv(0x04, p0, p1, p2, p3);
      nop();
}

void  tft_get_status() noexcept
{
      unsigned char p1;
      unsigned char p2;
      unsigned char p3;
      unsigned char p4;
      unsigned char p5;
      tft_recv(0x09, p1, p2, p3, p4, p5);
      nop();
}

void  tft_get_power_mode() noexcept
{
      unsigned char p1;
      unsigned char p2;
      tft_recv(0x0a, p1, p2);
      nop();
}

void  tft_madctl(uint8_t value) noexcept
{
      tft_send(0x36, value);
}

void  tft_caset(uint16_t px0, uint16_t px1) noexcept
{
      tft_send(0x2a, px0, px1);
}

void  tft_raset(uint16_t py0, uint16_t py1) noexcept
{
      tft_send(0x2b, py0, py1);
}

void  tft_blt(uint16_t* p, size_t size) noexcept
{
      uint16_t colour = 0xffff;
      tft_send(0x2c);
      if(size) {
          cs_set_l();
          nop();
          wr_set_l();
          nop();
          while(size) {
              tft_put(colour);      
              size--;
          }
          wr_set_h();
          nop();
          cs_set_h();
          nop();
      }
}

void  tft_reset() noexcept
{
      rst_set_l();
      nop(32);
      rst_set_h();
      sleep_ms(125);
}

bool  tft_initialise() noexcept
{
      tft_send(0xef, 0x03, 0x80, 0x02);
      tft_send(0xcf, 0x00, 0xc1, 0x30);
      tft_send(0xed, 0x64, 0x03, 0x12, 0x81);
      tft_send(0xe8, 0x85, 0x00, 0x78);               // driver timing control A
      tft_send(0xcb, 0x39, 0x2c, 0x00, 0x34, 0x02);   // extended power control A
      tft_send(0xf7, 0x20);                           // pump ratio control
      tft_send(0xea, 0x00, 0x00);                     // driver timing control B
      tft_send(ILI9341_PWCTR1, 0x23);
      tft_send(ILI9341_PWCTR2, 0x10);
      tft_send(ILI9341_VMCTR1, 0x3e, 0x28);
      tft_send(ILI9341_VMCTR2, 0x86);

      tft_send(ILI9341_MADCTL, 0x48);

      tft_send(ILI9341_PIXFMT, 0x55);                 // pixel format == 16 bit per colour
      tft_send(ILI9341_FRMCTR1, 0x00, 0x13);          // frame control: 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
      tft_send(ILI9341_DFUNCTR, 0x08, 0x82, 0x27);
      tft_send(0xf2, 00);                             // 3gamma function disable
      tft_send(ILI9341_GAMMASET, 0x01);
      tft_send(ILI9341_GMCTRP1, 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00);
      tft_send(ILI9341_GMCTRN1, 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f);
    
      tft_send(ILI9341_SLPOUT);
      sleep_ms(125);
      tft_send(ILI9341_DISPON);
      sleep_ms(125);
      return true;
}

bool  tft_dispose() noexcept
{
      return true;
}

void  initialise() noexcept
{
      gpio_init_mask(bit_cs | bit_rd | bit_wr | bit_dc | bit_rst);
      gpio_set_dir_out_masked(bit_cs | bit_rd | bit_wr | bit_dc | bit_rst);
      gpio_set_mask(bit_cs | bit_rd | bit_wr | bit_dc | bit_rst);
      gpio_init_mask(bit_data);
      tft_reset();
      gpio_set_dir_out_masked(bit_data);
      tft_swrst();
}

void  dispose() noexcept
{
      tft_dispose();
}

int   main()
{
      initialise();
      tft_initialise();
      tft_nop();
      tft_get_info();
      tft_get_status();
      tft_get_power_mode();

      while(true) {
          tft_send(ILI9341_CASET, 0x00, 0x00, 0x00, 0xef);
          tft_send(ILI9341_PASET, 0x00, 0x00, 0x00, 0xef);
          cs_set_l();
          dc_set_l();
          tft_put(ILI9341_RAMWR);
          dc_set_h();
          nop(2);
          for(int x = 0; x != 240 * 240; x++) {
              wr_set_l();
              gpio_set_mask(0b00000011 << 8);
              wr_set_h();
              gpio_clr_mask(bit_data);
              wr_set_l();
              gpio_set_mask(0b11100000 << 8);
              wr_set_h();
              gpio_clr_mask(bit_data);
              nop();
          }
          nop(2);
          cs_set_h();

      }
      dispose();
      return 0;
}
