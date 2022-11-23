/** 
    Copyright (c) 2022, wicked systems
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
#include <sys/adt.h>
#include "device.h"
#include "directory.h"
#include <cstring>
#include <util.h>

namespace sys {

constexpr char chr_path_root = '/';
constexpr int  chr_reserve_bytes = 32;

static char* s_adt_path_cache;
static int   s_adt_path_size;

node*   adt_get_entry(const char* path) noexcept
{
      return nullptr;
}

device* adt_get_device(const char* path) noexcept
{
      sys::device* l_device = nullptr;
      if(path != nullptr) {
          char* l_path_ptr;
          int   l_path_size = std::strlen(path) + 1;
          int   l_path_reserve = get_round_value(l_path_size, chr_reserve_bytes);
          // reserve memory for the path object
          if(l_path_reserve > s_adt_path_size) {
              s_adt_path_cache = reinterpret_cast<char*>(realloc(s_adt_path_cache, l_path_reserve));
              if(s_adt_path_cache == nullptr) {
                  return nullptr;
              }
              s_adt_path_size = l_path_reserve;
          }
          l_path_ptr = reinterpret_cast<char*>(std::memcpy(s_adt_path_cache, path, l_path_size));
          // process header
          if(l_path_ptr[0] == chr_path_root) {
              l_path_ptr += 1;
          }
          // browse path
          l_device = g_adt->get_at(l_path_ptr);
      }
      return l_device;
}

directory* adt_get_directory(const char* path) noexcept
{
      sys::device* l_device = adt_get_device(path);
      if(l_device != nullptr) {
          return static_cast<directory*>(l_device);
      }
      return nullptr;
}
/*namespace sys*/ }
