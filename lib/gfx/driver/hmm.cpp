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
#include "hmm.h"

namespace gfx {

      hmm::hmm() noexcept
{
}

      hmm::hmm(const hmm&) noexcept
{
}

      hmm::hmm(hmm&&) noexcept
{
}

std::uint8_t* hmm::cso_reserve(int size) noexcept
{
      return reinterpret_cast<std::uint8_t*>(malloc(size));
}

std::uint8_t* hmm::cso_dispose(std::uint8_t* data, int) noexcept
{
      free(data);
      return nullptr;
}

std::uint8_t* hmm::cmo_reserve(int size) noexcept
{
      return reinterpret_cast<std::uint8_t*>(malloc(size));
}

std::uint8_t* hmm::cmo_dispose(std::uint8_t* data, int) noexcept
{
      free(data);
      return nullptr;
}

std::uint8_t* hmm::cbo_reserve(int size) noexcept
{
      return reinterpret_cast<std::uint8_t*>(malloc(size));
}

std::uint8_t* hmm::cbo_dispose(std::uint8_t* data, int) noexcept
{
      free(data);
      return nullptr;
}

std::uint8_t* hmm::pbo_reserve(int size) noexcept
{
      return reinterpret_cast<std::uint8_t*>(malloc(size));
}

std::uint8_t* hmm::pbo_dispose(std::uint8_t* data, int) noexcept
{
      free(data);
      return nullptr;
}

/* load()
   import the driver functions into the current context
*/
void  hmm::load() noexcept
{
      gfx_cso_reserve = cso_reserve;
      gfx_cso_dispose = cso_dispose;
      gfx_cmo_reserve = cmo_reserve;
      gfx_cmo_dispose = cmo_dispose;
      gfx_cbo_reserve = cbo_reserve;
      gfx_cbo_dispose = cbo_dispose;
      gfx_pbo_reserve = pbo_reserve;
      gfx_pbo_dispose = pbo_dispose;

}

hmm& hmm::operator=(const hmm&) noexcept
{
      return *this;
}

hmm& hmm::operator=(hmm&&) noexcept
{
      return *this;
}

/*namespace gfx*/ }
