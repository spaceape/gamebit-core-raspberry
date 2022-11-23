#ifndef dev_raw_mbr_h
#define dev_raw_mbr_h
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
#include <dev.h>

namespace dev {

struct pbr_t
{
  std::uint8_t  m_boot_jmp[3];        // boot jump instruction (x86)
  char          m_oem[8];             // partition oem name
  std::uint8_t  m_bpb[109];           // partition structure
  std::uint8_t  m_boot_code[390];     // boot code
  std::uint8_t  m_signature[2];
};

typedef pbr_t pbr;

/* mbe
   master boot record entry
*/
struct mbe_t
{
  std::uint8_t  m_boot;
  std::uint8_t  m_begin_chs[3];
  std::uint8_t  m_type;
  std::uint8_t  m_end_chs[3];
  std::uint8_t  m_part_offset[4];
  std::uint8_t  m_part_count[4];
};

/* mbr
   master boot record
*/
struct mbr_t
{
  static constexpr std::uint8_t  boot_undef = 127;
  static constexpr std::uint8_t  boot_bootable = 128;
  static constexpr std::uint8_t  type_undef = 0x00;
  static constexpr std::uint8_t  type_fat12 = 0x01;
  static constexpr std::uint8_t  type_fat16 = 0x14;
  static constexpr std::uint8_t  type_fat32_chs = 0x0b;
  static constexpr std::uint8_t  type_fat32 = 0x0c;
  static constexpr std::uint8_t  type_extended = 0x0f;

  static constexpr int           part_min  = 0;
  static constexpr int           part_max  = 3;
  static constexpr std::uint16_t signature = 0xaa55;

  std::uint8_t  m_code[446];
  mbe_t         m_part[4];
  std::uint8_t  m_signature[2];
};

typedef mbr_t mbr;

/*namespace dev*/ }
#endif
