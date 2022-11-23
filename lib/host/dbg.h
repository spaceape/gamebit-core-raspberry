#ifndef host_dbg_h
#define host_dbg_h
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
#include <log.h>

#if NDEBUG
static constexpr bool is_debug = false;
#else
static constexpr bool is_debug = true;
#endif

inline void dbg_dump_hex(void* bytes, int size, bool use_absolute_address = false) noexcept
{
      if constexpr (is_debug) {
          int   i_byte = 0;
          int   l_width = 16;
          char  l_hex_cache[128];
          char  l_ascii_cache[128];
          off_t l_data_base = 0;
          auto l_data_ptr = reinterpret_cast<std::uint8_t*>(bytes);
          if(use_absolute_address == true) {
              l_data_base = reinterpret_cast<off_t>(bytes);
          }
          while(i_byte < size) {
              int  i_column       = 0;
              int  l_data_offset  = i_byte;
              int  l_print_offset = 0;
              // print hex dump
              while(i_column < l_width) {
                  l_ascii_cache[i_column] = '.';
                  if(i_byte < size) {
                      l_print_offset += std::snprintf(l_hex_cache + l_print_offset, 4, "%.2x ",  *l_data_ptr);
                      if((*l_data_ptr > 32) &&
                          (*l_data_ptr < 128)) {
                          l_ascii_cache[i_column] = *l_data_ptr;
                      }
                      l_data_ptr++;
                  } else
                      l_print_offset += std::snprintf(l_hex_cache + l_print_offset, 4, "   ");
                  i_column++;
                  i_byte++;
              }
              l_hex_cache[l_print_offset] = 0;
              l_ascii_cache[i_column] = 0;
              printf("    %.8x: %s %s\n", l_data_base + l_data_offset, l_hex_cache, l_ascii_cache);
          }
      }
}
#endif
