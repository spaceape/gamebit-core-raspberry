#ifndef sys_adt_device_h
#define sys_adt_device_h
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
#include "tree.h"

/* device
*/
namespace sys {

class device
{
  unsigned short int  m_type;
  short int           m_hooks;

  protected:
  /* path_t
     utility struct necessary to trace path elements for get_next()
  */
  struct path_t
  {
    path_t* prev;
    device* node;
  };

  public:
  /* type_*
  */
  static constexpr unsigned int type_undef = 0u;

  /* uuid_*
  */
  static constexpr unsigned int uuid_default = 0u;

  /* type
     the device type for this class (each device object will get its own)
  */
  static constexpr unsigned int type = type_undef;

  protected:
          void     set_rc_enabled() noexcept;
          void     set_rc_disabled() noexcept;
  virtual device*  get_next(char*, path_t*) noexcept;
  friend  class node;

  public:
          device(unsigned int) noexcept;
          device(const device&) noexcept = delete;
          device(device&&) noexcept = delete;
  virtual ~device();

          unsigned int get_type() const noexcept;
          bool     has_type(unsigned int) const noexcept;
  virtual unsigned int get_id() const noexcept;
          bool     has_id(unsigned int) const noexcept;

  virtual device*  get_entry_by_name(const char*, int = 0) noexcept;
  virtual device*  get_entry_by_index(int) noexcept;
  virtual int      get_entry_count() noexcept;
          device*  get_at(char*) noexcept;

          device*  hook() noexcept;
          device*  drop() noexcept;          

  static  device*  get_from_adt(const char* path) noexcept {
          return   adt_get_device(path);
  }

  template<typename Xt>
  static  Xt*      cast_from_adt(const char* path) noexcept {
          return   static_cast<Xt*>(adt_get_device(path));
  }

  template<typename Xt, typename... Args>
  static  Xt*       make(directory* root, const char* name, Args&&... args) noexcept {
          if(auto
              l_node = new(std::nothrow) dynamic_mount_point<Xt>(root, name, std::forward<Args>(args)...);
              l_node != nullptr) {
              return l_node->get_ptr();
          }
          return nullptr;
  }

  virtual void     yield(unsigned int) const noexcept;
  virtual void     sync(int) noexcept;
  virtual void     list(int = 0) noexcept;

          device&  operator=(const device&) noexcept = delete;
          device&  operator=(device&&) noexcept = delete;
};

/*namespace sys*/ }
#endif
