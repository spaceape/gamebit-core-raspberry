#ifndef __LCD_ILI9341_HPP__
#define __LCD_ILI9341_HPP__

#include <stdint.h>

class LCD_ILI9341 {
	uint16_t viewport[4];
	uint8_t pixelFormat;
	uint8_t rotation;
	
	inline void lcd_send(uint8_t command, uint32_t argc = 0, const uint8_t* argv = (const uint8_t*)0);
	uint32_t baudrate;
	
public:
	
	LCD_ILI9341();
	
	~LCD_ILI9341();
	
	uint32_t getBaudrate() const;
	
	void setViewport(uint16_t x, uint16_t y, uint16_t z, uint16_t w);
	
	uint16_t getWidth() const;
	
	uint16_t getHeight() const;
	
	void setRotation(uint8_t rotation);
	
	uint8_t getRotation() const;
	
	int initialize();
	
	void reset(bool soft = true, bool hard = false);
	
	void blit(const uint8_t* data, uint32_t length);
};

#endif // __LCD_ILI9341_HPP__