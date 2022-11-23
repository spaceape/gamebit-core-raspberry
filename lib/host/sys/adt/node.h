#ifndef sys_node_h
#define sys_node_h
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
#include <sys.h>
#include <sys/adt.h>
#include <char.h>

/* node
*/
namespace sys {

class node
{
  char_ptr<>  m_name;
  directory*  m_root;
  node*       m_node_prev;
  node*       m_node_next;

  protected:
  device*     m_device_ptr;

  protected:
          void  enable_device_rc() noexcept;
          void  disable_device_rc() noexcept;
  virtual void  adt_attach(directory*) noexcept;
  virtual void  adt_detach(directory*) noexcept;
  friend  class directory;

  public:
          node(device* = nullptr) noexcept;
          node(const char*, directory*, device* = nullptr) noexcept;
          node(const node&) noexcept;
          node(node&&) noexcept;
  virtual ~node();

          bool    has_root() const noexcept;
          bool    has_root(directory*) const noexcept;
          bool    has_name(const char*, int = 0) const noexcept;
          auto    get_name() const noexcept -> const char*;
          device* get_device() const noexcept;
          device* get_device_hook() noexcept;

          void    sync(int) noexcept;

          bool    is_attached() const noexcept;

          device* operator->() const noexcept;
          node&   operator=(const node&) noexcept;
          node&   operator=(node&&) noexcept;
};
/*namespace sys*/ }
#endif
