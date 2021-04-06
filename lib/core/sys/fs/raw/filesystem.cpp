/** 
    Copyright (c) 2020, wicked systems
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
#include "filesystem.h"
#include "raw.h"
#include "mbr.h"
#include <dev/block.h>

namespace sys {
namespace raw {

      filesystem::filesystem(dev::block* dev) noexcept:
      p_dev(dev),
      m_sector(s_sector_undef)
{
}

      filesystem::filesystem(const filesystem& copy) noexcept:
      p_dev(copy.p_dev),
      m_sector(s_sector_undef)
{
}

      filesystem::filesystem(filesystem&& copy) noexcept:
      p_dev(copy.p_dev),
      m_sector(s_sector_undef)
{
}

      filesystem::~filesystem()
{
}

bool  filesystem::load_sector(std::uint32_t index) noexcept
{
      if(index == m_sector) {
          return true;
      }
      if(p_dev->load(m_cache, index, s_sector_size)) {
          m_sector = index;
          return true;
      }
      return false;
}

bool  filesystem::save_sector(std::uint32_t index) noexcept
{
      if(p_dev->save(m_cache, index, s_sector_size)) {
          m_sector = index;
          return true;
      }
      return false;
}

void  filesystem::dump_sector(std::uint32_t sector) noexcept
{
      if(load_sector(sector)) {
          dump_cache();
      } else
          printf("--- part:%p: Failed to download sector %.6lx\n", this, sector);
}

void  filesystem::dump_cache() noexcept
{
      for(int y = 0; y < 32; y++) {
          for(int x = 0; x < 16; x++) {
              std::uint8_t l_value = m_cache[y * 16 + x];
              if(l_value > 31 && l_value < 127) {
                  printf("%c  ", l_value);
              } else
                  printf("%.2x ", l_value);
          }
          printf("\n");
      }
}

mbr_t* filesystem::get_mbr() noexcept
{
      if(load_sector(0)) {
          mbr_t* l_mbr = reinterpret_cast<mbr_t*>(m_cache);
          auto   l_signature = get_uint16_le(l_mbr->m_signature);
          if(l_signature == mbr::signature) {
              return l_mbr;
          }
      }
      return nullptr;
}

pbr_t* filesystem::get_pbr(int part) noexcept
{
        std::uint32_t l_part_sector;
        std::uint32_t l_part_size = 0u;
        return get_pbr(part, l_part_sector, l_part_size);
}

pbr_t* filesystem::get_pbr(int part, std::uint32_t& part_offset, std::uint32_t& part_count) noexcept
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
                  pbr_t* l_pbr = reinterpret_cast<pbr_t*>(m_cache);
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

std::uint8_t* filesystem::get_raw(std::uint32_t sector) noexcept
{
      if(load_sector(sector)) {
          return m_cache;
      }
      return nullptr;
}

std::uint32_t filesystem::get_current_sector() noexcept
{
      return m_sector;
}

filesystem& filesystem::operator=(const filesystem& rhs) noexcept
{
      p_dev = rhs.p_dev;
      return *this;
}

filesystem& filesystem::operator=(filesystem&& rhs) noexcept
{
      p_dev = rhs.p_dev;
      return *this;
}

/*namespace raw*/ }
/*namespace sys*/ }