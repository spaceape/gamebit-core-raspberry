#include "LCD_ILI9341.hpp"
#include "LCDConfig.hpp"
#include "pico/stdlib.h"
#ifdef  LCD_INTERFACE_SPI
#include "hardware/spi.h"
#endif
#include "hardware/gpio.h"
#include <stdio.h>

#define ILI9341_WIDTH       240
#define ILI9341_HEIGHT      320

#define ILI9341_NOP         0x00 // No-operation
#define ILI9341_SWRESET     0x01 // Software reset register
#define ILI9341_RDDID       0x04 // Read identification information
#define ILI9341_RDDST       0x09 // Read status

#define ILI9341_SLPIN       0x10 // Enter sleep mode
#define ILI9341_SLPOUT      0x11 // Exit sleep mode
#define ILI9341_PTLON       0x12 // Partial mode on
#define ILI9341_NORON       0x13 // Normal display mode on

#define ILI9341_RDMODE      0x0A // Read power mode
#define ILI9341_RDMADCTL    0x0B // Read MADCTL
#define ILI9341_RDPIXFMT    0x0C // Read pixel format
#define ILI9341_RDIMGFMT    0x0D // Read image format
#define ILI9341_RDSELFDIAG  0x0F // Read self-diagnostic result

#define ILI9341_INVOFF      0x20 // Inversion OFF
#define ILI9341_INVON       0x21 // Inversion ON
#define ILI9341_GAMMASET    0x26 // Gamma set
#define ILI9341_DISPOFF     0x28 // Display OFF
#define ILI9341_DISPON      0x29 // Display ON

#define ILI9341_CASET       0x2A // Column address set
#define ILI9341_PASET       0x2B // Page address set
#define ILI9341_RAMWR       0x2C // Memory write
#define ILI9341_RAMRD       0x2E // Memory read

#define ILI9341_PTLAR       0x30 // Partial area
#define ILI9341_VSCRDEF     0x33 // Vertical scrolling definition
#define ILI9341_MADCTL      0x36 // Memory access control
#define ILI9341_VSCRSADD    0x37 // Vertical scrolling start address
#define ILI9341_PIXFMT      0x3A // COLMOD: pixel format set

#define ILI9341_FRMCTR1     0xB1 // Frame rate control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2     0xB2 // Frame rate control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3     0xB3 // Frame rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR      0xB4 // Display inversion control
#define ILI9341_DFUNCTR     0xB6 // Display function control

#define ILI9341_PWCTR1      0xC0 // Power control 1
#define ILI9341_PWCTR2      0xC1 // Power control 2
#define ILI9341_PWCTR3      0xC2 // Power control 3
#define ILI9341_PWCTR4      0xC3 // Power control 4
#define ILI9341_PWCTR5      0xC4 // Power control 5
#define ILI9341_VMCTR1      0xC5 // VCOM control 1
#define ILI9341_VMCTR2      0xC7 // VCOM control 2

#define ILI9341_RDID1       0xDA // Read ID 1
#define ILI9341_RDID2       0xDB // Read ID 2
#define ILI9341_RDID3       0xDC // Read ID 3
#define ILI9341_RDID4       0xDD // Read ID 4

#define ILI9341_GMCTRP1     0xE0 // Positive gamma correction
#define ILI9341_GMCTRN1     0xE1 // Negative gamma correction
//#define ILI9341_PWCTR6     0xFC

#define ILI9341_MADCTL_MY   0x80 // Bottom to top
#define ILI9341_MADCTL_MX   0x40 // Right to left
#define ILI9341_MADCTL_MV   0x20 // Reverse mode
#define ILI9341_MADCTL_ML   0x10 // LCD refresh bottom to top
#define ILI9341_MADCTL_RGB  0x00 // Red-Green-Blue
#define ILI9341_MADCTL_BGR  0x08 // Blue-Green-Red
#define ILI9341_MADCTL_MH   0x40 // LCD refresh right to left

#define ILI9341_IFCTL       0xF6

#ifndef LCD_INTERFACE_SPI
// #define LCD_DATA_MASK(s)   (s << LCD_PIN_D0)
// #define LCD_PORT_MASK      LCD_DATA_MASK(255)
#define LCD_CTRL_MASK (\
  (1 << LCD_PIN_CS)  | \
  (1 << LCD_PIN_DC)  | \
  (1 << LCD_PIN_RD)  | \
  (1 << LCD_PIN_WR)  | \
  (1 << LCD_PIN_RST)   \
)
#define LCD_DATA_MASK  (255 << LCD_PIN_D0)
#define LCD_DATA_BITS(bits)  ((bits << LCD_PIN_D0) && LCD_DATA_MASK)
#endif

void LCD_ILI9341::lcd_send(uint8_t command, uint32_t argc, const uint8_t* argv) {
#if defined(LCD_INTERFACE_SPI)
  gpio_put(LCD_PIN_CS, 0);
  asm volatile("nop \n nop \n nop");
	// Send command
	gpio_put(LCD_PIN_DC, 0);
	spi_write_blocking(LCD_SPI_PORT, &command, 1);
	gpio_put(LCD_PIN_DC, 1);
	
	// Send parameters
	spi_write_blocking(LCD_SPI_PORT, argv, argc);
  gpio_put(LCD_PIN_CS, 1);
  asm volatile("nop \n nop \n nop");
#else
  gpio_put(LCD_PIN_CS, 0);
	gpio_put(LCD_PIN_DC, 0);
  gpio_put(LCD_PIN_WR, 0);
  gpio_set_mask(command << LCD_PIN_D0);
  gpio_put(LCD_PIN_WR, 1);
  gpio_clr_mask(LCD_DATA_MASK);
	gpio_put(LCD_PIN_DC, 1);
	for(unsigned int index = 0; index < argc; ++index) {
      gpio_put(LCD_PIN_WR, 0);
      gpio_set_mask(argv[index] << LCD_PIN_D0);
      gpio_put(LCD_PIN_WR, 1);
    	gpio_clr_mask(LCD_DATA_MASK);
	}
  gpio_put(LCD_PIN_CS, 1);
#endif
}

LCD_ILI9341::LCD_ILI9341() {
	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = ILI9341_WIDTH;
	viewport[3] = ILI9341_HEIGHT;
	
	rotation = 0;
}

LCD_ILI9341::~LCD_ILI9341() {
}

uint32_t LCD_ILI9341::getBaudrate() const {
	return baudrate;
}

void LCD_ILI9341::setViewport(uint16_t x, uint16_t y, uint16_t z, uint16_t w) {
	if ((viewport[0] == x) &&
		(viewport[1] == y) &&
		(viewport[2] == z) &&
		(viewport[3] == w)) {
		return;
	}

	const uint8_t caset_data[4] = {(x >> 8) & 0xFF, x & 0xFF, (z >> 8) & 0xFF, z & 0xFF};
	const uint8_t paset_data[4] = {(y >> 8) & 0xFF, y & 0xFF, (w >> 8) & 0xFF, w & 0xFF};

	lcd_send(ILI9341_CASET, 4, caset_data);
	lcd_send(ILI9341_PASET, 4, paset_data);
	
	viewport[0] = x;
	viewport[1] = y;
	viewport[2] = z;
	viewport[3] = w;	
}

uint16_t LCD_ILI9341::getWidth() const {
	return viewport[2] - viewport[0];
}

uint16_t LCD_ILI9341::getHeight() const {
	return viewport[2] - viewport[0];
}

void LCD_ILI9341::setRotation(uint8_t value) {
	if (rotation == value) {
		return;
	}
	
	uint16_t width;
	uint16_t height;
	uint8_t madctl;

	switch (value % 4) {
	case 0 :
		madctl = ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR;
		width = ILI9341_WIDTH;
		height = ILI9341_HEIGHT;
		break;
	case 1 :
		madctl = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
		width = ILI9341_HEIGHT;
		height = ILI9341_WIDTH;
		break;
	case 2 :
		madctl = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR;
		width = ILI9341_WIDTH;
		height = ILI9341_HEIGHT;
		break;
	case 3 :
		madctl = ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
		width = ILI9341_HEIGHT;
		height = ILI9341_WIDTH;
		break;
	default:
		return;
	}

	lcd_send(ILI9341_MADCTL, 1, &madctl);
	setViewport(0, 0, width - 1, height - 1);
	
	rotation = value;
}

uint8_t LCD_ILI9341::getRotation() const {
	return rotation;
}

int LCD_ILI9341::initialize() {
	const uint32_t TARGET_BAUDRATE = 32 * 1024 * 1024; // 31250000
	const uint8_t positive_gama_correction[] = {0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00};
	const uint8_t negative_gama_correction[] = {0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f};
#if defined(LCD_INTERFACE_SPI)
  spi_init(LCD_SPI_PORT, TARGET_BAUDRATE);
  baudrate = spi_set_baudrate(LCD_SPI_PORT, TARGET_BAUDRATE);
  sleep_ms(100);

  gpio_set_function(LCD_PIN_MISO, GPIO_FUNC_SPI);
  gpio_set_function(LCD_PIN_CLK,  GPIO_FUNC_SPI);
  gpio_set_function(LCD_PIN_MOSI, GPIO_FUNC_SPI);

#else
	baudrate = 0;
  // gpio_init_mask(LCD_CTRL_MASK);
  // gpio_set_dir_out_masked(bit_cs | bit_rd | bit_wr | bit_dc | bit_rst);
  // gpio_set_mask(bit_cs | bit_rd | bit_wr | bit_dc | bit_rst);
  // gpio_init_mask(bit_data);
  // gpio_set_dir_out_masked(bit_data);

	gpio_init_mask(LCD_CTRL_MASK | LCD_DATA_MASK);
	gpio_set_dir_out_masked(LCD_CTRL_MASK);
	gpio_set_dir_out_masked(LCD_DATA_MASK);
  gpio_put(LCD_PIN_RD, 1);
  gpio_put(LCD_PIN_WR, 1);
  gpio_clr_mask(LCD_DATA_MASK);
#endif

  // Chip select is active-low, so we'll initialise it to a driven-high state
  gpio_init(LCD_PIN_CS);
  gpio_set_dir(LCD_PIN_CS, GPIO_OUT);
  gpio_put(LCD_PIN_CS, 1);

  // Reset is active-low
  gpio_init(LCD_PIN_RST);
  gpio_set_dir(LCD_PIN_RST, GPIO_OUT);
  gpio_put(LCD_PIN_RST, 1);

  // high = data, low = command
  gpio_init(LCD_PIN_DC);
  gpio_set_dir(LCD_PIN_DC, GPIO_OUT);
  gpio_put(LCD_PIN_DC, 1);

	reset(true, true);

	lcd_send(ILI9341_GAMMASET, 1, (const uint8_t[1]){0x01});
	lcd_send(ILI9341_GMCTRP1, 15, positive_gama_correction);
  lcd_send(ILI9341_GMCTRN1, 15, negative_gama_correction);
	lcd_send(ILI9341_MADCTL,   1, (const uint8_t[1]){ILI9341_MADCTL_MX});
	lcd_send(ILI9341_PIXFMT,   1, (const uint8_t[1]){0x55});
	lcd_send(ILI9341_FRMCTR1,  2, (const uint8_t[2]){0x00, 0x08});
	lcd_send(ILI9341_SLPOUT);
  lcd_send(ILI9341_DISPON);

	setViewport(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT);
	return 0;
}

void LCD_ILI9341::reset(bool soft, bool hard) {
	if (hard) {
		gpio_put(LCD_PIN_RST, 0);
		sleep_ms(10);
		gpio_put(LCD_PIN_RST, 1);
    sleep_ms(240);
	}

	if (soft) {
		lcd_send(ILI9341_SWRESET);
		sleep_ms(100);
	}
}

void LCD_ILI9341::blit(const uint8_t* data, uint32_t length) {
	lcd_send(ILI9341_RAMWR, length, data);
}