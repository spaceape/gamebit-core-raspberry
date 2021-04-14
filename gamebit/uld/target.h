#ifndef uld_target_h
#define uld_target_h
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
#include <uld.h>
#include "target/runtime.h"

namespace uld {

class target
{
  public:
          target() noexcept;
  virtual ~target();
  virtual bool          reserve(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept;
  virtual rtl_address_t map(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept = 0;
  virtual rtl_address_t unmap(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept = 0;
  virtual bool          dispose(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept;
  virtual void          reset() noexcept;
};

/*namespace uld*/ }
#endif
