#ifndef dev_raw_drive_h
#define dev_raw_drive_h
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

struct mbr_t;
struct pbr_t;

struct index
{
  std::uint32_t  m_dir_bit:1;         // index is part of a directory
  std::uint32_t  m_file_bit:1;        // index has content
  std::uint32_t  m_address:28;        // base cluster for FAT (or a locating feature for other FS)
  std::uint32_t  m_size;
  index*         m_parent;
};

/* drive
   instance of a partition on a block device
*/
class drive
{
  dev::bd*      p_device;

  protected:
  static constexpr int            s_sector_size  = 512;
  static constexpr std::uint32_t  s_sector_undef = 0xffffffff;

  protected:
  std::uint32_t m_cache_sector;
  std::uint8_t  m_cache_data[s_sector_size];

  protected:

          bool  load_sector(std::uint32_t) noexcept;
          bool  save_sector(std::uint32_t) noexcept;
          void  dump_sector(std::uint32_t) noexcept;
          void  dump_cache() noexcept;

  public:
          drive(dev::bd*) noexcept;
          drive(const drive&) noexcept;
          drive(drive&&) noexcept;
  virtual ~drive();
          mbr_t*        get_mbr() noexcept;
          pbr_t*        get_pbr(int) noexcept;
          pbr_t*        get_pbr(int, std::uint32_t&, std::uint32_t&) noexcept;
          std::uint8_t* get_raw(std::uint32_t) noexcept;

  virtual index         get_index(const char*, long int, long int) noexcept = 0;
  virtual std::size_t   get_raw(std::uint8_t*&, std::uint32_t, std::uint32_t, std::size_t) noexcept = 0;
  virtual std::size_t   set_raw(std::uint8_t*&, std::uint32_t, std::uint32_t, std::size_t) noexcept = 0;

          drive& operator=(const drive&) noexcept;
          drive& operator=(drive&&) noexcept;
};
/*namespace dev*/ }
#endif
