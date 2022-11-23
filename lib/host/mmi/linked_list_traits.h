#ifndef mmi_linked_list_traits_h
#define mmi_linked_list_traits_h
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
#include <memory.h>
#include <traits.h>

namespace mmi {
namespace linked_list_traits {

  struct has_prev_impl {
    template<typename Xt>
    static constexpr auto test(int) -> decltype(std::declval<Xt>().get_prev());

    template<typename Xt>
    static constexpr auto test(...) -> std::false_type;
  };

  template<typename Xt>
  struct has_prev: std::conditional<
          std::is_same<Xt*, decltype(has_prev_impl::test<Xt>(0))>::value,
          std::true_type, 
          std::false_type
      >::type {
  };

  struct has_next_impl {
    template<typename Xt>
    static constexpr auto test(int) -> decltype(std::declval<Xt>().get_next());

    template<typename Xt>
    static constexpr auto test(...) -> std::false_type;
  };

  template<typename Xt>
  struct has_next: std::conditional<
          std::is_same<Xt*, decltype(has_next_impl::test<Xt>(0))>::value,
          std::true_type, 
          std::false_type
      >::type {
  };

  /* forward_iterator
   * forward iterator for linked list nodes
  */
  template<typename Xt, typename Rt = Xt>
  class  forward_iterator
  {
    public:
    using node_type = typename std::remove_cv<Xt>::type;
    using value_type = Rt;

    private:
    node_type* m_node;

    public:
    inline  forward_iterator() noexcept:
            m_node(nullptr) {
    }

    inline  forward_iterator(node_type* node) noexcept:
            m_node(node) {
    }

    inline  forward_iterator(node_type& node) noexcept:
            m_node(std::addressof(node)) {
    }

    inline  forward_iterator(const forward_iterator& copy) noexcept:
            m_node(copy.m_node) {
    }

    inline  forward_iterator(forward_iterator&& copy) noexcept:
            m_node(copy.m_node) {
            copy.m_node = nullptr;
    }

    inline  forward_iterator& next() noexcept {
            if(m_node) {
                m_node = m_node->m_next;
            }
            return *this;
    }

    inline  forward_iterator& operator++() noexcept {
            return next();
    }

    inline  forward_iterator  operator++(int) noexcept {
            return next();
    }
            
    inline  value_type& operator->() noexcept {
            return *m_node;
    }
    
    inline  forward_iterator& operator=(const forward_iterator& rhs) noexcept {
            m_node = rhs.m_node;
            return *this;
    }
    
    inline  forward_iterator& operator=(forward_iterator&& rhs) noexcept {
            m_node = rhs.m_node;
            rhs.m_node = nullptr;
            return *this;
    }
  };
  
  /* reverse_iterator
   * generic reverse iterator for linked list nodes
  */
  template<typename Xt, typename Rt = Xt>
  class  reverse_iterator
  {
    public:
    using node_type = typename std::remove_cv<Xt>::type;
    using value_type = Rt;

    private:
    node_type* m_node;

    public:
    inline  reverse_iterator() noexcept:
            m_node(nullptr) {
    }

    inline  reverse_iterator(node_type* node) noexcept:
            m_node(node) {
    }

    inline  reverse_iterator(node_type& node) noexcept:
            m_node(std::addressof(node)) {
    }

    inline  reverse_iterator(const reverse_iterator& copy) noexcept:
            m_node(copy.m_node) {
    }

    inline  reverse_iterator(reverse_iterator&& copy) noexcept:
            m_node(copy.m_node) {
            copy.m_node = nullptr;
    }

    inline  reverse_iterator& prev() noexcept {
            if(m_node) {
                m_node = m_node->m_prev;
            }
            return *this;
    }

    inline  reverse_iterator& operator--() noexcept {
            return prev();
    }

    inline  reverse_iterator  operator--(int) noexcept {
            return prev();
    }
            
    inline  value_type& operator->() noexcept {
            return *m_node;
    }
    
    inline  reverse_iterator& operator=(const reverse_iterator& rhs) noexcept {
            m_node = rhs.m_node;
            return *this;
    }
    
    inline  reverse_iterator& operator=(reverse_iterator&& rhs) noexcept {
            m_node = rhs.m_node;
            rhs.m_node = nullptr;
            return *this;
    }
  };

  /* iterator
   * bidirectional iterator for linked list nodes
  */
  template<typename Xt, typename Rt = Xt>
  class  iterator
  {
    public:
    using node_type = typename std::remove_cv<Xt>::type;
    using value_type = Rt;

    private:
    node_type* m_node;

    public:
    inline  iterator() noexcept:
            m_node(nullptr) {
    }

    inline  iterator(node_type* node) noexcept:
            m_node(node) {
    }

    inline  iterator(node_type& node) noexcept:
            m_node(std::addressof(node)) {
    }

    inline  iterator(const iterator& copy) noexcept:
            m_node(copy.m_node) {
    }

    inline  iterator(iterator&& copy) noexcept:
            m_node(copy.m_node) {
            copy.m_node = nullptr;
    }

    inline  iterator& prev() noexcept {
            if(m_node) {
                m_node = m_node->m_prev;
            }
            return *this;
    }

    inline  iterator& next() noexcept {
            if(m_node) {
                m_node = m_node->m_next;
            }
            return *this;
    }

    inline  iterator& operator--() noexcept {
            return prev();
    }

    inline  iterator  operator--(int) noexcept {
            return prev();
    }
            
    inline  iterator& operator++() noexcept {
            return next();
    }

    inline  iterator  operator++(int) noexcept {
            return next();
    }
            
    inline  value_type* operator->() noexcept {
            return  m_node;
    }
    
    inline  bool    operator==(iterator&) noexcept {
            return  m_node == nullptr;
    }

    inline  bool    operator==(nullptr_t) noexcept {
            return  m_node == nullptr;
    }
    
    inline  bool    operator==(node_type* node) noexcept {
            return  m_node == node;
    }
    
    inline  bool    operator==(node_type& node) noexcept {
            return  m_node == std::addressof(node);
    }
    
    inline  iterator& operator=(const iterator& rhs) noexcept {
            m_node = rhs.m_node;
            return *this;
    }
    
    inline  iterator& operator=(iterator&& rhs) noexcept {
            m_node = rhs.m_node;
            rhs.m_node = nullptr;
            return *this;
    }
  };
  
/*namespace linked_list_traits*/ }
/*namespace mmi*/ }
#endif
