#ifndef memory_hash_map_h
#define memory_hash_map_h
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
#include "flat_map_traits.h"
#include <hash.h>
#include <compare.h>

namespace mem {
  
template<typename Kt, typename Xt, typename Ht = std::uint64_t>
class hash_map: protected flat_map_traits<Ht, Xt>::base_type
{
  public:
  using  key_type   = typename flat_map_traits<Kt, Xt>::key_type;
  using  value_type = typename flat_map_traits<Kt, Xt>::value_type;
  using  hash_type  = typename flat_map_traits<Ht, Xt>::key_type;
  using  node_type  = typename flat_map_traits<Ht, Xt>::node_type;
  using  base_type  = typename flat_map_traits<Ht, Xt>::base_type;

  public:
  using  node_forward_type = typename std::conditional<
         (sizeof(node_type) > sizeof(void*)),
          node_type&,
          node_type
         >::type;

  using  iterator_type = typename flat_map_traits<Ht, Xt>::iterator_type;
  using  result_type   = node_type*;

  private:
  iterator_type m_pos;
  unsigned int  m_replace_bit:1; /*whether to replace an already existing element or fail*/
  unsigned int  m_remove_bit:1;  /*whether to erase or invalidate an element upon removal*/

  private:
  /* test_p()
  */
  inline  bool test_p(hash_type key) noexcept {
          int  cmp = compare(m_pos->key, key);
          if(cmp == 0) {
              return true;
          } else
          if(cmp < 0) {
            ++m_pos;
          }
          return false;
  }

  /* find_p()
   * binary search with m_pos as initial pivot
  */
          bool find_p(hash_type key) noexcept {
          int  cmp;
          if(m_pos != base_type::end()) {
              cmp = compare(m_pos->key, key);
              if(cmp < 0) {
                  m_pos = std::lower_bound(m_pos + 1, base_type::end(), key);
                  if(m_pos != base_type::end()) {
                      return test_p(key);
                  }
              } else
              if(cmp > 0) {
                  m_pos = std::lower_bound(base_type::begin(), m_pos - 1, key);
                  if(m_pos != base_type::end()) {
                      return test_p(key);
                  }
              } else
                  return true;
          }
          return false;
  }

  /* place_p()
   * blind insert before m_pos
  */
  template<typename... Args>
  inline  bool  place_p(Args&&... args) noexcept {
          m_pos = base_type::emplace(m_pos, std::forward<Args>(args)...);
          return  true;
  }

  /* place_p()
   * blind insert before m_pos
  */
  inline  bool  place_p(const node_type& node) noexcept {
          m_pos = base_type::insert(m_pos, node);
          return  true;
  }

  /* replace_p()
   * replace node at m_pos
  */
  inline  void  replace_p(const node_type& node) noexcept {
          *m_pos = node;
  }

  /* replace_p()
   * replace node at m_pos
  */
  inline  void  replace_p(node_type&& node) noexcept {
          *m_pos = std::move(node);
  }

  /* reset_p()
   * try to set the pointer into a valid state if it's at the end()
  */
  inline  void  reset_p() noexcept {
          if(m_pos == base_type::end()) {
              m_pos = base_type::begin() + (base_type::size() / 2);
          }
  }

  public:
  inline  hash_map(
              std::pmr::memory_resource* r,
              bool replace = false,
              bool remove = true
          ) noexcept:
          base_type(r),
          m_pos(),
          m_replace_bit(replace),
          m_remove_bit(remove) {
          m_pos = base_type::end();
  }

  inline  hash_map(
              std::pmr::memory_resource* r,
              size_t reserve,
              bool replace = false,
              bool remove = true
          ) noexcept:
          base_type(r),
          m_pos(),
          m_replace_bit(replace),
          m_remove_bit(remove) {
          base_type::reserve(reserve);
          m_pos = base_type::end();
  }

  inline  hash_map(const hash_map& copy) noexcept:
          base_type(copy),
          m_pos(copy.m_pos),
          m_replace_bit(copy.m_replace_bit), 
          m_remove_bit(copy.m_remove_bit) {
  }

  inline  hash_map(hash_map&& copy) noexcept:
          base_type(std::move(copy)),
          m_pos(std::move(copy.m_pos)),
          m_replace_bit(copy.m_replace_bit), 
          m_remove_bit(copy.m_remove_bit) {
  }

          ~hash_map() {
  }

  /* find()
  */
  inline  iterator_type find(key_type key) noexcept {
          hash_ptr hash(key);
          return   find(hash);
  }

  /* find_hash()
  */
  inline  iterator_type find(hash_type hash) noexcept {
          reset_p();
          if(find_p(hash)) {
              return m_pos;
          } else
              return base_type::end();
  }

  /* insert()
  */
  inline  iterator_type insert(key_type key) noexcept {
          hash_ptr hash(key);
          return   insert(hash);
  }

  /* insert()
  */
  inline  iterator_type insert(hash_type hash) noexcept {
          reset_p();
          if(find_p(hash)) {
          } else
          if(place_p(hash)) {
              return m_pos;
          }
          return base_type::end();
  }

  /* insert()
  */
  template<typename... Args>
  inline  iterator_type insert(key_type key, Args&&... args) noexcept {
          hash_ptr hash(key);
          return   insert(hash, std::forward<Args>(args)...);
  }

  /* insert()
  */
  template<typename... Args>
  inline  iterator_type insert_hash(hash_type hash, Args&&... args) noexcept {
          reset_p();
          if(find_p(hash)) {
              if(m_replace_bit) {
                  m_pos->value = value_type(std::forward<Args>(args)...);
              }
          } else
          if(place_p(hash, std::forward<Args>(args)...)) {
              return m_pos;
          }
          return base_type::end();
  }

  /* insert()
  */
  inline  iterator_type insert(key_type key, const value_type& value) noexcept {
          hash_ptr hash(key);
          return   insert(hash, value);
  }

  /* insert()
  */
  inline  iterator_type insert(hash_type hash, const value_type& value) noexcept {
          reset_p();
          if(find_p(hash)) {
              if(m_replace_bit) {
                  m_pos->value = value;
                  return m_pos;
              }
          } else
          if(place_p(hash, value)) {
              return m_pos;
          }
          return base_type::end();
  }

  /* insert()
  */
  inline  iterator_type insert(key_type key, value_type&& value) noexcept {
          hash_ptr hash(key);
          return   insert(hash, std::move(value));
  }

  /* insert()
  */
  inline  iterator_type insert(hash_type hash, value_type&& value) noexcept {
          reset_p();
          if(find_p(hash)) {
              if(m_replace_bit) {
                  m_pos->value = std::move(value);
                  return m_pos;
              }
          } else
          if(place_p(hash, std::move(value))) {
              return m_pos;
          }
          return base_type::end();
  }

  /* remove()
   * erase node of given key, if found
  */
  inline  iterator_type remove(key_type key) noexcept {
          hash_ptr hash(key);
          return   remove(hash);
  }

  /* remove()
   * erase node of given hash, if found
  */
  inline  void remove(hash_type hash) noexcept {
          reset_p();
          if(find_p(hash)) {
              m_pos = base_type::erase(m_pos);
          }
  }

  /* remove()
  */
  inline  void remove(iterator_type pos) noexcept {
          m_pos = base_type::erase(pos);
  }

  inline  iterator_type begin() noexcept {
          return base_type::begin();
  }

  inline  iterator_type none() noexcept {
          return base_type::end();
  }
  
  inline  iterator_type end() noexcept {
          return base_type::end();
  }
  
  /* reserve()
  */
  inline  void reserve(size_t count) noexcept {
          base_type::reserve(count);
  }

  /* clear()
  */
  inline  void clear(size_t count) noexcept {
          base_type::clear();
  }

  inline  hash_map& operator=(const hash_map& rhs) noexcept {
          base_type::operator=(rhs);
          return *this;
  }

  inline  hash_map& operator=(hash_map&& rhs) noexcept {
          base_type::operator=(std::move(rhs));
          return *this;
  }
};
/*namespace mem*/ }
#endif
