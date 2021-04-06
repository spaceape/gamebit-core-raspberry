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
#include "partition.h"
#include <sys/fs/fat/fat.h>
#include <sys/fs/raw/mbr.h>
#include <dev/block.h>
#include <util.h>
#include <cstring>

namespace sys {
namespace fat {

/* fat_is_valid_name_char()
   check if the character is valid in file name
*/
bool  fat_is_valid_name_char(char c) noexcept
{
      return strchr("\"*+,./,:;<=>?[/]|", c) == nullptr;
}

/* fat_is_valid_path_char()
   check if character is valid in an user-specified path
*/
bool  fat_is_valid_path_char(char c) noexcept
{
      return strchr("\"*+,,:;<=>?[/]|", c) == nullptr;
}

/* fat_cvt_token()
   process file name token: convert to upper case, count characters, replace path separator with NUL
*/
int   fat_cvt_token(char* name) noexcept
{
      int  l_index = 0;
      while(name[l_index]) {
          if((name[l_index] == '/') || (name[l_index] == '\\')) {
              name[l_index] = 0;
              break;
          } else
          if((name[l_index] >= 'a') && (name[l_index] <= 'z')) {
              name[l_index] = name[l_index] - 32;
              l_index++;
          } else
          if(name[l_index] < 32) {
              return 0;
          } else
          if(name[l_index] > 126) {
              return 0;
          } else
          if(fat_is_valid_path_char(name[l_index])) {
              l_index++;
          } else
              return 0;
      }
      return l_index;
}

/* fat_copy_name()
   make a copy of directory entry's name, adjusting case and converting the weird "BASE    EXT" format
*/
int   fat_copy_name(char* name, dir_t* dir) noexcept
{
      int   l_si  = 0;
      int   l_di  = 0;
      bool  l_dot = false;  // insert dot
      while(l_si < static_cast<int>(sizeof(dir->m_name)) + 1) {
          if(dir->m_name[l_si] == ' ') {
              if(l_dot == false) {
                  l_dot = true;
              }
              l_si++;
          } else
          if(dir->m_name[l_si] != ' ') {
              if(l_dot == true) {
                  name[l_di++] = '.';
                  l_dot = false;
              }
              if((dir->m_name[l_si] >= 'a') && (dir->m_name[l_si] <= 'z')) {
                  name[l_di++] = dir->m_name[l_si++] - 32;
              } else
                  name[l_di++] = dir->m_name[l_si++];
          }
      }
      name[l_di] = 0;
      return l_di;
}

      partition::partition(dev::block* dev, int part) noexcept:
      filesystem(dev),
      m_type(0),
      m_part_id(0),
      m_bps(0),
      m_spc(0),
      m_fat_bits(0),
      m_status(-1)
{
      std::uint8_t  l_type = 0;
      std::uint32_t l_part_offset;
      std::uint32_t l_part_count;
      if(mbr_t* l_mbr = get_mbr(); l_mbr != nullptr) {
          mbe_t* l_mbe = std::addressof(l_mbr->m_part[part]);
          if(get_uint32_le(l_mbe->m_part_count) > 0 &&
              get_uint32_le(l_mbe->m_part_offset) > 0) {
              l_type = l_mbe->m_type;
          }
      }
      if(l_type == mbr::type_fat32) {
          if(pbr_t* l_pbr = get_pbr(part, l_part_offset, l_part_count); l_pbr != nullptr) {
              bpb32_t*      l_bpb = reinterpret_cast<bpb32_t*>(l_pbr->m_bpb);
              std::uint32_t l_dc_count;
              if((l_bpb->m_bps > 0) &&
                  (l_bpb->m_spc > 0) &&
                  (l_bpb->m_reserved_count > 0) &&
                  (l_bpb->m_fat_count > 0) &&
                  (l_bpb->m_sec32_count > 1) &&
                  (l_bpb->m_root_cluster > 0)) {
                  m_part_id = part;
                  m_bps = l_bpb->m_bps;
                  m_spc = l_bpb->m_spc;
                  m_part_base = l_part_offset;
                  m_part_size = l_part_count;
                  m_fat_base = m_part_base + l_bpb->m_reserved_count;
                  m_fat_size = l_bpb->m_fat_count * l_bpb->m_fat32_size;
                  m_data_base = m_fat_base + m_fat_size;
                  m_data_size = l_bpb->m_sec32_count - l_bpb->m_reserved_count - m_fat_size;
                  // determine FAT entry size based on the number of data clusters
                  l_dc_count = m_data_size / m_spc;
                  if(l_dc_count < 4085) {
                      m_fat_bits = 12;
                  } else
                  if(l_dc_count < 65525) {
                      m_fat_bits = 16;
                      m_status = 0;
                  } else
                  if(l_dc_count > 65524) {
                      m_fat_bits = 32;
                      m_status = 0;
                  }
                  m_root_node = fat_get_root(l_bpb->m_root_cluster);
              }
          }
      }
      if(m_status == 0) {
          m_type = l_type;
      }
}

      partition::partition(const partition& copy) noexcept:
      filesystem(copy)
{
}

      partition::partition(partition&& copy) noexcept:
      filesystem(std::move(copy))
{
}

      partition::~partition()
{
}

/* fat_get_root()
   find the root directory chain in the FAT
*/
fsi_t partition::fat_get_root(std::uint32_t cluster) noexcept
{
      fsi_t  l_result;
      l_result.m_dir_bit = 1u;
      l_result.m_file_bit = 0u;
      l_result.m_base = cluster;
      l_result.m_parent = nullptr;
      return l_result;
}

/* fat_load_file()
   load the sector with given index of the file identified by its base cluster
*/
bool  partition::fat_load_file(std::uint32_t cluster, std::uint32_t index) noexcept
{
      std::uint32_t l_cluster = 0;
      std::uint32_t l_sector;
      // if the sector index is lower than the number of sectors in a cluster, we won't need to
      // reference the FAT, as we already know what the first cluster of the file is
      if(index < m_spc) {
          l_cluster = cluster;
          l_sector = index;
      } else
      if(index >= m_spc) {
          std::uint32_t l_fat_index = 0;
          std::uint32_t l_fat_sequence = index / m_spc;
          std::uint32_t l_fat_cluster = cluster;
          std::uint32_t l_fat_last_sector = 0;
          while(l_fat_index < l_fat_sequence) {
              std::uint32_t l_fat_position = l_fat_cluster * (m_fat_bits / 8);
              std::uint32_t l_fat_sector   = m_fat_base + l_fat_position / m_bps;
              std::uint32_t l_fat_offset   = l_fat_position % m_bps;
              if(l_fat_sector != l_fat_last_sector) {
                  if(load_sector(l_fat_sector) == false) {
                      return false;
                  }
                  l_fat_last_sector = l_fat_sector;
              }
              if(m_fat_bits == 32) {
                  std::uint32_t l_value   =*reinterpret_cast<std::uint32_t*>(m_cache + l_fat_offset);
                  // std::uint32_t l_flags   = l_value & clu32_flag_bits;
                  std::uint32_t l_address = l_value & clu32_addr_bits;
                  if((l_address >= clu32_addr_min) &&
                      (l_address <= clu32_addr_max)) {
                      l_fat_cluster = l_address;
                  } else
                      return false;
              } else
                  return false;
              l_fat_index++;
          }
          l_cluster = l_fat_cluster;
          l_sector  = index % m_spc;
      }
      // load the found cluster/sector
      if(l_cluster > 1) {
          std::uint32_t l_load_sector = m_data_base + ((l_cluster - 2) * m_spc) + l_sector;
          std::uint32_t l_load_boundary = m_data_base + m_data_size;
          if(l_load_sector < l_load_boundary) {
              if(load_sector(l_load_sector)) {
                  return true;
              }
          }
      }

      return false;
}

/* fat_seek_next()
*/
bool  partition::fat_seek_next(char* name, fsi_t& root, fsi_t& result) noexcept
{
      // process special paths
      if(name[0] == '/') {
          return fat_seek_next(name + 1, root, result);
      } else
      if(name[0] == '.') {
          if(name[1] == '.') {
              if(root.m_parent) {
                  return fat_seek_next(name + 3, *root.m_parent, result);
              } else
                  return false;
          } else
          if(name[1] == 0) {
              return fat_seek_next(name + 2, root, result);
          }
      } else
      if(name[0] == 0) {
          result = root;
          return true;
      }

      // we have a file or directory name, seek to its location
      if(root.m_dir_bit) {
          // process file name: convert to upper case, count characters, replace path separator with NUL
          int   l_name_length = fat_cvt_token(name);
          fsi_t l_node;
          //bool  l_name_error  = false;
          bool  l_name_found  = false;
          // open the root directory and find the entry corresponding to the name given;
          if(l_name_length) {
              char   l_name_ptr[16];
              int    l_read_index  = 0;
              int    l_read_sector = 0;
              int    l_read_fragment = m_bps / sizeof(dir_t);
              dir_t* l_read_ptr = nullptr;
              // traverse all the entries in the directory
              while(l_name_found == false) {
                  if((l_read_index % l_read_fragment) == 0) {
                      if(fat_load_file(root.m_base, l_read_sector) == false) {
                          break;
                      }
                      l_read_ptr = reinterpret_cast<dir_t*>(m_cache);
                      l_read_sector++;
                  }
                  if(l_read_ptr->m_name[0] == '.') {
                      // dot entry, skip
                  } else
                  if((l_read_ptr->m_name[0] == dir_tag_none) || (l_read_ptr->m_name[0] == dir_tag_alt_none)) {
                      // skip
                  } else
                  if((l_read_ptr->m_attributes & dir_attr_long_name) == dir_attr_long_name) {
                      // not yet supporting long directory entries, skip
                  } else
                  if(l_read_ptr->m_name[0]) {
                      fat_copy_name(l_name_ptr, l_read_ptr);
                      if(std::strncmp(l_name_ptr, name, sizeof(l_name_ptr)) == 0) {
                          l_node.m_dir_bit = 0u;
                          l_node.m_file_bit = 1u;
                          l_node.m_base = (l_read_ptr->m_cluster_h << 16) | l_read_ptr->m_cluster_l;
                          l_node.m_parent = std::addressof(root);
                          if(l_read_ptr->m_attributes & dir_attr_directory) {
                              l_node.m_dir_bit = 1u;
                          }
                          l_name_found = true;
                          break;
                      }
                  }
                  l_read_ptr++;
                  l_read_index++;
              }
          }
          if(l_name_found) {
              return fat_seek_next(name + l_name_length + 2, l_node, result);
          }
      }
      return false;
}

/* fat_seek()
*/
bool  partition::fat_seek(const char* path) noexcept
{
      char*  l_path_ptr;
      fsi_t  l_index;
      fsi_t  l_node = m_root_node;
      bool   l_result = false;

      // setup path buffer
      if(path) {
          std::size_t l_path_size;
          std::size_t l_path_length = std::strlen(path);
          if(l_path_length) {
              if(l_path_length < s_path_max) {
                  l_path_size = get_round_value(l_path_length + 1, 16);
                  l_path_ptr  = reinterpret_cast<char*>(malloc(l_path_size));
                  if(l_path_ptr) {
                      std::memset(l_path_ptr, 0, l_path_size);
                      std::strncpy(l_path_ptr, path, l_path_size);
                  } else
                      return false;
              } else
                  return false;
          } else
              l_path_ptr = nullptr;
      } else
          l_path_ptr = nullptr;

      // traverse path and return l_node
      if(fat_seek_next(l_path_ptr, l_node, l_index)) {
          if(fat_load_file(l_index.m_base, 0)) {
              dump_cache();
              l_result = true;
          }
      }
      if(l_path_ptr) {
          free(l_path_ptr);
      }
      return l_result;
}

void* partition::open(const char* path) noexcept
{
      if(m_status == 0) {
          fat_seek(path);
      }
      return nullptr;
}

void* partition::close() noexcept
{
      return nullptr;
}

partition& partition::operator=(const partition&) noexcept
{
      return *this;
}

partition& partition::operator=(partition&&) noexcept
{
      return *this;
}
/*namespace fat*/ }
/*namespace sys*/ }
