#ifndef mmi_linked_list_base_h
#define mmi_linked_list_base_h
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
#include "linked_list_traits.h"

namespace mmi {

/* linked_list_base
 * intrusive linked list
 * Xt - node type
 * assume_contiguous_ptr - optimize search algorithms by assuming that the nodes occupy contiguous memory
 * assume_ordered_ptr - optimize search algorithms by assuming nodes' adresses are in order
*/
template<typename Xt, bool assume_contiguous_ptr = false, bool assume_ordered_ptr = assume_contiguous_ptr>
struct linked_list_base
{
  using node_type = typename std::remove_cv<Xt>::type;

  static constexpr bool has_prev = linked_list_traits::has_prev<node_type>::value;
  static constexpr bool has_next = linked_list_traits::has_next<node_type>::value;
  static constexpr bool has_contiguous_ptr = assume_contiguous_ptr;
  static constexpr bool has_ordered_ptr = assume_contiguous_ptr || assume_ordered_ptr;

  static_assert(has_next | has_prev, "linked_list_base requires node_type be either forward-, reverse-, or doubly linked.");

  protected:
  node_type* m_head;
  node_type* m_tail;
  node_type* m_last;
  node_type* m_iter;

  protected:
  inline  node_type* get_last() const noexcept {
          return m_last;
  }

  inline  node_type* get_iter() const noexcept {
          return m_iter;
  }

  inline  node_type* get_prev(node_type* node) noexcept {
          node_type* result = nullptr;
          if constexpr (has_prev) {
              result = node->get_prev();
          } else
          if constexpr (has_contiguous_ptr) {
              if(node > m_head) {
                  if(node <= m_tail) {
                      result = node - 1;
                  }
              }
          } else
          if(m_head) {
            //find prev node by iterating forward throug each node
              if constexpr (has_ordered_ptr) {
                  if(m_iter) {
                      if(m_iter > node) {
                          m_iter = m_head;
                      }
                  } else
                      m_iter = m_head;
              } else
                  m_iter = m_head;

              m_last = nullptr;
              while(m_iter) {
                  if(m_iter == node) {
                      result = m_last;
                      break;
                  }
                  m_last = m_iter;
                  m_iter = m_iter->get_next();
              }
          }
          return result;
  }

  inline  node_type* get_next(node_type* node) noexcept {
          node_type* result = nullptr;
          if constexpr (has_next) {
              result = node->get_next();
          } else
          if constexpr (has_contiguous_ptr) {
              if(node < m_tail) {
                  result = node + 1;
              }
          } else
          if(m_tail) {
          //find next node by iterating backward throug each node
              if constexpr (has_ordered_ptr) {
                  if(m_iter) {
                      if(m_iter < node) {
                          m_iter = m_tail;
                      }
                  } else
                      m_iter = m_tail;
              } else
                  m_iter = m_tail;

              m_last = nullptr;
              while(m_iter) {
                  if(m_iter == node) {
                      result = m_last;
                      break;
                  }
                  m_last = m_iter;
                  m_iter = m_iter->get_prev();
              }
          }
          return result;
  }

  inline  node_type* insert(node_type* node, node_type* prev, node_type* next) {
          if(prev) {
              if constexpr (has_next) {
                  prev->set_next(node);
              }
              if constexpr (has_prev) {
                  node->set_prev(prev);
              }
          } else
              m_head = node;
          if(next) {
              if constexpr (has_prev) {
                  next->set_prev(node);
              }
              if constexpr (has_next) {
                  node->set_next(next);
              }
          } else
              m_tail = node;
          m_last = prev;
          m_iter = node;
          return node;
  }

  public:
  class iterator
  {
    node_type* m_node;

    public:
    inline  iterator() noexcept:
            m_node(nullptr) {
    }

    inline  iterator(node_type* node) noexcept:
            m_node(node) {
    }

    inline  iterator(const iterator& copy) noexcept:
            m_node(copy.node) {
    }

    inline  iterator(iterator&& copy) noexcept:
            m_node(copy.node) {
    }

    inline  ~iterator() {
    }

    inline  bool has_node(node_type* value) const noexcept {
            return m_node == value;
    }

    inline  node_type*  get_node() const noexcept {
            return m_node;
    }

    inline  bool reset() noexcept {
            m_node = nullptr;
            return false;
    }

    inline  node_type* release() noexcept {
            node_type* l_result = m_node;
            if(l_result) {
                reset();
            }
            return l_result;
    }

    inline  node_type& operator*() const noexcept {
            return *m_node;
    }

    inline  node_type* operator->() const noexcept {
            return  m_node;
    }

    inline  iterator operator++(int) noexcept {
            node_type* l_copy = m_node;
            if(l_copy) {
                m_node = m_node->get_next();
            }
            return l_copy;
    }

    inline  iterator operator--(int) noexcept {
            node_type* l_copy = m_node;
            if(l_copy) {
                m_node = m_node->get_prev();
            }
            return l_copy;
    }

    inline  bool operator==(const iterator& rhs) noexcept {
            return m_node == rhs.m_node;
    }

    inline  bool operator!=(const iterator& rhs) noexcept {
            return m_node != rhs.m_node;
    }

    inline  iterator& operator=(const iterator& rhs) noexcept {
            m_node = rhs.m_node;
            return *this;
    }

    inline  iterator& operator=(iterator&& rhs) noexcept {
            m_node = rhs.m_node;
            return *this;
    }

    inline  operator node_type*() const noexcept {
            return m_node;
    }
  };

  public:
  inline  linked_list_base() noexcept:
          m_head(nullptr),
          m_tail(nullptr),
          m_last(nullptr),
          m_iter(nullptr) {
  }

  inline  linked_list_base(const linked_list_base& copy) noexcept:
          m_head(copy.head),
          m_tail(copy.tail),
          m_last(copy.last),
          m_iter(copy.index) {
  }

  inline  linked_list_base(linked_list_base&& copy) noexcept:
          linked_list_base(copy) {
  }

  inline  iterator begin() const noexcept {
          return m_head;
  }

  inline  iterator end() const noexcept {
          return  nullptr;
  }

  inline  node_type* get_head() const noexcept {
          return m_head;
  }

  inline  node_type* get_tail() const noexcept {
          return m_tail;
  }

  inline  node_type* erase(node_type* node) noexcept {
          if(node == m_head) {
              if(node == m_tail) {
                  m_head = nullptr;
                  m_tail = nullptr;
                  m_last = nullptr;
                  m_iter = nullptr;
                  return node;
              }
          }
          if(node) {
              node_type* prev = get_prev(node);
              node_type* next = get_next(node);
              if(prev) {
                  if constexpr (has_next) {
                      prev->set_next(next);
                      node->set_next(nullptr);
                  }
              } else
                  m_head = next;
              if(next) {
                  if constexpr (has_prev) {
                      next->set_prev(prev);
                      node->set_prev(nullptr);
                  }
              } else
                  m_tail = prev;

              if(m_iter == node) {
                  if(prev) {
                      m_iter = prev;
                  } else
                      m_iter = m_head;
              }
          }
          return node;
  }

  inline  linked_list_base& operator=(const linked_list_base& rhs) noexcept {
          m_head = rhs.m_head;
          m_tail = rhs.m_tail;
          m_last = rhs.m_last;
          m_iter = rhs.m_iter;
          return *this;
  }

  inline  linked_list_base& operator=(linked_list_base&& rhs) noexcept {
          return operator=(rhs);
  }

  inline  operator node_type*() const noexcept {
          return m_head;
  }
};

/*namespace mmi*/ }
#endif
