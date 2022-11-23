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
#include "stdc.h"
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

namespace gfx {

      stdc::stdc() noexcept
{
}

      stdc::stdc(const stdc&) noexcept
{
}

      stdc::stdc(stdc&&) noexcept
{
}

/* gfx_load_cmo()
*/
bool  stdc::gfx_load_cmo_preset(cmo& cmo, int preset, unsigned int) noexcept
{
      if(preset == 256) {
          cmo.reset(cmo::fmt_argb_8888, 256);
          cmo.set_uint32(0, 0, 0, 0, 0);
          for(int i_colour = 1; i_colour < 256; i_colour++) {
              cmo.set_uint32(i_colour, 255, rand(), rand(), rand());
          }
          return true;
      }
      return false;
}

/* gfx_load_cmo()
*/
bool  stdc::gfx_load_cmo(cmo&, const char*, unsigned int, int) noexcept
{
      return false;
}

/* gfx_load_cso()
*/
bool  stdc::gfx_load_cso(pbo& pbo, const char* file_name, unsigned int format, int sx, int sy) noexcept
{
      return gfx_load_pbo(pbo, file_name, format, sx, sy * 128);
}

/* gfx_load_pbo()
*/
bool  stdc::gfx_load_pbo(pbo& pbo, const char* file_name, unsigned int format, int sx, int sy) noexcept
{
      if((sx > 0) && (sx < std::numeric_limits<short int>::max()) &&
          (sy > 0) && (sy < std::numeric_limits<short int>::max())) {

          pbo.reset(format, sx, sy);

          if(auto l_data_ptr = pbo.get_data_ptr(); l_data_ptr != nullptr) {
              int l_data_file = open(file_name, O_RDONLY);
              if(l_data_file >= 0) {
                  int  l_read_size;
                  int  l_data_size    = pbo.get_data_size();
                  bool l_read_success = true;
                  // load data
                  do {
                      l_read_size = read(l_data_file, l_data_ptr, 256);
                      if(l_read_size > 0) {
                          l_data_ptr  += l_read_size;
                          l_data_size -= l_read_size;
                      } else
                      if(l_read_size < 0) {
                          l_read_success = false;
                          break;
                      }
                  }
                  while(l_read_size);

                  // clear remaining unset bytes
                  if(l_read_success) {
                      if(l_data_size > 0) {
                          std::memset(l_data_ptr, 0, l_data_size);
                      }
                      return true;
                  }
              }
          }
      }
      return true;
}

stdc& stdc::operator=(const stdc&) noexcept
{
      return *this;
}

stdc& stdc::operator=(stdc&&) noexcept
{
      return *this;
}

/*namespace gfx*/ }
