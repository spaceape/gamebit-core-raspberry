#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/spi.h>
#include "test-spi-sd.h"
#include <cstring>
#include <cstdint>
#include <utility>

static constexpr unsigned int sdc_pin_ss  = 13;
static constexpr unsigned int sdc_pin_rx  = 12;
static constexpr unsigned int sdc_pin_tx  = 11;
static constexpr unsigned int sdc_pin_sck = 10;

static constexpr bool         sdc_use_crc = false;
static constexpr bool         sdc_use_dma = false;

inline void  sdc_select() noexcept
{
      gpio_put(sdc_pin_ss, 0);
}

inline void  sdc_deselect() noexcept
{
      gpio_put(sdc_pin_ss, 1);
}

inline  void sdc_put(std::uint8_t*) noexcept
{
}

template<typename Xt, typename... Args>
inline  void sdc_put(std::uint8_t* p, Xt value, Args&&... args) noexcept
{
        std::memcpy(p, std::addressof(value), sizeof(value));
        sdc_put(p + sizeof(Xt), std::forward<Args>(args)...);
}

inline  std::size_t sdc_get_size() noexcept
{
      return 0;
}

template<typename Xt, typename... Args>
inline  std::size_t sdc_get_size(Xt& value, Args... args) noexcept
{
      return sizeof(value) + sdc_get_size(std::forward<Args>(args)...);
}

void  sdc_hold() noexcept
{
      std::uint8_t  l_recv[16];
      sdc_deselect();
      spi_read_blocking(spi1, 0xff, l_recv, sizeof(l_recv));
      sdc_deselect();
}

sd_result_t sdc_send_command(std::uint8_t cmd, std::uint32_t value = 0) noexcept
{
      sd_result_t   l_result;
      std::size_t   l_size = 6;
      std::uint8_t  l_data[16];
  
      sdc_put(l_data, cmd | sd_tag, value);
      if(cmd == sd_cmd_reset) {
          if(value == 0) {
              l_data[l_size - 1] = 0x95;
          }
      } else
      if(cmd == sd_cmd_8) {
          if(value == 0x01aa) {
              l_data[l_size - 1] = 0x87;
          }
      } else
      if constexpr (sdc_use_crc) {
      } else
          l_data[l_size - 1] = 1;

      sdc_select();
      spi_write_blocking(spi1, l_data, l_size);
      spi_read_blocking(spi1, 0xff, l_data, 16);
      sdc_deselect();
      if(true) {
          int l_rx = 0;
          int l_dx = 0;
          while(l_dx < 16) {
              if(l_data[l_dx] != 255) {
                  while(l_rx < 4) {
                      l_result.m_byte[l_rx] = l_data[l_dx];
                      l_rx++;
                      l_dx++;
                  }
                  if constexpr (sdc_use_crc) {
                  }
                  break;
              }
              l_dx++;
          }
      }
      return l_result;
}

void  sdc_init() noexcept
{
      spi_init(spi1, 1000000);
      gpio_init(sdc_pin_ss);
      gpio_set_dir(sdc_pin_ss, GPIO_OUT);
      gpio_put(sdc_pin_ss, 1);
      gpio_set_function(sdc_pin_rx, GPIO_FUNC_SPI);
      gpio_set_function(sdc_pin_tx, GPIO_FUNC_SPI);
      gpio_set_function(sdc_pin_sck, GPIO_FUNC_SPI);

      // send 80 clock cycles to place the SD card in SPI mode
      sdc_hold();

      // send reset (CMD_0)
      auto l_result = sdc_send_command(sd_cmd_reset, 0);
      if(l_result.m_r1.b_idle) {
          printf("> sd rst: %.2x [OK]\n", l_result.m_value);
      }

    // // Grab some unused dma channels
    // const uint dma_tx = dma_claim_unused_channel(true);
    // const uint dma_rx = dma_claim_unused_channel(true);
}

void  sdc_free() noexcept
{
      spi_deinit(spi1);
      gpio_put(sdc_pin_ss, 1);
      gpio_set_dir(sdc_pin_ss, GPIO_IN);
}

int   main(int, char**)
{
      stdio_init_all();
      printf("Ohai");
      sdc_init();
      sdc_free();
      return 0;
}
