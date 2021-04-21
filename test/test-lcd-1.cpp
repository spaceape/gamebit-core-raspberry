#include <stdio.h>
#include <dev/lcd.h>
#include <dev/lcd/ili9341.h>
#include <dev/lcd/gfx/scene.h>
#include <dev/lcd/gfx/canvas.h>
#include "pico/stdlib.h"

void  test_lcd_01() noexcept
{
      dev::ili9341 l_lcd(128, 160);
      while(true) {
          int l_colour = rand() % 65536;
          for(int py = 0; py < 240; py++) {
              for(int dt = 0; dt < 128; dt++) {
                  l_lcd.fill(0, py, 320, py + 1, l_colour);
              }
          }
      }
}

void  test_lcd_02() noexcept
{
      dev::ili9341     l_lcd;
      // dev::gfx::scene&  l_scene = l_lcd.get_device();
      // dev::gfx::canvas* l_canvas = l_scene.make_canvas(dev::gfx::fmt_indexed_xcs, -16, -4, 32, 8);

      // auto l_lb = l_canvas->get_lb_ptr();
      // auto l_hb = l_canvas->get_hb_ptr();
      // auto l_fx = l_canvas->get_fx_ptr();
      // l_lb[0] = 'A';
      // l_lb[1] = 0;
      // l_lb[2] = 0;
      // l_lb[3] = 0;
      // l_hb[0] = 0;
      // l_fx[0] = 0;
      // l_fx[1] = 0;

      // while(true) {
      //     l_lcd.sync(0.01);
      //     l_lcd.render();
      //     sleep_ms(10);
      // }
}

int   main(int, char**)
{
      test_lcd_01();
      return 0;
}
