#ifndef uld_target_memory_h
#define uld_target_memory_h
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
#include <uld/target.h>
#include "runtime.h"
#include <vector>

namespace uld {

/* memory
   memory link target: allows running native code loaded into RAM
*/
class memory: public target
{
  // TODO: Come up with a better storage model for the segments; current scheme is simple enough for a quick implementation
  // and works well up to a relatively large number of loaded objects, but as segments accumulate, vectors will eventually
  // have to up their capacity, which will lead to a a new alloc with double the previous capacity, followed by a copy- and
  // finally a dealloc of the of the old data;
  // Apart from the obvious performance implications, this will temporarily use 50% more memory than it actually needs and
  // worst of all - potentially introduce some unwanted memory fragmentation.
  // Even a vector of vectors would be a better choice, though I might have something even better (a purpose tailorded
  // memory::bank perhaps, if this proves to be an useful feature after all)
  std::vector<rtl_segment_t>  m_segment_list;
  int                         m_lower_bound;  // segment to start the search from
  int                         m_upper_bound;  // really a long winded way of saying "highest segment in the list"

  protected:
          rtl_segment_t&  get_undef() noexcept;
          rtl_segment_t&  get_at(int) noexcept;
          rtl_segment_t&  get_by_type(std::uint8_t) noexcept;
          rtl_segment_t&  get_by_address(std::uint32_t) noexcept;
          rtl_segment_t&  find_by_type(std::uint8_t) noexcept;
          rtl_segment_t&  find_by_address(std::uint32_t) noexcept;

  public:
          memory();
          memory(const memory&) noexcept = delete;
          memory(memory&&) noexcept = delete;
  virtual ~memory();
  virtual rtl_fragment_t  map(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept override;
  virtual rtl_fragment_t  unmap(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept override;
  virtual void            reset() noexcept override;
          memory& operator=(const memory&) noexcept = delete;
          memory& operator=(memory&&) noexcept = delete;
};

/*namespace uld*/ }
#endif
