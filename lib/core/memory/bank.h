#ifndef memory_bank_h
#define memory_bank_h
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
#include "page.h"

namespace memory {

/* bank
   memory pool
   Xt - data type
   PageSize - minumum number of elements a page can hold (default: 1024)
   ArraySize - array size (default: 1)
   Rt - resource type (default: heap)
*/
template<typename Xt, std::size_t PageSize, std::size_t ArraySize, typename Rt>
class bank: public page<Xt, global::get_quotient_value(PageSize, 8), ArraySize, Rt>
{
  static_assert(PageSize > 0, "PageSize of 0 is useless.");

  public:
  using  page_type     = page<Xt, global::get_quotient_value(PageSize, 8), ArraySize, Rt>;
  using  base_type     = page_type;
  using  node_type     = typename base_type::node_type;
  using  resource_type = typename base_type::resource_type;

  private:
  page_type*    m_page_iter;
  node_type*    m_page_pos;
  unsigned int  m_page_count;
  unsigned int  m_page_max;

  public:
  inline  bank(const resource_type& resource) noexcept:
          base_type(resource, PageSize, std::numeric_limits<unsigned int>::max()),
          m_page_iter(this),
          m_page_pos(nullptr),
          m_page_count(1),
          m_page_max(std::numeric_limits<unsigned int>::max()) {
  }

          bank(const bank&) noexcept = delete;
          bank(bank&&) noexcept = delete;

  inline  ~bank() {
  }

  template<typename... Args>
          node_type* emplace(Args&&... args) noexcept {
          node_type* l_result = m_page_iter->find(nullptr, m_page_pos);
          if(l_result) {
              return m_page_iter->make_node(l_result, std::forward<Args>(args)...);
          }
          if(m_page_count < m_page_max) {
              auto l_page_next = m_page_iter->get_next_page();
              if(l_page_next == nullptr) {
                  l_page_next = m_page_iter->make_next_page();
                  if(l_page_next != nullptr) {
                      m_page_iter = l_page_next;
                      m_page_pos  = nullptr;
                      m_page_count++;
                      l_result = emplace(std::forward<Args>(args)...);
                  }
              }
          }
          return l_result;
  }

          node_type* remove(node_type* node) noexcept {
          page_type* l_page_iter;
          node_type* l_page_pos;
          if(node) {
              l_page_iter = m_page_iter;
              l_page_pos  = nullptr;
              while(l_page_iter) {
                  if(l_page_iter->find(node, l_page_pos)) {
                      l_page_iter->free_node(node);
                      m_page_iter = l_page_iter;
                      m_page_pos  = l_page_pos;
                      break;
                  }
                  l_page_iter = l_page_iter->get_prev_page();
              }
              if(l_page_iter == nullptr) {
                  l_page_iter = m_page_iter->get_next_page();
                  while(l_page_iter) {
                      if(l_page_iter->find(node, l_page_pos)) {
                          l_page_iter->free_node(node);
                          m_page_iter = l_page_iter;
                          m_page_pos  = l_page_pos;
                          break;
                      }
                      l_page_iter = l_page_iter->get_next_page();
                  }
                  if(l_page_iter == nullptr) {
                      return nullptr;
                  }
              }
          }
          return node;
  }
  
  inline  page_type* get_root_page() noexcept {
          return this;
  }

  inline  page_type* get_current_page() const noexcept {
          return m_page_iter;
  }

  inline  node_type* get_current_node() const noexcept {
          return m_page_pos;
  }

          bank& operator=(const bank&) noexcept = delete;
          bank& operator=(bank&&) noexcept = delete;
};

/*namespace memory*/ }
#endif
