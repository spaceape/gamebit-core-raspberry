#ifndef sys_adt_directory_h
#define sys_adt_directory_h
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
#include "device.h"
#include "tree.h"

/* device
*/
namespace sys {

class directory: public device
{
  node*   m_node_head;
  node*   m_node_tail;

  public:
  /* type
     the device type for this class
  */
  static constexpr unsigned int type = 1u;

  public:
          directory() noexcept;
          directory(const directory&) noexcept = delete;
          directory(directory&&) noexcept = delete;
  virtual ~directory();

  template<typename Xt, typename... Args>
  inline  node*    make_node(const char* name, Args&&... args) noexcept {
          auto l_node = new(std::nothrow) dynamic_mount_point<Xt>(this, name, std::forward<Args>(args)...);
          if(l_node->has_root(nullptr)) {
              delete l_node;
              return nullptr;
          }
          return l_node;
  }

  inline  node*    make_link(const char* name, node* source) noexcept {
          auto l_node = new(std::nothrow) linked_mount_point(this, name, source);
          if(l_node->has_root(nullptr)) {
              delete l_node;
              return nullptr;
          }
          return l_node;
  }
  
          bool     link_node(node*) noexcept;

  virtual device*  get_entry_by_name(const char*, int = 0) noexcept override;
  virtual device*  get_entry_by_index(int) noexcept override;
  virtual int      get_entry_count() noexcept override;
          
          bool     drop_node(node*) noexcept;
 
  static  directory* get_from_adt(const char* path) noexcept {
          return adt_cast<sys::directory>(path);
  }          

  virtual void     sync(int) noexcept override;
  
  virtual void     list(int = 0) noexcept override;

          directory&  operator=(const directory&) noexcept = delete;
          directory&  operator=(directory&&) noexcept = delete;
};

/*namespace sys*/ }
#endif
