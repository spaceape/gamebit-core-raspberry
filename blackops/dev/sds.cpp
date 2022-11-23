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
#include "sds.h"
#include <hardware.h>
#include "bd/raw/crc.h"

namespace dev {

// SD initialises at a lower rate, specify what that is
static constexpr unsigned int sdc_init_baud   = 32768;

// SD needs a bunch of wait cycles to retrieve the data for the read command
static constexpr unsigned int sdc_wait_cycles = 8192;

// some commands may initially fail (mandated by the SD specs); retry after this short delay
static constexpr unsigned int sdc_throttle_us = 128;

// timeout
static constexpr unsigned int sdc_timeout_us = 32768;

static constexpr bool         sdc_use_crc = false;
static constexpr bool         sdc_use_dma = false;

      sds::sds(spi* bus, unsigned int cs_pin) noexcept:
      bd(),
      m_io(bus, cs_pin),
      m_type(0),
      m_sdhc(false),
      m_sdxc(false),
      m_status(-1),
      m_ready_bit(false)
{
      m_io.set_rx_pattern(255);
      m_io.set_timeout(sdc_timeout_us);
      m_io.set_baud(sdc_init_baud);
      resume();
}

      sds::~sds()
{
      suspend();
}

bool  sds::bus_acquire() noexcept
{
      return m_io.acquire();
}

void  sds::bus_release() noexcept
{
      m_io.release();
}

/* bus_poll()
*/
void  sds::bus_poll(long int cycles) noexcept
{
      bus_acquire();
      m_io.recv(cycles);
      bus_release();
}

/* dev_send_command()
*/
auto  sds::dev_send_command(std::uint8_t cmd, std::uint32_t value) noexcept -> sd_result_t
{
      std::uint8_t  l_send[6];
      sd_result_t   l_result;

      cmd_put(l_send, cmd, value);

      // fix start and stop bits
      l_send[0] |= 0x40;
      l_send[5] |= 0x01;

      // some commands *always* require a CRC
      if(bool
          l_apply_crc =
              sdc_use_crc |
              (cmd == sd_cmd_0) ||
              (cmd == sd_cmd_8) ||
              (cmd == sd_cmd_55) ||
              (cmd == sd_acmd_41);
          l_apply_crc == true) {
          l_send[5] = get_crc7(l_send, sizeof(l_send));
      }

      // SPI transaction
      if(bus_acquire()) {
      // spi_write_blocking(m_spi, l_send, 6);
          int l_wait = 0;
          while(l_wait < 64) {
              // spi_read_blocking(m_spi, 255, std::addressof(l_result.m_data[0]), 1);
              if(l_result.m_data[0] != 255) {
                  // spi_read_blocking(m_spi, 255, std::addressof(l_result.m_data[1]), 4);
                  break;
              }
              l_wait++;
          }
          bus_release();
      }
      return l_result;
}

bool  sds::dev_load_cid() noexcept
{
      return false;
}

bool  sds::dev_load_csd() noexcept
{
      return false;
}

/* dev_read_sb()
*/
bool  sds::dev_load_sb(std::uint8_t* data, std::uint32_t sector) noexcept
{
      std::uint32_t l_address = sector;
      if(m_sdhc == false) {
          l_address = sector * 512;
      }
      std::uint8_t* l_cache = data;
      // std::uint8_t  l_crc[2];
      int           l_try = 4;
      int           l_wait = sdc_wait_cycles;
      bool          l_success = false;
      while(l_try) {
          sd_result_t l_result = dev_send_command(sd_cmd_17, l_address);
          if(l_result.m_data[0] == 0) {
              bus_acquire();
              while(l_wait) {
                  // spi_read_blocking(m_spi, 255, l_cache, 1);
                  if(l_cache[0] == sd_start_token) {
                      break;
                  }
                  l_wait--;
              }
              if(l_cache[0] == sd_start_token) {
                  // spi_read_blocking(m_spi, 255, l_cache, 512);
                  // spi_read_blocking(m_spi, 255, l_crc, 2);
                  // if constexpr (spio_use_crc) {
                  // } else
                  //     l_success = true;
              }
              bus_release();
              return l_success;
          } else
          if(l_result.m_data[0] != 0) {
              return false;
          }
          l_try--;
          if(l_try) {
              busy_wait_us_32(sdc_throttle_us);
          }
      }
      return false;
}

bool  sds::dev_load_mb(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept
{
      return false;
}

bool  sds::dev_save_sb(std::uint8_t*, std::uint32_t) noexcept
{
      return false;
}

bool  sds::dev_save_mb(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept
{
      return false;
}

/* dev_reset()
   send CMD_0, retry if fails
*/
bool  sds::dev_reset() noexcept
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

bool  sds::resume() noexcept
{
      if(m_ready_bit == false) {
          if(bool
              l_io_ready = m_io.is_ready();
              l_io_ready == false) {
              return false;
          }
          // poll the data lines for at least 78 cycles (if I recall)
          busy_wait_us(2048);
          bus_poll(128);
          busy_wait_us(6144);

          // reset the SD Card and enable SPI mode
          if(bool
              l_sdc_reset = dev_reset();
              l_sdc_reset == true) {
              std::uint32_t l_wait = 255;
              std::uint32_t l_mode = 0;
              sd_result_t   l_result;
              // enable CRC, if so configured
              if constexpr (sdc_use_crc) {
                  l_result = dev_send_command(sd_cmd_59, 1);
                  if(l_result.response1.b_idle != 1) {
                      return false;
                  }
              }
              // send interface conditions
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
              // if(l_result.response1.b_idle == 0) {
              //     l_result = dev_send_command(sd_cmd_16, 512);
              //     if(baud > spio_init_baud) {
              //         spi_set_baudrate(m_spi, baud);
              //     } 
              //     m_status = s_ready;
              //     m_ready  = true;
              // } else
              //     return false;
        } else
            return false;
      }
      return true;
}

bool  sds::load(std::uint8_t* data, std::uint32_t sector, std::uint32_t size) noexcept
{
      // std::uint8_t* l_load_ptr = data;
      // std::uint32_t l_load_sector = sector;
      // std::uint32_t l_load_size;
      // if(m_ready) {
      //     while(size) {
      //         if(size > 512) {
      //             l_load_size = 512;
      //         } else
      //             l_load_size = size;
      //         if(dev_load_sb(l_load_ptr, l_load_sector)) {
      //             l_load_ptr += l_load_size;
      //             l_load_sector += 1;
      //             size -= l_load_size;
      //         } else
      //             return false;
      //     }
      //     return true;
      // }
      return false;
}

bool  sds::save(std::uint8_t*, std::uint32_t, std::uint32_t) noexcept
{
      return false;
}

int   sds::get_status() const noexcept
{
      return m_status;
}

bool  sds::suspend() noexcept
{
      return false;
}

/*namespace dev*/ }
