#ifndef gfx_cso_h
#define gfx_cso_h
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

/* cso
   character set object with copy-on-write capabilities;
   contains the glyphs that make up a tile set;
   1 and 8 bits per pixel format supported
*/
class cso: public ac
{
  protected:
  class ref
  {
    std::uint8_t    m_format;
    short int       m_sx;
    short int       m_sy;
    std::uint8_t*   m_data;
    short int       m_size;
    short int       m_hooks;
    
    public:
            ref() noexcept;
            ref(unsigned int, int, int) noexcept;
            ref(unsigned int, int, int, std::uint8_t*, std::size_t) noexcept;
            ref(const ref&) noexcept = delete;
            ref(ref&&) noexcept = delete;
            ~ref();

            unsigned int get_format() const noexcept;
            int     get_glyph_sx() const noexcept;
            int     get_glyph_sy() const noexcept;

            void    reset() noexcept;
            bool    reset(unsigned int, int, int) noexcept;
            bool    reset(unsigned int, int, int, std::uint8_t*, std::size_t) noexcept;

            std::uint8_t* get_data() const noexcept;
            int           get_size() const noexcept;

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
  static  constexpr  unsigned int  fmt_1bpp = 1;
  static  constexpr  unsigned int  fmt_8bpp = 8;

  protected:
  protected:
  static constexpr std::size_t get_line_size(unsigned int format, std::uint16_t sx) noexcept {
          if(sx & 7) {
              sx &= ~7;
              sx += 8;
          }
          if(format == fmt_1bpp) {
              return sx / 8;
          } else
          if(format == fmt_8bpp) {
              return sx;
          }
          return 0;
  }

  static constexpr std::size_t get_glyph_size(unsigned int format, std::uint16_t sx, std::uint16_t sy) noexcept {
          return get_line_size(format, sx) * sy;
  }

  static  constexpr std::size_t get_data_size(unsigned int format, int sx, int sy) noexcept {
          return get_glyph_size(format, sx, sy) * charset_size;
  }

  template<typename... Args>
  static  ref*   make_ptr(Args&&... args) noexcept {
          return new(std::nothrow) ref(std::forward<Args>(args)...);
  }

  public:
          cso() noexcept;
          cso(unsigned int, int, int) noexcept;
          cso(unsigned int, int, int, std::uint8_t*, std::size_t) noexcept;
          cso(const cso&) noexcept;
          cso(cso&&) noexcept;
          ~cso();

          bool   reset(unsigned int, int, int) noexcept;
          bool   reset(unsigned int, int, int, std::uint8_t*, std::size_t) noexcept;
          void   dispose() noexcept;
          void   release() noexcept;

  inline  std::uint8_t* get_data_ptr() const noexcept {
          return m_ptr->get_data();
  }

  inline  std::uint8_t* get_glyph_ptr(int glyph_index, int line_index = 0) const noexcept {

          unsigned int l_format = m_ptr->get_format();
          int l_sx = m_ptr->get_glyph_sx();
          int l_sy = m_ptr->get_glyph_sy();
          return m_ptr->get_data() + (glyph_index * get_glyph_size(l_format, l_sx, l_sy)) + (line_index * get_line_size(l_format, l_sx));
  }

  inline  int    get_data_size() const noexcept {
          return m_ptr->get_size();
  }
  
          operator bool() const noexcept;

          cso&  operator=(const cso&) noexcept;
          cso&  operator=(cso&&) noexcept;
};

/*namespace gfx*/ }
#endif
