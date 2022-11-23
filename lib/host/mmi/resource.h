#ifndef mmi_resource_h
#define mmi_resource_h
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
#include <global.h>

class resource
{
  public:
  static constexpr size_t max_align = alignof(max_align_t);
 
  protected:
  virtual void* do_allocate(std::size_t, std::size_t) = 0;
  virtual void  do_deallocate(void*, std::size_t, std::size_t) = 0;
  virtual bool  do_is_equal(const resource&) const noexcept = 0;

  public:
          resource() noexcept;
          resource(const resource&) noexcept;
          resource(resource&&) noexcept;
  virtual ~resource();
 
          void* allocate(std::size_t, std::size_t = max_align) noexcept;
          void  deallocate(void*, std::size_t, std::size_t = max_align) noexcept;
          bool  is_equal(const resource&) const noexcept;

          resource& operator=(const resource&) noexcept;
          resource& operator=(resource&&) noexcept;
};

#endif
