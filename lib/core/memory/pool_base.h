#ifndef memory_pool_base_h
#define memory_pool_base_h
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
#include <memory.h>
#include "metrics.h"
// #include <cstring>

namespace memory {

/* pool_base
   base type for memory pool

   Xt - data type;
   Rt - resource type (default: heap)
   Pt - allocation policy type (default: none)
*/
template<typename Xt, typename Rt, typename Pt>
class pool_base
{
  public:
  using  node_type     = typename std::remove_cv<Xt>::type;
  using  resource_type = typename std::remove_cv<Rt>::type;
  using  policy_type   = typename std::remove_cv<Pt>::type;

  static constexpr std::size_t node_size = sizeof(node_type);

  static constexpr bool is_node_constructible =
      std::is_trivial<node_type>::value == false;

  static constexpr bool is_node_destructible =
      std::is_destructible<node_type>::value && 
      (std::is_trivially_destructible<node_type>::value == false);

  static constexpr bool is_resource_static    = get_fixed_size<resource_type, node_type>();
  static constexpr bool is_resource_resizable = get_resizable<resource_type, policy_type>();

  protected:
  resource_type m_resource;
  policy_type   m_policy;
  unsigned int  m_align;
  unsigned int  m_size;
  unsigned int  m_count_min;
  unsigned int  m_count_max;
  Xt*           m_base;
  Xt*           m_head;
  Xt*           m_tail;
  Xt*           m_last;

  protected:
  template<typename... Args>
  inline  node_type* make_node(node_type* node, Args&&... args) noexcept {
          if constexpr (is_node_constructible && std::is_constructible<node_type, Args...>::value) {
              new(node) node_type(std::forward<Args>(args)...);
          }
          return node;
  }

  template<typename... Args>
  inline  node_type* make_list(node_type* head, node_type* tail, Args&&... args) noexcept {
          for(auto l_iter = head; l_iter < tail; l_iter++) {
              make_node(l_iter, std::forward<Args>(args)...);
          }
          return head;
  }

  inline  void  copy_list(node_type* head, node_type* tail, node_type* copy) noexcept {
          if constexpr (is_node_constructible) {
              if constexpr (std::is_constructible<node_type, node_type*>::value) {
                  while(head < tail) {
                      make_node(head, copy);
                      head++;
                      copy++;
                  }
              } else
              if constexpr (std::is_move_constructible<node_type>::value) {
                  while(head < tail) {
                      make_node(head, std::move(*copy));
                      head++;
                      copy++;
                  }
              } else
              if constexpr (std::is_copy_constructible<node_type>::value) {
                  while(head < tail) {
                      make_node(head, *copy);
                      head++;
                      copy++;
                  }
              }
          } else
              std::memcpy(head, copy, (tail - head) * sizeof(node_type));
  }

  inline  void  free_list(node_type* head, node_type* tail) noexcept {
          node_type*   prev;
          while(tail > head) {
              prev = tail - 1;
              free_node(prev);
              tail = prev;
          }
  }

  inline  void  free_node(node_type* node) noexcept {
          if constexpr (is_node_destructible) {
              node->~node_type();
          }
  }

  protected:
  /**/    node_type*  resize(std::size_t size) noexcept {
          node_type*  l_base;
          std::size_t l_size_next = 0;
          std::size_t l_size_prev = m_size;
          if(size > l_size_prev) {
              if constexpr (is_resource_static) {
              // resource is static (a fixed block of memory has already been reserved for it)
              // resizing within its limits will be essentially a no-op
              // resizing outside its limits will tragically fail
                  l_size_next = get_fixed_size<resource_type, node_type>();
              } else
              if constexpr (is_resource_resizable) {
              // resource allows realloc()
              // if the nodes are constructible c++ objects, reallocation is *not* performed
              // directly (violates the c++ principles), but instead replaced by a new
              // allocation, followed by a move.
              // otherwise, for primitive types and pure data nodes realloc() works as follows:
              // - if not previously allocated, simply alloc();
              // - if empty, don't realloc(), simply alloc() - that saves an expensive and
              //   unnecessary move operation with garbage data;
              // - proceed as normal otherwise
                  if constexpr (is_node_constructible) {
                      l_size_next = get_alloc_size<resource_type, node_type>(size);
                  } else
                  if(m_base == nullptr) {
                      l_size_next = get_alloc_size<resource_type, node_type>(size);
                  } else
                  if(m_base == m_tail) {
                      l_size_next = get_alloc_size<resource_type, node_type>(size);
                  } else
                  if(std::size_t l_size_exp = get_alloc_size<resource_type, node_type>(size); l_size_exp <= std::numeric_limits<unsigned int>::max()) {
                      auto  l_size_new = l_size_exp;
                      auto  l_copy_ptr = reinterpret_cast<node_type*>(m_resource.reallocate(m_base, l_size_prev * node_size, l_size_exp * node_size, m_align));
                      if(l_copy_ptr) {
                          if(l_size_new > m_count_max) {
                              l_size_new = m_count_max;
                          }
                          l_base = m_base;
                          m_base = l_copy_ptr;
                          m_last = l_copy_ptr + l_size_new;
                          m_size = l_size_exp;
                      } else
                          return nullptr;
                  } else
                      return nullptr;
              } else
              if(m_base == nullptr) {
              // resource is neither static nor resizable - only the first allocation succeeds
                  l_size_next = get_alloc_size<resource_type, node_type>(size);
              } else
                  return nullptr;

              if(l_size_next > l_size_prev) {
                  if(l_size_next <= std::numeric_limits<unsigned int>::max()) {
                      auto  l_size_new = l_size_next;
                      auto  l_copy_ptr = reinterpret_cast<node_type*>(m_resource.allocate(l_size_next * node_size, m_align));
                      if(l_copy_ptr) {
                          if(l_size_new > m_count_max) {
                              l_size_new = m_count_max;
                          }
                          l_base = m_base;
                          m_base = l_copy_ptr;
                          m_last = l_copy_ptr + l_size_new;
                          if(m_base != l_base) {
                              node_type*  l_head = m_head;
                              if(std::ptrdiff_t l_head_offset = m_head - l_base; l_head_offset > 0) {
                                  m_head = m_base + l_head_offset;
                              } else
                                  m_head = m_base;

                              node_type*  l_tail = m_tail;
                              if(std::ptrdiff_t l_tail_offset = m_tail - l_base; l_tail_offset > 0) {
                                  m_tail = m_base + l_tail_offset;
                              } else
                                  m_tail = m_base;

                              if(l_base) {
                                  if(m_head < m_tail) {
                                      copy_list(m_head, m_tail, l_head);
                                      free_list(l_base, l_tail);
                                  }
                                  m_resource.deallocate(l_base, l_size_prev * node_size, m_align);
                              }
                          }
                          m_size = l_size_next;
                      } else
                          return nullptr;
                  } else
                      return nullptr;
              }
          }
          return m_head;
  }

  inline  bool   upsize() noexcept {
          return resize(get_used_size() + get_alloc_size<resource_type, node_type>());
  }

  inline  bool   reserve(std::size_t count) noexcept {
          return resize(get_used_size() + count);
  }

  protected:
  /* raw_get()
     get a pointer to a new element, created using the given args - if constructible
  */
  template<typename... Args>
  inline  node_type* raw_get(Args&&... args) noexcept {
          if(m_tail < m_last) {
              return make_node(m_tail++, std::forward<Args>(args)...);
          } else
          if(upsize()) {
              return raw_get(std::forward<Args>(args)...);
          } else
              return nullptr;
  }

  /* raw_set()
     assign a value to the element identified by offset
  */
  template<typename Vt>
  inline  node_type* raw_set(off_t offset, Vt&& value) noexcept {
          node_type* l_result = at(offset);
          if(l_result) {
             *l_result = value;
          }
          return l_result;
  }

  /* raw_get_array()
     get a pointer to the first element in a contiguous array of the given size; if constructible, each element will be
     created with the arguments passed, in a manner similar to raw_get()
  */
  template<typename... Args>
  inline  node_type* raw_get_array(std::size_t count, Args&&... args) noexcept {
          node_type* l_tail = m_tail + count;
          if(l_tail < m_last) {
              node_type* l_result = m_tail;
              while(m_tail < l_tail) {
                  make_node(m_tail++, std::forward<Args>(args)...);
              }
              return l_result;
          }
          std::size_t l_size = l_tail - m_base;
          if(resize(l_size)) {
              return raw_get_array(count, std::forward<Args>(args)...);
          } else
              return nullptr;
  }

  /* raw_unget()
     free the space occupied by the last <count> elements
  */
  inline  node_type* raw_unget(std::size_t count = 1u) noexcept {
          node_type* l_tail = m_tail - count;
          if(l_tail < m_head) {
              l_tail = m_head;
          }
          if constexpr (is_node_destructible) {
              std::size_t l_count = m_tail - l_tail;
              if(l_count) {
                  if(l_count > 1) {
                      free_list(l_tail, m_tail);
                  } else
                      free_node(m_tail);
              }
          }
          return m_tail = l_tail;
  }

  /* at()
     get object at given position, with bounds checking
  */
  inline  node_type* at(off_t offset) const noexcept {
          if(m_base) {
              if(offset > 0) {
                  if(auto l_result = m_head + offset; l_result < m_tail) {
                      return l_result;
                  }
              } else
              if(offset < 0) {
                  if(auto l_result = m_tail + offset; l_result >= m_head) {
                      return l_result;
                  }
              } else
                  return  m_head;
          }
          return nullptr;
  }

  /* clear()
     remove all used items
  */
  inline  void clear() noexcept {
          if constexpr (is_node_destructible) {
              free_list(m_head, m_tail);
          }
          m_tail = m_head;
  }

  public:
  /**/    pool_base() noexcept:
          pool_base(resource_type()) {
  }

    /**/  pool_base(
            const resource_type& resource,
            std::size_t  e_min = 0,
            std::size_t  e_max = std::numeric_limits<unsigned int>::max()
          ) noexcept:
          m_resource(resource),
          m_policy(),
          m_align(alignof(node_type)),
          m_size(0),
          m_count_min(0),
          m_count_max(0),
          m_base(nullptr),
          m_head(nullptr),
          m_tail(nullptr),
          m_last(nullptr) {
          if(e_max) {
              if(e_max < e_min) {
                  e_max = e_min;
              }
              if(e_max < std::numeric_limits<unsigned int>::max()) {
                  m_count_max = e_max;
              } else
                  m_count_max = std::numeric_limits<unsigned int>::max();
          }
          if(e_min) {
              if(e_min < std::numeric_limits<unsigned int>::max()) {
                  m_count_min = e_min;
              } else
                  m_count_min = std::numeric_limits<unsigned int>::max();
              reserve(m_count_min);
          }
  }

          pool_base(const pool_base&) noexcept = delete;
          pool_base(pool_base&&) noexcept = delete;

  /**/    ~pool_base() {
          if(m_size) {
              clear();
              m_resource.deallocate(m_base, m_size * node_size, m_align);
          }
  }

  inline  std::size_t get_align() const noexcept {
          return m_align;
  }

  inline  ptrdiff_t   get_head_pos() const noexcept {
          return 0;
  }

  inline  ptrdiff_t   get_tail_pos() const noexcept {
          return m_tail - m_head;
  }

  inline  std::size_t get_capacity() const noexcept {
          return m_last - m_head;
  }

  inline  std::size_t get_capacity(ssize_t value) const noexcept {
          return get_capacity() + value;
  }

  inline  std::size_t get_used_size() const noexcept {
          return m_tail - m_base;
  }

  inline  std::size_t get_used_size(ssize_t value) const noexcept {
          return get_used_size() + value;
  }

  inline  std::size_t get_free_size() const noexcept {
          return m_last - m_tail;
  }

  inline  std::size_t get_free_size(ssize_t value) const noexcept {
          return get_free_size() + value;
  }

          pool_base& operator=(const pool_base&) noexcept = delete;
          pool_base& operator=(pool_base&&) noexcept = delete;
};

/*namespace memory*/ }
#endif
