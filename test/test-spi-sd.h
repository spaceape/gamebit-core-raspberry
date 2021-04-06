#ifndef test_sd_h
#define test_sd_h
#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <cstdint>
#include <utility>

static constexpr std::uint8_t sd_tag = 0x40;
static constexpr std::uint8_t sd_cmd_reset = 0x00;  // place SD Card in idle state
static constexpr std::uint8_t sd_cmd_8 = 0x08;      //
static constexpr std::uint8_t sd_cmd_59 = 0x59;     // CRC ON/OFF

union  sd_result_t
{
  struct {
    std::uint8_t  b_idle:1;
    std::uint8_t  b_erase_reset:1;
    std::uint8_t  b_illegal_command:1;
    std::uint8_t  b_crc_error:1;
    std::uint8_t  b_erase_sequence_error:1;
    std::uint8_t  b_address_error:1;
    std::uint8_t  b_parameter_error:1;
    std::uint8_t  b_zero:1;
  } m_r1;
  std::uint8_t    m_byte[4];
  std::uint32_t   m_value;
};

#endif