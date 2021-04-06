#ifndef memory_fragment_h
#define memory_fragment_h
/** 
    Copyright (c) 2020, wicked systems
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
#include "resource.h"
#include "policy.h"

class fragment: public resource
{
  public:
          fragment() noexcept;
          fragment(const fragment&) noexcept;
          fragment(fragment&&) noexcept;
          ~fragment();

  virtual void*  reallocate(void*, std::size_t, std::size_t, std::size_t, memory::fixed) noexcept;
  virtual void*  reallocate(void*, std::size_t, std::size_t, std::size_t, memory::expand_throw);
  virtual void*  reallocate(void*, std::size_t, std::size_t, std::size_t, ...) noexcept;

  inline  bool   has_fixed_size() const noexcept {
          return get_fixed_size();
  }

  virtual std::size_t get_fixed_size() const noexcept = 0;
  virtual bool        has_variable_size() const noexcept = 0;
  virtual std::size_t get_alloc_size(std::size_t) const noexcept = 0;

  static  fragment*   get_default() noexcept;
  static  fragment*   set_default(fragment*) noexcept;
  
          fragment&   operator=(const fragment&) noexcept;
          fragment&   operator=(fragment&&) noexcept;
};
#endif
