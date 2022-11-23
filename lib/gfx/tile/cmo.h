#ifndef gfx_cmo_h
#define gfx_cmo_h
/** 
    Copyright (c) 2021, wicked systems
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
#include <gfx.h>
#include <gfx/tile.h>
#include <gfx/ac.h>
#include <gfx/config.h>

namespace gfx {

/* cmo
   colour map object with copy-on-write capabilities
*/
class cmo: public ac
{
  protected:
  class ref
  {
    std::uint8_t    m_format;
    short int       m_count;
    std::uint8_t*   m_data;
    short int       m_size;
    short int       m_hooks:15;
    bool            m_dynamic:1;    // memory for the object is allocated dynamically
    
    public:
            ref() noexcept;
            ref(unsigned int, int) noexcept;
            ref(unsigned int, int, std::uint8_t*, std::size_t) noexcept;
            ref(const ref&) noexcept = delete;
            ref(ref&&) noexcept = delete;
            ~ref();

            unsigned int  get_format() const noexcept;
            
            void    reset() noexcept;
            bool    reset(unsigned int, int) noexcept;
            bool    reset(unsigned int, int, std::uint8_t*, std::size_t) noexcept;

            std::uint8_t* get_data_ptr() const noexcept;
            std::uint8_t* get_data_at(int) const noexcept;
            int           get_colour_count() const noexcept;
            int           get_data_size() const noexcept;

            ref&    operator=(const ref&) noexcept = delete;
            ref&    operator=(ref&&) noexcept = delete;
            
            ref*    hook() noexcept;
            ref*    drop() noexcept;
  };

  class ptr
  {
    ref*    m_ref;

    public:
            ptr() noexcept;
            ptr(ref*) noexcept;
            ptr(const ptr&) noexcept;
            ptr(ptr&&) noexcept;
            ~ptr();
            ref*    get() const noexcept;
            ref*    clone() const noexcept;
            void    reset() noexcept;
            ref*    release() noexcept;

            ref*    operator->() const noexcept;

                    operator ref*() const noexcept;

            ptr&    operator=(std::nullptr_t) noexcept;
            ptr&    operator=(ref*) noexcept;
            ptr&    operator=(const ptr&) noexcept;
            ptr&    operator=(ptr&&) noexcept;
  };
  
  protected:
  ptr     m_ptr;

  public:
  static  constexpr  unsigned int  fmt_rgb  = 16;
  static  constexpr  unsigned int  fmt_xrgb = 32;
  static  constexpr  unsigned int  fmt_argb = 64;
  static  constexpr  unsigned int  fmt_rgb_232    = fmt_rgb  | 0;
  static  constexpr  unsigned int  fmt_rgb_565    = fmt_rgb  | 1;
  static  constexpr  unsigned int  fmt_rgb_888    = fmt_rgb  | 2;
  static  constexpr  unsigned int  fmt_xrgb_0222  = fmt_xrgb | 0;
  static  constexpr  unsigned int  fmt_xrgb_0444  = fmt_xrgb | 1;
  static  constexpr  unsigned int  fmt_xrgb_0888  = fmt_xrgb | 3;
  static  constexpr  unsigned int  fmt_argb_2222  = fmt_argb | 0;
  static  constexpr  unsigned int  fmt_argb_4444  = fmt_argb | 1;
  static  constexpr  unsigned int  fmt_argb_8888  = fmt_argb | 3;
  static  constexpr  unsigned int  fmt_argb_float = fmt_argb | 15;

  static  constexpr  unsigned int  fmt_size_bits  = 15;
  
  protected:
  template<typename... Args>
  static  ref*   make_ptr(Args&&... args) noexcept {
          return new(std::nothrow) ref(std::forward<Args>(args)...);
  }

  static  int    get_colour_size(unsigned int) noexcept;
  static  int    get_palette_size(unsigned int, int) noexcept;
  
  public:
          cmo() noexcept;
          cmo(unsigned int, int) noexcept;
          cmo(unsigned int, int, std::uint8_t*, std::size_t) noexcept;
          cmo(const cmo&) noexcept;
          cmo(cmo&&) noexcept;
          ~cmo();

          bool   reset(unsigned int, int) noexcept;
          bool   reset(unsigned int, int, std::uint8_t*, std::size_t) noexcept;

          bool   set_uint8(int, std::uint8_t) noexcept;
          bool   set_uint32(int, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t) noexcept;
          bool   set_uint32(int, std::uint32_t) noexcept;

          void   dispose() noexcept;
          void   release() noexcept;

  inline  std::uint8_t* get_data_ptr() const noexcept {
          return m_ptr->get_data_ptr();
  }
  
  inline  int    get_data_size() const noexcept {
          return m_ptr->get_data_size();
  }
  
          operator bool() const noexcept;

          cmo&  operator=(const cmo&) noexcept;
          cmo&  operator=(cmo&&) noexcept;
};

/*namespace gfx*/ }
#endif
