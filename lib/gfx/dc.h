#ifndef gfx_dc_h
#define gfx_dc_h
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
#include "metrics.h"
#include "ac.h"
#include "tile.h"
#include "tile/cmo.h"
#include "tile/cso.h"
#include "tile/cbo.h"
#include "raster/pbo.h"

namespace gfx {

/* dc
   device context
*/
class dc: public ac
{
  static  device*   s_device;
  
  protected:
  /* mapping_base_t
     surface mapping
  */
  struct mapping_base_t
  {
    surface*        source;
    std::uint8_t    format;
    std::uint16_t   render_flags;       // blending mode described by surface::map_* flags
    std::uint16_t   option_flags;
    short int       px;
    short int       py;
    short int       sx;
    short int       sy;
    short int       dx;                 // horizontal offset at which the logical window is mapped onto the physical window
    short int       dy;                 // vertical offset at which the logical window is mapped onto the physical window
    short int       wsx;                // logical window horizontal size
    short int       wsy;                // logical window vertical size
    short int       gsx;                // tile horizontal size
    short int       gsy;                // tile vertical size
    cmo             cm;
    cso             cs[charset_count];  // tile set array
    cbo             cb;
    pbo             pb;
    short int       cc;                 // colour count
    bool            wsa:1;              // window size is automatically adjusted to the size of the mapping (default == true)
  };

  /* sp_base_t
     surface partition
  */
  struct sp_base_t
  {
  };

  private:
  /* de_t
     device environment: backup struct for context switches
  */
  struct de
  {
      device* restore_device;

      std::uint8_t*(*restore_cso_reserve)(int) noexcept;
      std::uint8_t*(*restore_cso_dispose)(std::uint8_t*, int) noexcept;
      std::uint8_t*(*restore_cmo_reserve)(int) noexcept;
      std::uint8_t*(*restore_cmo_dispose)(std::uint8_t*, int) noexcept;
      std::uint8_t*(*restore_cbo_reserve)(int) noexcept;
      std::uint8_t*(*restore_cbo_dispose)(std::uint8_t*, int) noexcept;
      std::uint8_t*(*restore_pbo_reserve)(int) noexcept;
      std::uint8_t*(*restore_pbo_dispose)(std::uint8_t*, int) noexcept;
  };

  protected:
  static  surface*        s_surface_ptr;
  static  mapping_base_t* s_surface_mapping;

  protected:
  static  bool      gfx_set_format(unsigned int, int, int, int) noexcept;
  static  void      gfx_set_option_flags(unsigned int) noexcept;
  static  void      gfx_set_render_flags(unsigned int) noexcept;
  static  bool      gfx_set_window_size(int, int) noexcept;

  // static  void  gfx_cmo_reset(unsigned int, int) noexcept;
  // static  bool  gfx_cmo_load(int, unsigned int) noexcept;
  // static  bool  gfx_cmo_load(const char*, unsigned int, int) noexcept;
  // static  void  gfx_cso_reset(int, unsigned int, int, int) noexcept;
  // static  bool  gfx_cso_load(int, const char*, unsigned int, int, int) noexcept;
  // static  bool  gfx_cbo_resize(int, int) noexcept;

  static  uint8_t*  gfx_get_lb_ptr() noexcept;
  static  uint8_t*  gfx_get_hb_ptr() noexcept;
  static  uint8_t*  gfx_get_xb0_ptr() noexcept;
  static  uint8_t*  gfx_get_xb1_ptr() noexcept;

  static  void      gfx_scroll_rel(int, int) noexcept;
  static  void      gfx_scroll_abs(int, int) noexcept;

  static  void      gfx_save(de&, device*) noexcept;
  static  void      gfx_restore(de&) noexcept;

  private:
  friend class  device;
  friend class  driver;

  public:
          dc() noexcept;
          dc(const dc&) noexcept = delete;
          dc(dc&&) noexcept = delete;
          ~dc();
          dc&  swap(dc&) = delete;
          dc& operator=(const dc& rhs) noexcept = delete;
          dc& operator=(dc&& rhs) noexcept = delete;
};

/*namespace gfx*/ }
#endif
