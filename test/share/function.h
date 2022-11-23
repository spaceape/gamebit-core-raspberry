#ifndef test_function_h
#define test_function_h
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
#include <gfx.h>
#include <uld/target.h>
#include <uld/image.h>
#include <gfx/device.h>
#include <gfx/surface.h>

template<typename Rt, typename... Args>
struct function
{
  using type = Rt(*)(Args...);
};

template<typename Rt, typename... Args>
function<Rt, Args...>::type get_function(uld::image& image, const char* name) noexcept
{
      if(uld::symbol_t*
          l_symbol_ptr = image.find_symbol(name);
          l_symbol_ptr != nullptr) {
          if(l_symbol_ptr->ra != nullptr) {
              return (typename function<Rt, Args...>::type)l_symbol_ptr->ra;
          } else
              printf("/!\\ FAILED: %s %d: Symbol `%s` is undefined\n", __FILE__, __LINE__, name);
      } else
          printf("/!\\ FAILED: %s %d: Symbol `%s` not found.\n", __FILE__, __LINE__, name);
      return nullptr;
}

#endif
