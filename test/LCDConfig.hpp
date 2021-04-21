#ifndef __LCD_CONFIG_HPP__
#define __LCD_CONFIG_HPP__

// Commenting this out will switch to paralel interface instead of serial (SPI)
// #define LCD_INTERFACE_SPI

#if defined(LCD_INTERFACE_SPI)
	#define LCD_PIN_MISO  4
	#define LCD_PIN_MOSI  7
	#define LCD_PIN_DC    9
	#define LCD_PIN_CS    5
	#define LCD_PIN_RST   8
	#define LCD_PIN_CLK   6
	#define LCD_SPI_PORT spi0
#else
  
  // LCD control pins: where applicable, should be common with the SPI LCD interface
	#define LCD_PIN_CS    27
	#define LCD_PIN_DC    26
  #define LCD_PIN_RD    20
  #define LCD_PIN_WR    21

  // LCD data interface pins: should be a continuous range
  #define LCD_PIN_D0    12
  #define LCD_PIN_D1    13
  #define LCD_PIN_D2    14
  #define LCD_PIN_D3    15
  #define LCD_PIN_D4    16
  #define LCD_PIN_D5    17
  #define LCD_PIN_D6    18
  #define LCD_PIN_D7    19

  // LCD reset pin: should be common with the SPI LCD interface
	#define LCD_PIN_RST   22

#endif 

#endif // __LCD_CONFIG_HPP__