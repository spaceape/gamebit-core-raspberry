#ifndef core_hardware_h
#define core_hardware_h
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
#include "global.h"

// SD Card pins (SPI1)
#define PIN_SDC_CS  9
#define PIN_SDC_RX  8
#define PIN_SDC_TX  11
#define PIN_SDC_SCK 10
#define PIN_SDC_LED 25   // SD Card activity LED: 0 

// LCD pins (SPI0)
#define PIN_LCD_CS  5
#define PIN_LCD_RX  4
#define PIN_LCD_TX  7
#define PIN_LCD_SCK 6

// LCD control pins
// #define PIN_LCD_CS  27
#define PIN_LCD_DC  26
#define PIN_LCD_RD  20
#define PIN_LCD_WR  21

// LCD data interface pins: should be a continuous range
#define PIN_LCD_D0  12
#define PIN_LCD_D1  13
#define PIN_LCD_D2  14
#define PIN_LCD_D3  15
#define PIN_LCD_D4  16
#define PIN_LCD_D5  17
#define PIN_LCD_D6  18
#define PIN_LCD_D7  19

// LCD reset pin
#define PIN_LCD_RST 22

// debug pin
#define PIN_DBG     LED_PIN
#endif
