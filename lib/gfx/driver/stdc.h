#ifndef gfx_stdc_driver_h
#define gfx_stdc_driver_h
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
#include <gfx.h>
#include <gfx/driver.h>

namespace gfx {

/* stdc
 * provides gfx with functionality that standard library covers, on platforms where that's available
*/
class stdc: public driver
{
  public:
  static  bool  gfx_load_cmo_preset(cmo&, int, unsigned int) noexcept;
  static  bool  gfx_load_cmo(cmo&, const char*, unsigned int, int) noexcept;
  static  bool  gfx_load_cso(pbo&, const char*, unsigned int, int, int) noexcept;
  static  bool  gfx_load_pbo(pbo&, const char*, unsigned int, int, int) noexcept;
  
  public:
          stdc() noexcept;
          stdc(const stdc&) noexcept;
          stdc(stdc&&) noexcept;
          stdc& operator=(const stdc&) noexcept;
          stdc& operator=(stdc&&) noexcept;
};
  
/*namespace gfx*/ }
#endif
