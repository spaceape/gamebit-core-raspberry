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
#include "node.h"
#include "directory.h"
#include "device.h"

namespace sys {

      node::node(device* device_ptr) noexcept:
      m_name(),
      m_root(nullptr),
      m_node_prev(nullptr),
      m_node_next(nullptr),
      m_device_ptr(device_ptr)
{
      if(device_ptr != nullptr) {
          device_ptr->hook();
      }
}

      node::node(const char* name, directory* directory_ptr, device* device_ptr) noexcept:
      m_name(name),
      m_root(nullptr),
      m_node_prev(nullptr),
      m_node_next(nullptr),
      m_device_ptr(device_ptr)
{
      if(device_ptr != nullptr) {
          device_ptr->hook();
      }
      if(directory_ptr != nullptr) {
          directory_ptr->link_node(this);
      }
}

      node::node(const node& copy) noexcept:
      node(copy.m_name, copy.m_root, copy.m_device_ptr)
{
}

      node::node(node&& copy) noexcept:
      node(copy.m_name, copy.m_root, copy.m_device_ptr)
{
      copy.m_device_ptr->drop();
      copy.m_device_ptr = nullptr;
}

      node::~node()
{
      if(m_root != nullptr) {
          m_root->drop_node(this);
      }
      if(m_device_ptr != nullptr) {
          m_device_ptr->drop();
      }
}

void  node::enable_device_rc() noexcept
{
      if(m_device_ptr) {
          m_device_ptr->set_rc_enabled();
      }
}

void  node::disable_device_rc() noexcept
{
      if(m_device_ptr) {
          m_device_ptr->set_rc_disabled();
      }
}

void  node::adt_attach(directory*) noexcept
{
}

void  node::adt_detach(directory*) noexcept
{
}

bool  node::has_root() const noexcept
{
      return m_root != nullptr;
}

bool  node::has_root(directory* root) const noexcept
{
      return m_root == root;
}

bool  node::has_name(const char* name, int name_length) const noexcept
{
      return m_name == name;
}

auto  node::get_name() const noexcept -> const char*
{
      return m_name;
}

device* node::get_device() const noexcept
{
      return m_device_ptr;
}

device* node::get_device_hook() noexcept
{
      return m_device_ptr->hook();
}

void  node::sync(int dt) noexcept
{
      m_device_ptr->sync(dt);
}

bool  node::is_attached() const noexcept
{
      return m_root != nullptr;
}

node& node::operator=(const node& rhs) noexcept
{
      if(std::addressof(rhs) != this) {
          m_name = rhs.m_name;
          if(m_device_ptr != rhs.m_device_ptr) {
              if(m_device_ptr != nullptr) {
                  m_device_ptr->drop();
              }
              if(m_device_ptr = rhs.m_device_ptr; m_device_ptr != nullptr) {
                  m_device_ptr->hook();
              }
          }
      }
      return *this;
}

node& node::operator=(node&& rhs) noexcept
{
      if(std::addressof(rhs) != this) {
          m_name = rhs.m_name;
          if(m_device_ptr != rhs.m_device_ptr) {
              if(m_device_ptr != nullptr) {
                  m_device_ptr->drop();
              }
              m_device_ptr = rhs.m_device_ptr;
              rhs.m_device_ptr = nullptr;
          }
      }
      return *this;
}

/*namespace sys*/ }
