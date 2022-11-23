#ifndef sys_tree_h
#define sys_tree_h
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
#include "node.h"

namespace sys {

template<typename Xt>
class static_mount_point: public node
{
  protected:
  Xt      m_device_inst;

  public:
  template<typename... Args>
          static_mount_point(directory* root, const char* name, Args&&... args) noexcept:
          node(name, root),
          m_device_inst(std::forward<Args>(args)...) {
          m_device_ptr = std::addressof(m_device_inst);
          // shouldn't let this device be reference counted as freeing it would end tragically
          disable_device_rc();
  }

  inline  static_mount_point(const static_mount_point& copy) noexcept = delete;
  inline  static_mount_point(static_mount_point&& copy) noexcept = delete;

  virtual  ~static_mount_point() {
  }

  inline  Xt* get_ptr() noexcept {
          return std::addressof(m_device_inst);
  }
  
  inline  Xt* operator->() noexcept {
          return std::addressof(m_device_inst);
  }

  inline  operator Xt*() noexcept {
          return std::addressof(m_device_inst);
  }

  inline  static_mount_point& operator=(const static_mount_point& copy) noexcept = delete;
  inline  static_mount_point& operator=(static_mount_point&& copy) noexcept = delete;
};

template<typename Xt>
class dynamic_mount_point: public static_mount_point<Xt>
{
  protected:
  virtual void  adt_attach(directory*) noexcept override {           
  }

  virtual void  adt_detach(directory*) noexcept override {
          delete this;
  }

  public:
  template<typename... Args>
          dynamic_mount_point(directory* root, const char* name, Args&&... args) noexcept:
          static_mount_point<Xt>(root, name, std::forward<Args>(args)...) {
  }

  virtual  ~dynamic_mount_point() {
  }
};

class linked_mount_point: public node
{
  protected:
  virtual void  adt_attach(directory*) noexcept override {           
  }

  virtual void  adt_detach(directory*) noexcept override {
          delete this;
  }

  public:
          linked_mount_point(directory* root, const char* name, node* source) noexcept:
          node(name, root) {
          if(source != nullptr) {
              m_device_ptr = source->get_device_hook();
          }
  }

  inline  ~linked_mount_point() {
  }
};

/*namespace sys*/ }
#endif
