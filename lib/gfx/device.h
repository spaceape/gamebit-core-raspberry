#ifndef gfx_scene_h
#define gfx_scene_h
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
#include "surface.h"
#include "tile/cmo.h"
#include "tile/cso.h"

namespace gfx {

/* device
   base class for a rendering device
*/
class device: public surface
{
  protected:
  struct mapping_t: public mapping_base_t
  {
    bool  valid_bit:1;
    bool  ready_bit:1;
  };

  cmo     m_dev_cmo;      // default device colour map object
  cso     m_dev_cso;      // default device charset object

  protected:
          void   sdr_surface_push(surface*, surface*&, mapping_base_t*&) noexcept;
          bool   sdr_surface_prepare(surface*, mapping_base_t*) noexcept;
          // void   sdr_surface_draw() noexcept;
          void   sdr_surface_pop(surface*, surface*&, mapping_base_t*&) noexcept;
          bool   sdr_surface_release(surface*) noexcept;

          auto   sdr_make_mapping(surface*, int, int, int, int) noexcept -> mapping_t*;
          bool   sdr_reset_mapping(surface*, mapping_t*, int, int, int, int) noexcept;
          auto   sdr_free_mapping(surface*, mapping_t*) noexcept -> mapping_t*;

  virtual bool   gfx_attach_surface(surface*) noexcept override;
  virtual void   gfx_render() noexcept;
  virtual bool   gfx_detach_surface(surface*) noexcept override;
  virtual void   gfx_sync(int) noexcept override;

  public:
          device() noexcept;
          device(const device&) noexcept = delete;
          device(device&&) noexcept = delete;
  virtual ~device();

          bool   map(surface*, int, int, int, int) noexcept;
          bool   remap(surface*, int, int, int, int) noexcept;
  virtual bool   set_format(surface*, unsigned int) noexcept;
  virtual bool   set_tile_options(surface*, int, int) noexcept;
  virtual bool   set_raster_options(surface*, int, int) noexcept;
  virtual bool   set_vector_options(surface*, int, int) noexcept;
  virtual int    get_display_sx() const noexcept = 0;
  virtual int    get_display_sy() const noexcept = 0;
          bool   unmap(surface*) noexcept;

          device& operator=(const device&) noexcept = delete;
          device& operator=(device&&) noexcept = delete;
};

/*namespace gfx*/ }
#endif
