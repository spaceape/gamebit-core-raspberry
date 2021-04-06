#ifndef sys_fat_h
#define sys_fat_h
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

namespace sys {
namespace fat {

constexpr std::size_t s_path_max = 4096u;

/* bpb16_t
   FAT16 Bios Parameter Block (reserved region, sector 0)
*/
struct __attribute__((packed)) bpb16_t
{
  std::uint16_t m_bps;                // bytes per sector
  std::uint8_t  m_spc;                // sectors per cluster
  std::uint8_t  m_reserved_count;     // number of reserved sectors
  std::uint8_t  m_fat_count;          // number of FATs
  std::uint16_t m_root_entry_count;   // number of entries in the root directory
  std::uint16_t m_sec16_count;        // number of 16 bit sectors
  std::uint8_t  m_media;              // media type
  std::uint16_t m_fat16_size;         // number of sectors occupied by one 16 bit FAT
  std::uint16_t m_spt;                // number of sectors per track (makes sense only for physicsl disk drives)
  std::uint16_t m_head_count;         // number of heads (makes sense only for physicsl disk drives)
  std::uint32_t m_hidden_count;       // number of hidden sectors preceeding the current partition
  std::uint32_t m_sec32_count;        // number of 32 bit sectors
};

/* s_fatXX_typename
   partition identification strings held in m_volume_type field
*/
constexpr char  s_fat16_typename[8] = "FAT16";
constexpr char  s_fat32_typename[8] = "FAT32";

/* bpb32_t
   FAT32 Bios Parameter Block (reserved region, sector 0)
*/
struct __attribute__((packed)) bpb32_t
{
  std::uint16_t m_bps;                // bytes per sector
  std::uint8_t  m_spc;                // sectors per cluster
  std::uint16_t m_reserved_count;     // number of reserved sectors
  std::uint8_t  m_fat_count;          // number of FATs
  std::uint16_t m_root_entry_count;   // number of entries in the root directory
  std::uint16_t m_sec16_count;        // number of 16 bit sectors
  std::uint8_t  m_media;              // media type
  std::uint16_t m_fat16_size;         // number of sectors occupied by one 16 bit FAT
  std::uint16_t m_spt;                // number of sectors per track (makes sense only for physicsl disk drives)
  std::uint16_t m_head_count;         // number of heads (makes sense only for physicsl disk drives)
  std::uint32_t m_hidden_count;       // number of hidden sectors preceeding the current partition
  std::uint32_t m_sec32_count;        // number of 32 bit sectors

  std::uint32_t m_fat32_size;         // number of sectors occupied by one 32 bit FAT  
  std::uint8_t  m_ext_flags[2];
  std::uint8_t  m_filesystem_version[2];  
  std::uint32_t m_root_cluster;       // index of the first cluster of the root directory
  std::uint16_t m_filesystem_info;    // index of the sector containing the filesystem info structure
  std::uint16_t m_backup_boot_sector;
  std::uint8_t  m_reserved_0[12];     // boot file name

  std::uint8_t  m_drive_number;
  std::uint8_t  m_reserved_1;
  std::uint8_t  m_boot_signature;
  std::uint32_t m_volume_id;
  char          m_volume_label[11];
  char          m_volume_type[8];
};

/* clu_*
   cluXX_flag_* flag bits
   cluXX_addr_* address bits
   cluXX_decl_* undefined addresses with special menings
*/
constexpr std::uint32_t clu32_flag_bits = 0xf0000000;       // the bit mask for the flags field

constexpr std::uint32_t clu32_addr_bits = 0x0fffffff;       // the bit mask for the address field
constexpr std::uint32_t clu32_decl_free = 0x00000000;       // free (unused) cluster
constexpr std::uint32_t clu32_decl_mark = 0x00000001;       // flag used for internal purposes (usually as a temporary mark)
constexpr std::uint32_t clu32_addr_min  = 0x00000002;
constexpr std::uint32_t clu32_addr_max  = 0x0fffffef;
constexpr std::uint32_t clu32_decl_custom_min = 0x0ffffff0; // not sure, maybe use as custom markers if ever necessary
constexpr std::uint32_t clu32_decl_custom_max = 0x0ffffff5;
constexpr std::uint32_t clu32_decl_reserved = 0x0ffffff6;   // reserved
constexpr std::uint32_t clu32_decl_bad = 0x0ffffff7;        // bad cluster mark
constexpr std::uint32_t clu32_decl_eoc_min = 0x0ffffff8;    // end of chain mark min
constexpr std::uint32_t clu32_decl_eoc_max = 0x0fffffff;    // end of chain mark max


/* dir_tag_*
   directory entry m_name[0] tags
*/
constexpr std::uint8_t  dir_tag_none = 0xe5;                // unused entry
constexpr std::uint8_t  dir_tag_alt_none = 0x05;            // unused entry - equivalent to 0xe5 but Japan friendly
constexpr std::uint8_t  dir_tag_last = 0;                   // unused entry && end of directory list
constexpr std::uint8_t  dir_tag_last_long = 0x40;           // last long entry

/* dir_attr_*
   directory entry attributes
*/
constexpr std::uint8_t  dir_attr_read_only = 0x01;
constexpr std::uint8_t  dir_attr_hidden    = 0x02;
constexpr std::uint8_t  dir_attr_system    = 0x04;
constexpr std::uint8_t  dir_attr_label     = 0x08;
constexpr std::uint8_t  dir_attr_directory = 0x10;
constexpr std::uint8_t  dir_attr_archive   = 0x20;
constexpr std::uint8_t  dir_attr_long_name = 0x0f;

constexpr std::uint8_t  dir_case_lc_base   = 0x08;
constexpr std::uint8_t  dir_case_lc_ext    = 0x10;

/* dir_t
   directory entry
*/
struct dir_t
{
  std::uint8_t  m_name[11];
  std::uint8_t  m_attributes;
  std::uint8_t  m_case_flags;
  std::uint8_t  m_ctime_ms;
  std::uint16_t m_ctime;
  std::uint16_t m_cdate;
  std::uint16_t m_adate;
  std::uint16_t m_cluster_h;
  std::uint16_t m_mtime;
  std::uint16_t m_mdate;
  std::uint16_t m_cluster_l;
  std::uint32_t m_size;
};

/* dir_t
   directory entry for long name
*/
struct dir_long_t {
  std::uint8_t  m_order;
  std::uint8_t  m_name1[10];
  std::uint8_t  m_attributes;
  std::uint8_t  m_reserved0;
  std::uint8_t  m_checksum;
  std::uint8_t  m_name2[12];
  std::uint8_t  m_reserved1[2];
  std::uint8_t  m_name3[4];
};

/*namespace fat*/ }
/*namespace sys*/ }
#endif
