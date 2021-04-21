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
#include "drive.h"
#include <dev/bd.h>
#include "raw/mbr.h"
#include <stdio.h>

namespace dev {

      drive::drive(dev::bd* device) noexcept:
      p_device(device),
      m_cache_sector(s_sector_undef)
{
}

      drive::drive(const drive& copy) noexcept:
      p_device(copy.p_device),
      m_cache_sector(s_sector_undef)
{
}

      drive::drive(drive&& copy) noexcept:
      p_device(copy.p_device),
      m_cache_sector(s_sector_undef)
{
}

      drive::~drive()
{
}

bool  drive::load_sector(std::uint32_t sector) noexcept
{
      if(sector == m_cache_sector) {
          return true;
      }
      if(p_device->load(m_cache_data, sector, s_sector_size)) {
          m_cache_sector = sector;
          return true;
      }
      return false;
}

bool  drive::save_sector(std::uint32_t sector) noexcept
{
      if(p_device->save(m_cache_data, sector, s_sector_size)) {
          m_cache_sector = sector;
          return true;
      }
      return false;
}

void  drive::dump_sector(std::uint32_t sector) noexcept
{
      if(load_sector(sector)) {
          dump_cache();
      } else
          printf("--- part:%p: Failed to download sector %.6lx\n", (void*)this, sector);
}

void  drive::dump_cache() noexcept
{
      for(int y = 0; y < 32; y++) {
          for(int x = 0; x < 16; x++) {
              std::uint8_t l_value = m_cache_data[y * 16 + x];
              if(l_value > 31 && l_value < 127) {
                  printf("%c  ", l_value);
              } else
                  printf("%.2x ", l_value);
          }
          printf("\n");
      }
}

mbr_t* drive::get_mbr() noexcept
{
      if(load_sector(0)) {
          mbr_t* l_mbr = reinterpret_cast<mbr_t*>(m_cache_data);
          auto   l_signature = get_uint16_le(l_mbr->m_signature);
          if(l_signature == mbr::signature) {
              return l_mbr;
          }
      }
      return nullptr;
}

pbr_t* drive::get_pbr(int part) noexcept
{
        std::uint32_t l_part_sector;
        std::uint32_t l_part_size = 0u;
        return get_pbr(part, l_part_sector, l_part_size);
}

pbr_t* drive::get_pbr(int part, std::uint32_t& part_offset, std::uint32_t& part_count) noexcept
{
      if((part >= mbr::part_min) && (part <= mbr::part_max)) {
          std::uint32_t l_part_offset;
          std::uint32_t l_part_count  = 0;
          if(mbr_t* l_mbr = get_mbr(); l_mbr != nullptr) {
              mbe_t* l_mbe = std::addressof(l_mbr->m_part[part]);
              if((l_mbe->m_boot & mbr::boot_undef) == 0) {
                  l_part_offset = get_uint32_le(l_mbe->m_part_offset);
                  l_part_count = get_uint32_le(l_mbe->m_part_count);
              }
          }
          if(l_part_count) {
              if(load_sector(l_part_offset)) {
                  pbr_t* l_pbr = reinterpret_cast<pbr_t*>(m_cache_data);
                  auto   l_signature = get_uint16_le(l_pbr->m_signature);
                  if(l_signature == mbr::signature) {
                      part_offset = l_part_offset;
                      part_count = l_part_count;
                      return l_pbr;
                  }
              }
          }
      }
      part_offset = s_sector_undef;
      part_count = 0;
      return nullptr;
}

std::uint8_t* drive::get_raw(std::uint32_t sector) noexcept
{
      if(load_sector(sector)) {
          return m_cache_data;
      }
      return nullptr;
}

drive& drive::operator=(const drive& rhs) noexcept
{
      p_device = rhs.p_device;
      return *this;
}

drive& drive::operator=(drive&& rhs) noexcept
{
      p_device = rhs.p_device;
      return *this;
}
/*namespace dev*/ }
