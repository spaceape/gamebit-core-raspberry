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
#include "memory.h"
#include <limits>

namespace uld {

      constexpr int  s_segment_max  = std::numeric_limits<short int>::max() - 1;

      memory::memory() noexcept:
      target(),
      m_lower_bound(0),
      m_upper_bound(0)
{
      // steal 1K for the segment list right off the bat
      m_segment_list.reserve(1024 / sizeof(rtl_segment_t));

      // push the first (undefined) segment
      if(m_segment_list.capacity()) {
          auto& l_null = m_segment_list.emplace_back(); 
          l_null.attr = 0;
          l_null.used = 0;
          l_null.size = 0;
          l_null.address.base = rsa_base_undef;
          l_null.address.data = nullptr;
          m_lower_bound = 0;
          m_upper_bound = 1;
      }
}

      memory::~memory()
{
}

rtl_segment_t& memory::get_undef() noexcept
{
      return m_segment_list[0];
}

rtl_segment_t& memory::get_at(int index) noexcept
{
      if(index < static_cast<int>(m_segment_list.size())) {
          return m_segment_list[index + 1];
      }
      if(index < s_segment_max) {
          auto&  l_result = m_segment_list.emplace_back();
          l_result.attr = 0;
          l_result.used = 0;
          l_result.size = 0;
          l_result.address.base = 0;
          l_result.address.data = nullptr;
          return l_result;
      }
      return get_undef();
}

rtl_segment_t& memory::get_by_type(std::uint8_t attributes) noexcept
{
      if(attributes) {
          int l_index = m_lower_bound;
          while(l_index > 0) {
              if(m_segment_list[l_index].attr == attributes) {
                  return m_segment_list[l_index];
              }
              l_index--;
          }
      }
      return get_undef();
}

rtl_segment_t& memory::get_by_address(std::uint32_t) noexcept
{
      return get_undef();
}

rtl_address_t memory::map(std::uint8_t attr, std::uint32_t, std::uint32_t size, std::uint32_t align) noexcept
{
      rtl_address_t l_result;
      int           i_segment     = m_upper_bound;
      int           l_alloc_bytes = get_round_value(size, sizeof(void*));
      std::uint32_t l_address;
      std::uint8_t* l_pointer;
  
      // go from upper down to lower bound and try to find a segment of similar attributes to allocate space from;
      // if not found, set the segment index to upper bound;
      // if not found and at the maximum allowed number of segments already set the segment index to 0
      while(i_segment > m_lower_bound) {
          auto& l_segment = get_at(i_segment);
          if(l_segment.attr == 0) {
              if(l_segment.address.base == rsa_base_undef) {
                  i_segment = 0;
                  break;
              }
              // new segment, find the most recently filled one in the same region and append to its memory
              if(auto& l_tail = get_by_type(attr); l_tail.attr == attr) {
                  l_address = l_tail.address.base + l_tail.size * rsa_page_size;
              } else
                  l_address = 0;
              l_pointer = nullptr;
              break;
          }
          if(l_segment.attr == attr) {
              int l_size = l_segment.size * rsa_page_size;
              int l_used = l_segment.used;
              int l_free = l_size - l_segment.used;
              if(align >= 1) {
                  if(l_used % align) {
                      l_used += align - (l_used % align);
                      if(l_used > l_size) {
                          l_used = l_size;
                      }
                      l_free = l_size - l_used;
                  }
              }
              if(l_free < l_alloc_bytes) {
                  l_address = l_segment.address.base + l_size;
                  l_pointer = nullptr;
                  i_segment = m_upper_bound;
                  break;
              }
              l_address = l_segment.address.base + l_segment.used;
              l_pointer = l_segment.address.data + l_segment.used;
              break;
          }
          i_segment--;
      }

      // setup the allocated segments and the result
      l_result.base = rsa_base_undef;
      l_result.data = nullptr;
      if(i_segment > m_lower_bound) {
          int   l_pages;
          auto& l_segment = get_at(i_segment);
          bool  l_alloc_success;
          if(l_pointer != nullptr) {
              l_alloc_success = true;
          } else
          if(attr & rsa_mode_a) {
              l_pages = get_quotient_value(size, rsa_page_size);
              if(l_pages <= 255) {
                  if(l_pages * rsa_page_size < 65536) {
                      l_pointer = reinterpret_cast<std::uint8_t*>(malloc(l_pages * rsa_page_size));
                      if(l_pointer) {
                          l_segment.attr         = attr;
                          l_segment.size         = l_pages;
                          l_segment.address.base = l_address;
                          l_segment.address.data = l_pointer;
                          l_alloc_success = true;
                      } else
                          l_alloc_success = false;
                  } else
                      l_alloc_success = false;
              } else
                  l_alloc_success = false;
          } else
          if(true) {
              l_segment.attr         = attr;
              l_segment.size         = l_pages;
              l_segment.address.base = l_address;
              l_segment.address.data = nullptr;
              l_alloc_success        = true;
          }
          if(l_alloc_success) {
              l_result.base           = l_segment.address.base;
              l_result.data           = l_segment.address.data;
              l_segment.used         += l_alloc_bytes;
              l_segment.address.base += l_alloc_bytes;
              l_segment.address.data += l_alloc_bytes;
          }
      }
      return l_result;
}

rtl_address_t memory::unmap(std::uint8_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept
{
      rtl_address_t l_result;
      l_result.base = rsa_base_undef;
      l_result.data = nullptr;
      return l_result;
}

void  memory::reset() noexcept
{
}

/*namespace uld*/ }
