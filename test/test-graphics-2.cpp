#include <stdlib.h>
#include <string.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/malloc.h>
#include "lcd-config.h"
#include "lcd-setup.h"
#include <utility>

/* test_graphics_2
   full screen redraw 240 scan lines synchronised at 70FPS
   provided you can fetch 8 lines at a time (always true if running a framebuffer), this
   is a better idea than drawing each scan line in its own slot, because there is a small overhead
   to setup the display draw window (at least 700 or 800us to invoke caset(), raset() and memrw())
*/
static constexpr unsigned int bit_cs   = 1 << TFT_CS;
static constexpr unsigned int bit_dc   = 1 << TFT_DC;
static constexpr unsigned int bit_rd   = 1 << TFT_RD;
static constexpr unsigned int bit_wr   = 1 << TFT_WR;
static constexpr unsigned int bit_rst  = 1 << TFT_RST;
static constexpr unsigned int bit_dbg  = 1 << TFT_DBG;
static constexpr unsigned int bit_data = 0x0000ff00;     // need to move this bitch up 8 bits to free the pins for SPI1

static constexpr int g_screen_width = 320;
static constexpr int g_screen_height = 240;
static constexpr int g_screen_slice = 8;

static uint16_t      g_screen_ptr[g_screen_width * g_screen_height];

void  nop() noexcept
{
      asm volatile("nop\n");
}

inline void  nop(int wait) noexcept
{
      while(wait > 0) {
          nop();
          wait--;
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

inline void  tft_get() noexcept
{
}

template<typename... Args>
inline void  tft_get(uint8_t& data, Args&... args) noexcept
{
      rd_set_l();
      data = (gpio_get_all() & bit_data) >> 8;
      rd_set_h();
      tft_get(args...);
}

inline void  tft_put() noexcept
{
}

template<typename... Args>
inline void  tft_put(uint8_t data, Args&&... args) noexcept
{
      wr_set_l();
      gpio_set_mask(data << 8);
      wr_set_h();
      gpio_clr_mask(bit_data);
      tft_put(std::forward<Args>(args)...);
}

template<typename Xt, typename... Args>
inline void  tft_put(Xt&& data, Args&&... args) noexcept
{
      if constexpr (std::is_same_v<std::remove_cv_t<Xt>, uint16_t>) {
          wr_set_l();
          gpio_set_mask(data & 0xff00);
          wr_set_h();
          gpio_clr_mask(bit_data);
          wr_set_l();
          gpio_set_mask((data & 0x00ff) << 8);
          wr_set_h();
          gpio_clr_mask(bit_data);
      } else
      if constexpr (sizeof(data) >= 2) {
          size_t   l_size = sizeof(Xt);
          uint8_t* l_base = reinterpret_cast<uint8_t*>(std::addressof(data));
          uint8_t* l_iter = l_base + l_size;
          uint8_t* l_prev;
          while(l_iter > l_base) {
              l_prev = l_iter - 1;
              tft_put(*l_prev);
              l_iter = l_prev;
          }
      }
      tft_put(std::forward<Args>(args)...);
}

template<typename... Args>
void  tft_recv(uint8_t cmd, Args&... args) noexcept
{
      cs_set_l();
      dc_set_l();
      tft_put(cmd);
      dc_set_h();
      gpio_set_dir_in_masked(bit_data);
      tft_get(args...);
      gpio_set_dir_out_masked(bit_data);
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

int   tft_get_scanline() noexcept
{
      unsigned char p1;
      unsigned char p2;
      unsigned char p3;
      tft_recv(0x45, p1, p2, p3);
      return ((p2 << 8) | p3);
}

void  tft_get_power_mode() noexcept
{
      unsigned char p1;
      unsigned char p2;
      tft_recv(0x0a, p1, p2);
      nop();
}

void  tft_caset(uint16_t px0, uint16_t px1) noexcept
{
      tft_send(0x2a, px0, px1);
}

void  tft_raset(uint16_t py0, uint16_t py1) noexcept
{
      tft_send(0x2b, py0, py1);
}

void  tft_fill(uint16_t colour, size_t size) noexcept
{
      cs_set_l();
      dc_set_l();
      tft_put(ILI9341_RAMWR);
      dc_set_h();
      for(size_t index = 0; index < size; index++) {
          tft_put(colour);
      }
      cs_set_h();
}

// void  tft_blt(uint8_t* image, size_t size) noexcept
// {
//       cs_set_l();
//       dc_set_l();
//       tft_put(ILI9341_RAMWR);
//       dc_set_h();
//       //
//       cs_set_h();
// }

void  tft_blt(uint16_t* image, size_t size) noexcept
{
      cs_set_l();
      dc_set_l();
      tft_put(ILI9341_RAMWR);
      dc_set_h();
      if(true) {
          auto  l_iter = image;
          auto  l_last = image + size;
          while(l_iter < l_last) {
              gpio_clr_mask(bit_wr);
              gpio_set_mask(*l_iter & 0xff00);
              gpio_set_mask(bit_wr);
              gpio_clr_mask(bit_data);
              gpio_clr_mask(bit_wr);
              gpio_set_mask((*l_iter & 0x00ff) << 8);
              gpio_set_mask(bit_wr);
              gpio_clr_mask(bit_data);
              ++l_iter;
          }
      }
      cs_set_h();
}

void  tft_reset() noexcept
{
      rst_set_l();
      nop(32);
      rst_set_h();
      sleep_ms(125);
}

void  tft_int() noexcept
{
}

void  tft_core1_boot() noexcept
{
      int             l_line_ctr = 0;
      int             l_frame_ctr = 0;
      uint16_t*       l_line_ptr = std::addressof(g_screen_ptr[0]);
      uint16_t        l_bad_line;

      absolute_time_t l_mark_0;
      absolute_time_t l_mark_1;
      uint64_t        l_busy;
      uint64_t        l_sleep;

      // should be: 1'000'000 / FPS / g_screen_height
      // at an FPS of 70 (set by ILI9341_FRMCTR1) that equates to 59.52us for a single scan line
      // for our current preddicament of 8 (g_screen_slice) scanlines, that will round up to 476us
      uint64_t        l_delay = 1000000 * g_screen_slice / 70 / g_screen_height;

      // set column address
      tft_caset(0, g_screen_width - 1);

      // process framebuffer
      while(true) {
          gpio_set_mask(bit_dbg);
          l_mark_0 = get_absolute_time();
          tft_raset(l_line_ctr, l_line_ctr + g_screen_slice - 1);
          tft_blt(l_line_ptr, g_screen_width * g_screen_slice);
          l_line_ctr += g_screen_slice;
          l_line_ptr += g_screen_width * g_screen_slice;
          if(l_line_ctr >= g_screen_height) {
              l_line_ctr = 0;
              l_line_ptr = std::addressof(g_screen_ptr[0]);
              l_frame_ctr++;
              if(l_frame_ctr == 70) {
                  l_frame_ctr = 0;
              }
          }
          l_mark_1 = get_absolute_time();
          l_busy   = absolute_time_diff_us(l_mark_0, l_mark_1);
          gpio_clr_mask(bit_dbg);
          if(l_busy < l_delay) {
              l_sleep = l_delay - l_busy;
              busy_wait_us_32(l_sleep);
          }
      }
};

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

      tft_send(ILI9341_MADCTL, 48);

      tft_send(ILI9341_PIXFMT, 0x55);                 // pixel format == 16 bit per colour
      tft_send(ILI9341_FRMCTR1, 0x00, 0x1b);          // frame control: 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
      tft_send(ILI9341_DFUNCTR, 0x08, 0x82, 0x27);
      tft_send(0xf2, 00);                             // 3gamma function disable
      tft_send(ILI9341_GAMMASET, 0x01);
      tft_send(ILI9341_GMCTRP1, 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00);
      tft_send(ILI9341_GMCTRN1, 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f);
    
      tft_send(ILI9341_SLPOUT);
      tft_send(ILI9341_DISPON);
      sleep_ms(125);

      // reserve framebuffer memory
      // g_screen_ptr = reinterpret_cast<uint8_t*>(malloc(g_screen_width * g_screen_height));
    
      // launch the display process
      multicore_launch_core1(tft_core1_boot);
      return true;
}

bool  tft_dispose() noexcept
{
      // if(g_screen_ptr) {
      //     free(g_screen_ptr);
      // }
      return true;
}

void  initialise() noexcept
{
      gpio_init_mask(bit_cs | bit_rd | bit_wr | bit_dc | bit_rst | bit_dbg);
      gpio_set_dir_out_masked(bit_cs | bit_rd | bit_wr | bit_dc | bit_rst | bit_dbg);
      gpio_set_mask(bit_cs | bit_rd | bit_wr | bit_dc | bit_rst);
      gpio_init_mask(bit_data);
      tft_reset();
      gpio_set_dir_out_masked(bit_data);
      // set_sys_clock_khz(125000, true);
      tft_swrst();
}

void  dispose() noexcept
{
      tft_dispose();
}

void  test_solid(int cycles)
{
      int16_t l_colour = rand() & 65536;
      for(int index = 0; index < cycles; index++) {
          for(int l_pixel = 0; l_pixel < g_screen_width * g_screen_height; l_pixel++) {
              g_screen_ptr[l_pixel] = l_colour;
          }
      }
};

int   main()
{
      initialise();
      tft_initialise();
      tft_nop();
      tft_get_info();
      tft_get_status();
      tft_get_power_mode();

      // bunch of graphics tests
      int l_test_id = 0;
      while(true) {
          switch(l_test_id) {
            case 0:
                test_solid(100);
                break;
            default:
                l_test_id = 0;
                break;
          }
      }
      dispose();
      return 0;
}
