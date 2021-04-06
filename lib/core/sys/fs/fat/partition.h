#ifndef dev_fat_partition_h
#define dev_fat_partition_h
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
#include <sys.h>
#include <sys/fs/raw/filesystem.h>

namespace sys {
namespace fat {

/* fsi_t
   filesystem index node
*/
struct fsi_t
{
  std::uint32_t  m_dir_bit:1;
  std::uint32_t  m_file_bit:1;
  std::uint32_t  m_base:28;        // base cluster
  fsi_t*         m_parent;
};

class partition: public sys::raw::filesystem
{
  unsigned int   m_type:8;
  unsigned int   m_part_id:8;
  unsigned int   m_bps:16;         // bytes per sector
  unsigned int   m_spc:8;          // sectors per cluster
  unsigned int   m_fat_bits:8;     // FAT entry bits       
  int            m_status:4;
  int            m_part_base;      // partition base sector
  int            m_part_size;      // partition size, in sectors
  int            m_fat_base;
  int            m_fat_size;
  int            m_data_base;      // data base sector
  int            m_data_size;      // data sectors
  fsi_t          m_root_node;

  protected:
          fsi_t  fat_get_root(std::uint32_t) noexcept;
          bool   fat_load_map(std::uint32_t) noexcept;
          bool   fat_load_file(std::uint32_t, std::uint32_t) noexcept;
          bool   fat_seek_next(char*, fsi_t&, fsi_t&) noexcept;
          bool   fat_seek(const char*) noexcept;

  public:
          partition(dev::block*, int) noexcept;
          partition(const partition&) noexcept;
          partition(partition&&) noexcept;
          ~partition();
          void*  open(const char*) noexcept;
          void*  close() noexcept;
          partition& operator=(const partition&) noexcept;
          partition& operator=(partition&&) noexcept;
};

/*namespace fat*/ }
/*namespace sys*/ }
#endif
