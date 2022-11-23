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
#include "uart.h"
#include <hardware/uart.h>

namespace dev {

      uart::uart(uart_inst_t* uart, unsigned int rx_pin, unsigned int tx_pin) noexcept:
      device(type),
      ios(),
      m_uart(uart),
      m_mode(O_RDWR | O_NONBLOCK)
{
      uart_init(m_uart, uart_default_baud);
      gpio_set_function(rx_pin, GPIO_FUNC_UART);
      gpio_set_function(tx_pin, GPIO_FUNC_UART);
      uart_set_translate_crlf(m_uart, true);
}

      uart::~uart()
{
      uart_deinit(m_uart);
}

int   uart::get_char() noexcept
{
      if(m_mode & O_NONBLOCK) {
          if(uart_is_readable(m_uart) == false) {
              return EOF;
          }
      }
      return uart_getc(m_uart);
}

unsigned int  uart::get_byte() noexcept
{
      return uart_getc(m_uart);
}

int   uart::seek(int, int) noexcept
{
      return -1;
}

int   uart::read(int count) noexcept
{
      int l_read_count = 0;
      if(count > 0) {
          if(m_mode & O_NONBLOCK) {
              for(
                  l_read_count = 0;
                  uart_is_readable(m_uart) && (l_read_count < count);
                  l_read_count++) {
                  uart_getc(m_uart);
              }
          } else
          if(true) {
              for(l_read_count = 0; l_read_count < count; l_read_count++) {
                  uart_getc(m_uart);
              }
          }
      }
      return l_read_count;
}

int   uart::read(int count, char* data) noexcept
{
      int l_read_count = 0;
      if(count > 0) {
          if(m_mode & O_NONBLOCK) {
              for(
                  l_read_count = 0;
                  uart_is_readable(m_uart) && (l_read_count < count);
                  l_read_count++
              ) {
                  data[l_read_count] = uart_getc(m_uart);
              }
          } else
          if(true) {
              for(l_read_count = 0; l_read_count < count; l_read_count++) {
                  data[l_read_count] = uart_getc(m_uart);
              }
          }
      }
      return l_read_count;
}

int   uart::put_char(char value) noexcept
{
      uart_putc_raw(m_uart, value);
      return 1;
}

int   uart::put_byte(unsigned char value) noexcept
{
      uart_putc_raw(m_uart, static_cast<char>(value));
      return 1;
}

int   uart::write(int count, const char* data) noexcept
{
      uart_write_blocking(m_uart, reinterpret_cast<const std::uint8_t*>(data), count);
      return count;
}

int   uart::get_size() noexcept
{
      return 0;
}

void  uart::set_baud(long int value) noexcept
{
      uart_set_baudrate(m_uart, value);
}

void  uart::set_blocking(bool value) noexcept
{
      if(value) {
          m_mode &= ~O_NONBLOCK;
      } else
          m_mode |= O_NONBLOCK;
}

void  uart::set_nonblocking() noexcept
{
      set_blocking(false);
}

bool  uart::is_seekable() const noexcept
{
      return false;
}

bool  uart::is_readable() const noexcept
{
      return m_uart != nullptr;
}

bool  uart::is_writable() const noexcept
{
      return 
          (m_uart != nullptr) &&
          ((m_mode & O_WRONLY) != 0);
}

/*namespace dev*/ }
