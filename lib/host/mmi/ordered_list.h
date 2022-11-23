#ifndef mmi_ordered_list_h
#define mmi_ordered_list_h
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
#include "linked_list_base.h"
#include <compare.h>

namespace mmi {

/* ordered_list
*/
template<typename Kt, typename Xt, bool assume_contiguous_ptr = false>
class ordered_list: public linked_list_base<Xt, assume_contiguous_ptr, true>
{
  using base_type = linked_list_base<Xt, assume_contiguous_ptr, true>;
  using key_type = typename std::remove_cv<Kt>::type;
  using node_type = typename base_type::node_type;

  static_assert(std::is_convertible<node_type, key_type>::value, "ordered_list requires node_type to be convertible to key_type.");

  static constexpr bool has_contiguous_ptr = base_type::assume_contiguous_ptr;
  static constexpr bool has_ordered_ptr = base_type::assume_ordered_ptr;

  public:
  inline  ordered_list() noexcept:
          base_type() {
  }

  inline  ordered_list(const ordered_list& copy) noexcept:
          base_type(copy) {
  }

  inline  ordered_list(ordered_list&& copy) noexcept:
          base_type(std::move(copy)) {
  }

  /* get_lower_bound()
  */
          node_type* get_lower_bound(key_type key) noexcept {
          if(base_type::m_head) {
              if(base_type::m_iter == nullptr) {
                  base_type::m_iter = base_type::m_head;
              } else
              if(compare(base_type::m_iter->operator key_type(), key) > 0) {
                  base_type::m_iter = base_type::m_head;
              }

              while(base_type::m_iter) {
                  if(compare(base_type::m_iter->operator key_type(), key) >= 0) {
                      return base_type::m_iter;
                  }
                  base_type::m_last = base_type::m_iter;
                  base_type::m_iter = base_type::m_iter->get_next();
              }
          }
          return nullptr;
  }

  /* get_upper_bound()
  */
          node_type* get_upper_bound(key_type key) noexcept {
          if(base_type::m_head) {
              if(base_type::m_iter == nullptr) {
                  base_type::m_iter = base_type::m_head;
              } else
              if(compare(base_type::m_iter->operator key_type(), key) > 0) {
                  base_type::m_iter = base_type::m_head;
              }

              while(base_type::m_iter) {
                  if(compare(base_type::m_iter->operator key_type(), key) > 0) {
                      return base_type::m_iter;
                  }
                  base_type::m_last = base_type::m_iter;
                  base_type::m_iter = base_type::m_iter->get_next();
              }
          }
          return nullptr;
  }

  /* get_equal_range()
  */
  inline  node_type* get_equal_range(key_type key) noexcept  {
          return get_by_key(key, true);
  }

  inline  node_type* insert(node_type* node) noexcept {
          if(node) {
              if(get_upper_bound(node->operator key_type())) {
                  return base_type::insert(node, base_type::m_last, base_type::m_iter);
              } else
                  return base_type::insert(node, base_type::m_last, nullptr);
          }
          return nullptr;
  }

  inline  node_type* insert(node_type& node) noexcept {
          return insert(std::addressof(node));
  }

  inline  node_type* remove(node_type* node) noexcept {
          return base_type::erase(node);
  }

  inline  node_type* remove(node_type& node) noexcept {
          return remove(std::addressof(node));
  }

  inline  node_type* find(key_type& key) noexcept {
          int cmp;
          if(get_lower_bound(key)) {
              cmp = compare(key, base_type::m_iter->operator key_type());
              if(cmp > 0) {
                  cmp = compare(key, base_type::m_last->operator key_type());
                  if(cmp == 0) {
                      return base_type::m_last;
                  }
              } else
              if(cmp == 0) {
                  return base_type::m_iter;
              }
          }
          return nullptr;
  }

  inline  node_type* find(node_type*) noexcept {
          return nullptr;
  }

  inline  ordered_list& operator=(const ordered_list& rhs) noexcept {
          base_type::operator=(rhs);
          return *this;
  }

  inline  ordered_list& operator=(ordered_list&& rhs) noexcept {
          base_type::operator=(std::move(rhs));
          return *this;
  }
};

/*namespace mmi*/ }
#endif
