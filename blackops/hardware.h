#ifndef bops_hardware_h
#define bops_hardware_h
/** 
    Copyright (c) 2022, wicked systems
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
#include <sys.h>

/* hw_power_hard
*/
constexpr bool hw_power_hard = true;

/* hw_enable_uart0
*/
constexpr bool hw_enable_uart0 = true;

/* pin_uart0_*
   pin assignments for uart0
*/
constexpr unsigned int  pin_uart0_rx = 1;
constexpr unsigned int  pin_uart0_tx = 0;

/* hw_enable_uart1
*/
constexpr bool hw_enable_uart1 = false;

/* pin_uart1_*
   pin assignments for uart1
*/
constexpr unsigned int  pin_uart1_rx = pin_none;
constexpr unsigned int  pin_uart1_tx = pin_none;

/* hw_default_uart_baud
*/
constexpr long int  hw_default_uart_baud = 115200;

/* hw_enable_spi0
*/
constexpr bool  hw_enable_spi0 = true;

/* pin_spi0_*
   pin assignments for spi0
*/
constexpr unsigned int  pin_spi0_rx = 4;
constexpr unsigned int  pin_spi0_tx = 7;
constexpr unsigned int  pin_spi0_sck = 6;

/* hw_enable_spi1
*/
constexpr bool hw_enable_spi1 = true;

/* pin_spi1_*
   pin assignments for spi1
*/
constexpr unsigned int  pin_spi1_rx = 8;
constexpr unsigned int  pin_spi1_tx = 11;
constexpr unsigned int  pin_spi1_sck = 10;

/* hw_default_spi
   whether or not to add an ADT entry for a default 'spi' device and which spi instance that should point to
*/
constexpr auto hw_default_spi_index = -1;

/* hw_default_spi_baud
*/
constexpr long int  hw_default_spi_baud = 16777216;

/* hw_enable_sdc
   initialise SD card on the selected SPI interface
   requires: SPI
*/
constexpr bool hw_enable_sdc = false;
constexpr char hw_sdc_bus[]  = "/dev/spi";

/* pin_sdc_*
   default SD Card pins (SPI1)
*/
constexpr unsigned int  pin_sdc_cs = 9;
constexpr unsigned int  pin_sdc_led = PICO_DEFAULT_LED_PIN;   // SD Card activity LED (0 - disable)

/* hw_enable_lcd
   initialise LCD on the selected SPI interface
   requires: SPI
*/
constexpr bool hw_enable_lcd = true;
constexpr char hw_lcd_bus[]  = "/dev/spi0";

/* pin_lcd_*
   default LCD pins (SPI0)
*/
constexpr unsigned int  pin_lcd_cs = 5;
constexpr unsigned int  pin_lcd_ds = 9;
constexpr unsigned int  pin_lcd_reset = 8;

/* hw_enable_controller (always)
   boot up the EMC controller
   requires: UART
*/
// constexpr bool hw_enable_controller = true;
constexpr char hw_controller_io[]  = "/dev/uart0";

/* pin_dbg
   debug pin
*/
constexpr unsigned int  pin_dbg = 22;

#endif
