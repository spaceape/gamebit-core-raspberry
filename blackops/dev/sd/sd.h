#ifndef bops_dev_sd_h
#define bops_dev_sd_h
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
#include <blackops.h>

namespace dev {

static constexpr std::uint8_t sd_cmd_0 = 0u;      // SEND_RESET: place SD Card in idle state
static constexpr std::uint8_t sd_cmd_8 = 8u;      // SEND_IF_CMD (v2.0)
static constexpr std::uint8_t sd_cmd_16 = 16u;    // SET_BLOCK_LENGTH
static constexpr std::uint8_t sd_cmd_17 = 17u;    // READ_SINGLE_BLOCK
static constexpr std::uint8_t sd_acmd_41 = 41u;
static constexpr std::uint8_t sd_cmd_55 = 55u;    // APP_CMD: prepare to run an application command
static constexpr std::uint8_t sd_cmd_58 = 58u;    // read OCR register
static constexpr std::uint8_t sd_cmd_59 = 59u;    // CRC ON/OFF

static constexpr std::uint8_t sd_start_token = 254;

/* sd_r1_t
   response 1
*/
struct sd_r1_t
{
  std::uint8_t  b_idle:1;
  std::uint8_t  b_erase_reset:1;
  std::uint8_t  b_illegal_command:1;
  std::uint8_t  b_crc_error:1;
  std::uint8_t  b_erase_sequence_error:1;
  std::uint8_t  b_address_error:1;
  std::uint8_t  b_parameter_error:1;
  std::uint8_t  b_zero:1;
};

/* sd_r1_t
   response 2
*/
struct sd_r2_t
{
  std::uint8_t  b_locked:1;
  std::uint8_t  b_fail:1;
  std::uint8_t  b_error:1;
  std::uint8_t  b_cc_error:1;
  std::uint8_t  b_ecc_error:1;
  std::uint8_t  b_wp_error:1;
  std::uint8_t  b_erase_param:1;
  std::uint8_t  b_range_error:1;
};

/* sd_r3_t
   response 3
*/
struct sd_r3_t
{
  std::uint8_t b_unused_2[3];
  std::uint8_t b_unused_1:4;
  std::uint8_t b_unused_0:1;
  std::uint8_t b_busy:1;
  std::uint8_t b_ccs:1;
  std::uint8_t b_uhsii:1;
};


/* sd_r7_t
   response 7 (reply to CMD_8)
*/
struct sd_r7_t
{
  std::uint8_t  b_pad_1;
  std::uint8_t  b_pad_0;
  std::uint8_t  b_power_good; // power good
  std::uint8_t  b_echo;       // cmd8 echo pattern
};

/* sd_result_t
   standard SD 40 bit response
*/
union  sd_result_t
{
  sd_r1_t response1;

  struct  r2_t: public sd_r1_t, public sd_r2_t {
  } response2;

  struct  r3_t: public sd_r1_t, public sd_r3_t {
  } response3;

  struct  r7_t: public sd_r1_t, public sd_r7_t {
  } response7;

  std::uint8_t  m_data[8];
};

/*namespace dev*/ }
#endif
