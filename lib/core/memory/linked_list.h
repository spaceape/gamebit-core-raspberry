#ifndef memory_linked_list_h
#define memory_linked_list_h
/** 
    Copyright (c) 2019, wicked systems
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
#include "linked_list_base.h"

namespace mem {

/* linked_list
*/
template<typename Xt, bool assume_contiguous_ptr = false, bool assume_ordered_ptr = assume_contiguous_ptr>
class linked_list: public linked_list_base<Xt, assume_contiguous_ptr, assume_ordered_ptr>
{
  using   base_type = linked_list_base<Xt, assume_contiguous_ptr, assume_ordered_ptr>;
  using   node_type = typename base_type::node_type;

  static  constexpr bool has_contiguous_ptr = base_type::assume_contiguous_ptr;
  static  constexpr bool has_ordered_ptr = base_type::assume_ordered_ptr;

  public:
  inline  linked_list() noexcept:
          base_type() {
  }

  inline  linked_list(const linked_list& copy) noexcept:
          base_type(copy) {
  }

  inline  linked_list(linked_list&& copy) noexcept:
          base_type(std::move(copy)) {
  }

  /* append()
  */
          node_type* append(node_type* node) noexcept {
          if(node) {
              if(base_type::m_tail) {
                  if constexpr (base_type::has_next) {
                      base_type::m_tail->set_next(node);
                  }
                  if constexpr (base_type::has_prev) {
                      node->set_prev(base_type::m_tail);
                  }
              } else
                  base_type::m_base = node;
              base_type::m_tail = node;
              base_type::m_iter = node;
          }
          return node;
  }

  /* append()
  */
  inline  node_type* append(node_type& node) noexcept {
          return append(std::addressof(node));
  }

  /* remove()
  */
          node_type* remove() noexcept {
          node_type* prev;
          node_type* node = base_type::m_tail;
          if(node) {
              prev = base_type::get_prev(node);
              if(prev) {
                  if constexpr (base_type::has_next) {
                      prev->set_next(nullptr);
                  }
                  if constexpr (base_type::has_prev) {
                      node->set_prev(nullptr);
                  }
              } else
                  base_type::m_base = nullptr;
              if constexpr (base_type::has_next) {
                  node->set_next(nullptr);
              }
              if constexpr (base_type::has_prev) {
                  node->set_prev(nullptr);
              }
              base_type::m_tail = prev;
              if(base_type::m_iter == base_type::m_tail) {
                  base_type::m_iter = nullptr;
              }
          }
          return node;
  }

  inline  linked_list& operator=(const linked_list& rhs) noexcept {
          base_type::operator=(rhs);
          return *this;
  }

  inline  linked_list& operator=(linked_list&& rhs) noexcept {
          base_type::operator=(std::move(rhs));
          return *this;
  }
};
/*namespace mem*/ }
#endif
