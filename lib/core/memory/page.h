#ifndef memory_page_h
#define memory_page_h
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
#include "pool_base.h"
#include <cstring>
#include <limits>

namespace mem {

/* page
   memory page - maps objects onto a fixed size memory region

   Xt - data type
   MapSize - map size (default: 0)
   ArraySize - array size (default: 1)
   Rt - resource type (default: heap)
*/
template<typename Xt, std::size_t MapSize, std::size_t ArraySize, typename Rt>
class page: public pool_base<Xt, Rt, fixed>
{
  page*  m_prev;
  page*  m_next;
  char   m_bitmap[MapSize];

  static_assert(ArraySize > 0, "ArraySize must be greater than 0.");

  public:
  using  base_type     = pool_base<Xt, Rt, fixed>;
  using  node_type     = typename base_type::node_type;
  using  resource_type = typename base_type::resource_type;

  static constexpr std::size_t map_size = MapSize;
  static constexpr std::size_t node_size = sizeof(node_type);
  static constexpr std::size_t array_size = ArraySize;

  private:
  /* get_page_bytes()
     size of a page object, in bytes
  */
  constexpr std::size_t get_page_bytes() const noexcept {
          return sizeof(page);
  }

  /* get_page_nodes()
     compute how many nodes are necessary to store a single page object;
  */
  constexpr std::size_t get_page_nodes() const noexcept {
          std::size_t l_page_nodes = get_page_bytes() / node_size;
          if(get_page_bytes() % node_size) {
              l_page_nodes++;
          }
          if constexpr (array_size > 1) {
              if(l_page_nodes % array_size) {
                  l_page_nodes -= l_page_nodes % array_size;
                  l_page_nodes += array_size;
              }
          }
          return l_page_nodes;
  }

  /* get_min_alloc()
     get the minimum number of elements to allocate at construction;
     e_min is the user value that adds up to the necessary minimum size required for the
     page to function correctly
  */
  constexpr std::size_t get_min_alloc(std::size_t e_min) const noexcept {
          return get_page_nodes() + e_min;
  }

  /* get_max_alloc()
     get the maximum number of elements that we are allowing to be allocated for this page;
     e_max is an user specified value - if bitmap is enabled, it can be limited to the maximum
     number of elemens that the page can hold based on the specified map size
     (if bitmap is enabled via MapSize > 0)
  */
  constexpr std::size_t get_max_alloc(std::size_t e_max) const noexcept {
          if constexpr (map_size > 0) {
              if(e_max > (map_size * array_size * 8)) {
                  return (map_size * array_size * 8);
              } else
                  return e_max;
          } else
              return e_max;
  }

  protected:
  /* map_get_bit()
     get the bit in the map corresponding to the given node pointer
  */
          bool map_get_bit(node_type* node, unsigned int& byte, unsigned int& bit) noexcept {
          unsigned int l_index;
          if(node >= base_type::m_head) {
              if(node < base_type::m_last) {
                  l_index =(node - base_type::m_head) / array_size;
                  byte    = l_index / 8;
                  bit     = l_index % 8;
                  return true;
              }
          }
          return false;
  }

  /* map_get_mask()
     get the mask in the map corresponding to the given node pointer
  */
          bool map_get_mask(node_type* node, unsigned int& byte, unsigned int& mask) noexcept {
          unsigned int l_bit;
          if(map_get_bit(node, byte, l_bit)) {
              mask = 1 << l_bit;
              return true;
          }
          return false;
  }

  /* map_get_node()
     get the node pointer corresponding to the given bit in the map
  */
          node_type*  map_get_node(unsigned int byte, unsigned int bit) noexcept {
          node_type*  l_result = base_type::m_head + ((byte * 8) + bit) * array_size;
          if(l_result < base_type::m_last) {
              return l_result;
          }
          return nullptr;
  }

  /* map_set_node()
     set a the bit corresponding to the given object in the allocation map
  */
  inline  node_type*  map_set_node(node_type* node) noexcept {
          if constexpr (map_size > 0) {
              unsigned int l_byte = 0;
              unsigned int l_mask = 0;
              if(map_get_mask(node, l_byte, l_mask)) {
                  m_bitmap[l_byte] |= l_mask;
                  return node;
              }
          }
          return nullptr;
  }

  /* map_clear_node()
     clear the bit corresponding to the given object in the allocation map
  */
  inline  node_type*  map_clear_node(node_type* node) noexcept {
          if constexpr (map_size > 0) {
              unsigned int l_byte = 0;
              unsigned int l_mask = 0;
              if(map_get_mask(node, l_byte, l_mask)) {
                  m_bitmap[l_byte] &= ~l_mask;
                  return node;
              }
          }
          return nullptr;
  }

  inline  page*      make_next_page() noexcept {
          if(m_next == nullptr) {
              if(m_next = new(reinterpret_cast<page*>(base_type::m_base)) page(this); m_next != nullptr) {
                  m_next->m_prev = this;
              }
          }
          return m_next;
  }

  inline  page*      free_next_page() noexcept {
          if(m_next != nullptr) {
              if(m_next == reinterpret_cast<page*>(base_type::m_base)) {
                  m_next->~page();
                  m_next = nullptr;
              }
          }
          return m_next;
  }

  template<typename... Args>
  inline  node_type* make_node(node_type* node, Args&&... args) noexcept {
          if constexpr (array_size > 1) {
              if(base_type::make_list(node, node + array_size, std::forward<Args>(args)...)) {
                  return map_set_node(node);
              } else
                  return nullptr;
          } else
          if(base_type::make_node(node, std::forward<Args>(args)...)) {
              return map_set_node(node);
          } else
              return nullptr;
  }

  template<typename... Args>
  inline  node_type* make_list(node_type* head, node_type* tail, Args&&... args) noexcept {
          for(auto l_iter = head; l_iter < tail; l_iter += array_size) {
              make_node(l_iter, std::forward<Args>(args)...);
          }
          return head;
  }

  inline  void  free_list(node_type* head, node_type* tail) noexcept {
          node_type*   prev;
          while(tail > head) {
              prev = tail - array_size;
              free_node(prev);
              tail = prev;
          }
  }

  inline  void  free_node(node_type* node) noexcept {
          if(map_clear_node(node)) {
              if constexpr (array_size > 1) {
                  base_type::free_list(node, node + array_size);
              } else
                  base_type::free_node(node);
          }
  }

  template<typename OXt, std::size_t OPageSize, std::size_t OArraySize, typename ORt>
  friend  class bank;

  public:
  inline  page() noexcept {
  }

  inline  page(
            const resource_type& resource,
            std::size_t  e_min = 0,
            std::size_t  e_max = std::numeric_limits<unsigned int>::max()
          ) noexcept:
          base_type(resource, get_min_alloc(e_min * array_size), get_max_alloc(e_max * array_size)),
          m_prev(nullptr),
          m_next(nullptr) {
          if constexpr (map_size > 0) {
              std::memset(m_bitmap, 0, map_size);
          }
          //reserve space for the next page at the beginning of the allocated region
          if(base_type::m_base) {
              base_type::m_head += get_page_nodes();
              base_type::m_tail += get_page_nodes();
          }
  }

  inline  page(page* root) noexcept:
          page(root->m_resource, root->m_count_max, root->m_count_max) {
  }

          page(const page&) noexcept = delete;
          page(page&&) noexcept = delete;

  inline  ~page() {
          // free sub-allocated page, if it exists
          if(m_next) {
              if(m_next == reinterpret_cast<page*>(base_type::m_base)) {
                  m_next->~page();
                //m_next = nullptr;
              }
          }
          // free the allocated nodes
          // when the page is mapped, lookup allocated nodes in the map and free all found
          if constexpr (map_size > 0) {
              node_type*   l_node;
              unsigned int l_byte = 0;
              unsigned int l_bit  = 0;
              while(m_bitmap[l_byte] & (1 << l_bit)) {
                  l_node = map_get_node(l_byte, l_bit);
                  if(l_node) {
                      free_node(l_node);
                      if(l_bit < 7) {
                          l_bit++;
                      } else
                      if(l_byte < map_size - 1) {
                          l_bit = 0;
                          l_byte++;
                      } else
                          break;
                  } else
                      break;
              }
          }
  }

  inline  node_type*  get_head() const noexcept {
          if(base_type::m_tail > base_type::m_head) {
              return base_type::m_head;
          } else
              return nullptr;
  }
  
  /* find()
  */
          node_type*   find(node_type* node, node_type*& hint) noexcept {
          unsigned int l_byte = 0;
          unsigned int l_bit  = 0;
          if(node) {
              // find used node
              // return node if allocated, nullptr otherwise
              if(map_get_bit(node, l_byte, l_bit) == true) {
                  if(m_bitmap[l_byte] & (1 << l_bit)) {
                      return hint = node;
                  } else
                      return hint = nullptr;
              } else
                  return hint = nullptr;
          } else
          if constexpr (map_size > 0) {
              // find free node
              map_get_bit(hint, l_byte, l_bit);
              while(m_bitmap[l_byte] & (1 << l_bit)) {
                  if(l_bit < 8) {
                      l_bit++;
                  } else
                  if(l_byte < map_size - 1) {
                      l_bit = 0;
                      l_byte++;
                  } else
                      return hint = nullptr;
              }
              return hint = map_get_node(l_byte, l_bit);
          }
          return nullptr;
  }

  inline  node_type*  get_tail() const noexcept {
          if(base_type::m_tail > base_type::m_head) {
              return base_type::m_tail;
          } else
              return nullptr;
  }

  inline  page* get_prev_page() const noexcept {
          return m_prev;
  }

  inline  page* get_next_page() const noexcept {
          return m_next;
  }

  inline  page* get_next_page(int index) const noexcept {
          page* l_page = this;
          while(index > 0) {
              l_page = l_page->get_next_page();
              if(l_page == nullptr) {
                  break;
              }
              index--;
          }
          return l_page;
  }

          page& operator=(const page&) noexcept = delete;
          page& operator=(page&&) noexcept = delete;
};

/*namespace mem*/ }
#endif
