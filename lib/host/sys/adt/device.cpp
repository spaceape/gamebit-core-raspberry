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
#include "device.h"
#include <log.h>

constexpr char chr_path_split = '/';
constexpr char chr_path_end = 0;

namespace sys {

      device::device(unsigned int type) noexcept:
      m_type(type),
      m_hooks(1)
{
}

      device::~device()
{
}

void  device::set_rc_enabled() noexcept
{
      m_hooks = 1;
}

void  device::set_rc_disabled() noexcept
{
      // setting hooks to zero disables ref counting for the object;
      // this will prevent the object from being freed when the device is not a dynamically allocated
      m_hooks = 0;
}

device*  device::get_next(char* path_ptr, path_t* head_ptr) noexcept
{
      path_t l_trace;
      char*  l_root     = path_ptr;
      char*  l_next     = path_ptr;
      bool   l_has_root = false;
      bool   l_has_next = false;
      // extract current path element and seek to the next
      while(l_has_root == false) {
          if((*l_next == chr_path_split) ||
              (*l_next == chr_path_end)) {
              l_has_root = true;
              l_has_next = *l_next == chr_path_split;
              *l_next = 0;
          }
          l_next++;
      }
      // process current path element
      if(l_has_root) {
          int  l_root_length = l_next - l_root;
          if(l_root[0] == '.') {
              if(l_root[1] == '.') {
                  if(head_ptr != nullptr) {
                      return head_ptr->node->get_next(l_next, head_ptr->prev);
                  }
                  return nullptr;
              }
              return this;
          }
          device* l_device_ptr = get_entry_by_name(l_root, l_root_length);
          if(l_has_next) {
              if(l_device_ptr) { 
                  l_trace.prev = head_ptr;
                  l_trace.node = this;
                  return l_device_ptr->get_next(l_next, std::addressof(l_trace));
              }
          }
          return l_device_ptr;
      }
      return nullptr;
}

unsigned int  device::get_type() const noexcept
{
      return m_type;
}

bool  device::has_type(unsigned int type) const noexcept
{
      return (m_type & type) == type;
}

unsigned int  device::get_id() const noexcept
{
      return uuid_default;
}

bool  device::has_id(unsigned int id) const noexcept
{
      return get_id() == id;
}

device*  device::get_entry_by_name(const char*, int) noexcept
{
      return nullptr;
}

device*  device::get_entry_by_index(int) noexcept
{
      return nullptr;
}

int   device::get_entry_count() noexcept
{
      return 0;
}

device*  device::get_at(char* path_ptr) noexcept
{
      return get_next(path_ptr, nullptr);
}

device*  device::hook() noexcept
{
      if(m_hooks < std::numeric_limits<short int>::max()) {
          if(m_hooks > 0) {
              ++m_hooks;
          }
          return this;
      }
      return nullptr;
}

device*  device::drop() noexcept
{
      if(m_hooks > 0) {
          --m_hooks;
          if(m_hooks == 0) {
              delete this;
          }
      }
      return nullptr;
}

void  device::yield(unsigned int) const noexcept
{
}

void  device::sync(int) noexcept
{
}

void  device::list(int) noexcept
{
}
/*namespace sys*/ }
