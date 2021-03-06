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
#include "spio.h"
#include <hardware.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>

namespace dev {

// SD initialises at a lower rate, specify what that is
static constexpr unsigned int spio_init_baud   = 250000;

// SD needs a bunch of wait cycles to retrieve the data for the read command
static constexpr unsigned int spio_wait_cycles = 8192;

// some commands may initially fail (mandated by the SD specs); retry after this short delay
static constexpr unsigned int spio_throttle_us = 128;

static constexpr bool         spio_use_crc = false;
static constexpr bool         spio_use_dma = false;

      spio::spio(unsigned int baud) noexcept:
      bd(),
      m_spi(nullptr),
      m_type(0),
      m_sdhc(false),
      m_sdxc(false),
      m_status(-1),
      m_ready(false)
{
      if((PIN_SDC_TX == 3) ||
          (PIN_SDC_TX == 7) ||
          (PIN_SDC_TX == 19)) {
          spi_init(spi0, spio_init_baud);
          m_spi = spi0;
      } else
      if((PIN_SDC_TX == 11) ||
          (PIN_SDC_TX == 15)) {
          spi_init(spi1, spio_init_baud);
          m_spi = spi1;
      }
      gpio_init(PIN_SDC_CS);
      gpio_set_dir(PIN_SDC_CS, GPIO_OUT);
      gpio_put(PIN_SDC_CS, 1);
      gpio_set_function(PIN_SDC_RX, GPIO_FUNC_SPI);
      gpio_set_function(PIN_SDC_TX, GPIO_FUNC_SPI);
      gpio_set_function(PIN_SDC_SCK, GPIO_FUNC_SPI);
      if(PIN_SDC_LED) {
          gpio_init(PIN_SDC_LED);
          gpio_set_dir(PIN_SDC_LED, GPIO_OUT);
          gpio_put(PIN_SDC_LED, 0);
      }
      resume(baud);
}

      spio::~spio()
{
      suspend();
      if(m_spi) {
          spi_deinit(m_spi);
      }
}

void  spio::bus_select() const noexcept
{
      gpio_put(PIN_SDC_CS, 0);
      if(PIN_SDC_LED) {
          gpio_put(PIN_SDC_LED, 1);
      }
}

void  spio::bus_deselect() const noexcept
{
      if(PIN_SDC_LED) {
          gpio_put(PIN_SDC_LED, 0);
      }
      gpio_put(PIN_SDC_CS, 1);
}

/* bus_hold()
*/
void  spio::bus_hold() const noexcept
{
      std::uint8_t  l_recv[16];
      bus_deselect();
      spi_read_blocking(m_spi, 255, l_recv, sizeof(l_recv));
      bus_deselect();
}

/* dev_send_command()
*/
sd_result_t  spio::dev_send_command(std::uint8_t cmd, std::uint32_t value) const noexcept
{
      std::uint8_t  l_send[8];
      sd_result_t   l_result;

      cmd_put(l_send, cmd, value);

      // some commands *always* require a CRC, hardcoded here
      if(cmd == sd_cmd_0) {
          if(value == 0) {
              l_send[5] = 0x95;
          }
      } else
      if(cmd == sd_cmd_8) {
          if(value == 0x01aa) {
              l_send[5] = 0x87;
          }
      } else
      if(cmd == sd_cmd_55) {
          if(value == 0) {
              l_send[5] = 0x65;
          }
      } else
      if(cmd == sd_acmd_41) {
          if(value == 0) {
              l_send[5] = 0xe5;
          } else
          if(value == 0x40000000) {
              l_send[5] = 0x77;
          }
      } else
      if constexpr (spio_use_crc) {
      } else
          l_send[5] = 0;

      // fix start and stop bits
      l_send[0] |= 0x40;
      l_send[5] |= 0x01;

      // SPI transaction
      bus_select();
      spi_write_blocking(m_spi, l_send, 6);
      if(true) {
          int l_wait = 0;
          while(l_wait < 64) {
              spi_read_blocking(m_spi, 255, std::addressof(l_result.m_data[0]), 1);
              if(l_result.m_data[0] != 255) {
                  spi_read_blocking(m_spi, 255, std::addressof(l_result.m_data[1]), 4);
                  break;
              }
              l_wait++;
          }
      }
      bus_deselect();
      return l_result;
}

bool  spio::dev_load_cid() noexcept
{
      return false;
}

bool  spio::dev_load_csd() noexcept
{
      return false;
}

/* dev_read_sb()
*/
bool  spio::dev_load_sb(std::uint8_t* data, std::uint32_t sector) noexcept
{
      std::uint32_t l_address = sector;
      if(m_sdhc == false) {
          l_address = sector * 512;
      }
      std::uint8_t* l_cache = data;
      std::uint8_t  l_crc[2];
      int           l_try = 4;
      int           l_wait = spio_wait_cycles;
      bool          l_success = false;
      while(l_try) {
          sd_result_t l_result = dev_send_command(sd_cmd_17, l_address);
          if(l_result.m_data[0] == 0) {
              bus_select();
              while(l_wait) {
                  spi_read_blocking(m_spi, 255, l_cache, 1);
                  if(l_cache[0] == sd_start_token) {
                      break;
                  }
                  l_wait--;
              }
              if(l_cache[0] == sd_start_token) {
                  spi_read_blocking(m_spi, 255, l_cache, 512);
                  spi_read_blocking(m_spi, 255, l_crc, 2);
                  if constexpr (spio_use_crc) {
                  } else
                      l_success = true;
              }
              bus_deselect();
              return l_success;
          } else
          if(l_result.m_data[0] != 0) {
              return false;
          }
          l_try--;
          if(l_try) {
              busy_wait_us_32(spio_throttle_us);
          }
      }
      return false;
}

bool  spio::dev_load_mb(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept
{
      return false;
}

bool  spio::dev_save_sb(std::uint8_t*, std::uint32_t) noexcept
{
      return false;
}

bool  spio::dev_save_mb(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept
{
      return false;
}

/* dev_reset()
   send CMD_0, retry if fails
*/
bool  spio::dev_reset() const noexcept
{
      int         l_try = 0;
      sd_result_t l_result;
      while(l_try < 3) {
          l_result = dev_send_command(sd_cmd_0);
          if(l_result.response1.b_idle == 1) {
              return true;
          }
          // send "stop transfer" token
          // ...
          l_try++;
      }
      return false;
}

bool  spio::resume(unsigned int baud) noexcept
{
      if(m_ready == false) {
          busy_wait_us(2048);
          bus_hold();
          busy_wait_us(6144);
          if(dev_reset()) {
              std::uint32_t l_wait = 255;
              std::uint32_t l_mode = 0;
              sd_result_t   l_result;

              // enable CRC
              if constexpr (spio_use_crc) {
                  l_result = dev_send_command(sd_cmd_59, 1);
                  if(l_result.response1.b_idle != 1) {
                      return false;
                  }
              }

              // send interface conditions:
              l_result = dev_send_command(sd_cmd_8, 0x000001aa);
              if((l_result.response7.b_crc_error == 1) ||
                  (l_result.response7.b_parameter_error == 1)) {
                  return false;
              }
              if(l_result.response7.b_idle == 1) {
                  if(l_result.response7.b_echo == 0xaa) {
                      if(l_result.response7.b_power_good == 1) {
                          // determine the capacity class
                          l_result = dev_send_command(sd_cmd_58);
                          if(l_result.response3.b_idle == 1) {
                              l_mode = 0x40000000;
                              m_sdhc = l_result.response3.b_ccs;
                              m_sdxc = false;
                              m_type = 2;
                          } else
                              return false;
                      } else
                          return false;
                  } else
                      return false;
              } else
              if(l_result.response7.b_illegal_command == 1) {
                  m_type = 1;
              } else
                  return false;

              // wake the device up
              while(l_wait) {
                  l_result = dev_send_command(sd_cmd_55);
                  l_result = dev_send_command(sd_acmd_41, l_mode);
                  if((l_result.response1.b_illegal_command == 1) ||
                      (l_result.response1.b_crc_error == 1) ||
                      (l_result.response1.b_parameter_error == 1)) {
                      return false;
                  }
                  if(l_result.response1.b_idle == 0) {
                      break;
                  }
                  l_wait--;
              }
              if(l_result.response1.b_idle == 0) {
                  l_result = dev_send_command(sd_cmd_16, 512);
                  if(baud > spio_init_baud) {
                      spi_set_baudrate(m_spi, baud);
                  } 
                  m_status = s_ready;
                  m_ready  = true;
              } else
                  return false;
          } else
              return false;
      }
      return true;
}

bool  spio::load(std::uint8_t* data, std::uint32_t sector, std::uint32_t size) noexcept
{
      std::uint8_t* l_load_ptr = data;
      std::uint32_t l_load_sector = sector;
      std::uint32_t l_load_size;
      if(m_ready) {
          while(size) {
              if(size > 512) {
                  l_load_size = 512;
              } else
                  l_load_size = size;
              if(dev_load_sb(l_load_ptr, l_load_sector)) {
                  l_load_ptr += l_load_size;
                  l_load_sector += 1;
                  size -= l_load_size;
              } else
                  return false;
          }
          return true;
      }
      return false;
}

bool  spio::save(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept
{
      return false;
}

int   spio::get_status() const noexcept
{
      return m_status;
}

bool  spio::suspend() noexcept
{
      return false;
}

/*namespace dev*/ }
