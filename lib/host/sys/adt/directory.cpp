/** 
    Copyright (c) 2022, wicked systems
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
#include "directory.h"
#include "node.h"
#include "pico/stdio.h"

namespace sys {

      directory::directory() noexcept:
      device(type),
      m_node_head(nullptr),
      m_node_tail(nullptr)
{
}

      directory::~directory()
{
      while(m_node_tail)
      {
          drop_node(m_node_tail);
      }
}

bool  directory::link_node(node* node_ptr) noexcept
{
      if(node_ptr->m_root == nullptr) {
          node_ptr->m_root = this;
          node_ptr->m_node_next = nullptr;
          node_ptr->m_node_prev = m_node_tail;
          if(node_ptr->m_node_prev != nullptr) {
              node_ptr->m_node_prev->m_node_next = node_ptr;
          } else
              m_node_head = node_ptr;
          m_node_tail = node_ptr;
          m_node_tail->adt_attach(this);
          return true;
      }
      return false;
}

device*  directory::get_entry_by_name(const char* name, int name_length) noexcept
{
      node* i_node = m_node_head;
      while(i_node) {
          if(i_node->has_name(name)) {
              return i_node->get_device();
          }
          i_node = i_node->m_node_next;
      }
      return nullptr;
}

device*  directory::get_entry_by_index(int index) noexcept
{
      int   l_index = 0;
      node* i_node = m_node_head;
      while(i_node) {
          if(l_index == index) {
              return i_node->get_device();
          }
          ++l_index;
          i_node = i_node->m_node_next;
      }
      return nullptr;
}

int   directory::get_entry_count() noexcept
{
      int   l_count = 0;
      node* i_node = m_node_head;
      while(i_node) {
          ++l_count;
          i_node = i_node->m_node_next;
      }
      return l_count;
}

bool  directory::drop_node(node* node_ptr) noexcept
{
      if(node_ptr->m_root == this) {
          if(node_ptr->m_node_next != nullptr) {
              node_ptr->m_node_next->m_node_prev = node_ptr->m_node_prev;
          } else
              m_node_tail = node_ptr->m_node_prev;
          if(node_ptr->m_node_prev != nullptr) {
              node_ptr->m_node_prev->m_node_next = node_ptr->m_node_next;
          } else
              m_node_head = node_ptr->m_node_next;
          node_ptr->m_node_next = nullptr;
          node_ptr->m_node_prev = nullptr;
          node_ptr->m_root = nullptr;
          node_ptr->adt_detach(this);
      }
      return true;
}


void  directory::sync(int dt) noexcept
{
      node* i_node = m_node_head;
      while(i_node) {
          if(device*
              l_device = i_node->get_device();
              l_device != nullptr) {
              l_device->sync(dt);
          }
          i_node = i_node->m_node_next;
      }
}

void  directory::list(int depth) noexcept
{
      node* i_node = m_node_head;
      while(i_node) {
          for(int i_indent = 0; i_indent < depth; i_indent++) {
              printf("  ");
          }
          printf("%s\n", i_node->get_name());
          if(device*
              l_device = i_node->get_device();
              l_device != nullptr) {
              l_device->list(depth + 1);
          }
          i_node = i_node->m_node_next;
      }
}

/*namespace sys*/ }
