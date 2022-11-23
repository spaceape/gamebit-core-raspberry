#ifndef gfx_surface_h
#define gfx_surface_h
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
#include "gfx.h"
#include "dc.h"

namespace gfx {

class surface: public dc
{
  surface*      m_manager;            // device this is attached to
  void*         m_mid;                // id assigned by the manager
  surface*      m_attach_prev;
  surface*      m_attach_next;
  bool          m_allow_attach;

  public:
  /* map_*
     surface render flags
  */
  static constexpr unsigned int  map_none = 0;       // no mapping, surface will not be rendered
  static constexpr unsigned int  map_auto = 0;       // choose the default mapping (copy)
  static constexpr unsigned int  map_copy = 1;
  static constexpr unsigned int  map_opaque = 2;
  static constexpr unsigned int  map_volatile = 128;

  /* smp_*
     surface option flags
  */
  static constexpr unsigned int  opt_none = 0;
  static constexpr unsigned int  opt_request_tile_graphics = 1;
  static constexpr unsigned int  opt_request_raster_graphics = 2;
  static constexpr unsigned int  opt_request_vector_graphics = 4;
  static constexpr unsigned int  opt_opaque = 16;
  static constexpr unsigned int  opt_allow_attach = 128;
  static constexpr unsigned int  opt_public = 255;
  static constexpr unsigned int  opt_private = 0;
  static constexpr unsigned int  opt_default_flags = opt_request_tile_graphics | opt_opaque;
  static constexpr unsigned int  opt_graphics_flags = 
                                    opt_request_tile_graphics |
                                    opt_request_raster_graphics |
                                    opt_request_vector_graphics;
  static constexpr unsigned int  opt_any = std::numeric_limits<unsigned int>::max();
  static constexpr unsigned int  opt_all = opt_any;

  protected:
  surface*      m_attach_head;
  surface*      m_attach_tail;

  protected:
  virtual bool  gfx_prepare(device*) noexcept;
  virtual bool  gfx_attach_surface(surface*) noexcept;
  virtual bool  gfx_detach_surface(surface*) noexcept;
  virtual void  gfx_draw() noexcept;
  virtual void  gfx_sync(int) noexcept;
  virtual bool  gfx_release(device*) noexcept;

  friend class device;
  public:
          surface(bool) noexcept;
          surface(surface*, bool) noexcept;
          surface(const surface&) noexcept = delete;
          surface(surface&&) noexcept = delete;
  virtual ~surface();

          bool    attach(surface*) noexcept;
          bool    detach(surface*) noexcept;

          surface& operator=(const surface&) noexcept = delete;
          surface& operator=(surface&&) noexcept = delete;
};

/*namespace gfx*/ }
#endif
