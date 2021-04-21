#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "LCD_ILI9341.hpp"

bool  test_lcd_01() noexcept
{
      uint16_t*   data = reinterpret_cast<uint16_t*>(malloc(320 * 240 * sizeof(uint16_t)));
      LCD_ILI9341 lcd;
      lcd.initialize();
      lcd.setViewport(0, 0, 239, 319);
      while(true) {
          int colour = rand() % 0x0000ffff;
          for(int t = 0; t != 32; t++) {
              for(int x = 0; x != 240 * 240; x++) {
                  // data[x] = 0b00000'111111'00000;
                  if(x & 8) {
                      data[x] = colour;
                  } else
                      data[x] = colour ^ 0xffff;
              }
          }
          lcd.blit((uint8_t*)data, 240 * 240 * 2);
      }
      return false;
}

int   main()
{
      test_lcd_01();
      return 0;
}
