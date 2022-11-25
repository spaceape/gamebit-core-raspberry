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
#include "dc.h"
#include "config.h"
#include "device.h"

      gfx::device*        g_display_ptr;

namespace gfx {

      surface*            dc::gfx_surface_ptr;
      dc::mapping_base_t* dc::gfx_mapping_ptr;
  
      dc::dc() noexcept
{
}

      dc::~dc()
{
}

/* gfx_set_format()
   set the rendering properties for the current surface when called from surface::gfx_prepare();
*/
bool  dc::gfx_set_format(unsigned int format, int colour_count, int tile_size_x, int tile_size_y) noexcept
{
      if(gfx_mapping_ptr) {
          if((tile_size_x < glyph_sx_min) ||
              (tile_size_x > glyph_sx_max)) {
              return false;
          }
          if((tile_size_y < glyph_sy_min) ||
              (tile_size_y > glyph_sy_max)) {
              return false;
          }
          gfx_mapping_ptr->format = format;
          gfx_mapping_ptr->gsx = tile_size_x;
          gfx_mapping_ptr->gsy = tile_size_y;
          gfx_mapping_ptr->cc  = colour_count;
          return true;
      }
      return false;
}

/* gfx_set_option_flags()
   set the option flags for the current surface when called from surface::gfx_prepare();
*/
void  dc::gfx_set_option_flags(unsigned int flags) noexcept
{
      if(gfx_mapping_ptr) {
          gfx_mapping_ptr->option_flags = flags;
      }
}

/* gfx_set_render_flags()
   set the rendering flags for the current surface when called from surface::gfx_prepare();
*/
void  dc::gfx_set_render_flags(unsigned int flags) noexcept
{
      if(gfx_mapping_ptr) {
          gfx_mapping_ptr->render_flags = flags;
      }
}

/* gfx_set_window_size()
   set the logical size of the current surface; currently only the first call (during surface::gfx_prepare()) will succeed
*/
bool  dc::gfx_set_window_size(int sx, int sy) noexcept
{
      if(gfx_mapping_ptr) {
          if((gfx_mapping_ptr->wsx == 0) &&
              (gfx_mapping_ptr->wsy == 0)) {
              if((sx < 0) ||
                  (sx > std::numeric_limits<short int>::max())) {
                  return false;
              }
              if((sy < 0) ||
                  (sy > std::numeric_limits<short int>::max())) {
                  return false;
              }
              gfx_mapping_ptr->wsx = sx;
              gfx_mapping_ptr->wsy = sy;
              gfx_mapping_ptr->wsa = false;
              return true;
          }
      }
      return false;
}

/* gfx_get_lb_ptr()
   get a pointer to the 'low byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_lb_ptr() noexcept
{
      return g_display_ptr->get_lb_ptr(gfx_surface_ptr);
}

/* gfx_get_lb_ptr()
   get a pointer to the 'high byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_hb_ptr() noexcept
{
      return g_display_ptr->get_hb_ptr(gfx_surface_ptr);
}

/* gfx_get_xb0_ptr()
   get a pointer to the least significant 'extended byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_xb0_ptr() noexcept
{
      return g_display_ptr->get_xb0_ptr(gfx_surface_ptr);
}

/* gfx_get_xb0_ptr()
   get a pointer to the most significant 'extended byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_xb1_ptr() noexcept
{
      return g_display_ptr->get_xb1_ptr(gfx_surface_ptr);
}

void  dc::gfx_scroll_rel(int dx, int dy) noexcept
{
      return g_display_ptr->scroll_rel(gfx_surface_ptr, dx, dy);
}

void  dc::gfx_scroll_abs(int dx, int dy) noexcept
{
      return g_display_ptr->scroll_abs(gfx_surface_ptr, dx, dy);
}

/* gfx_push_device()
*/
void  dc::gfx_push_device(device* device, de& restore_cb) noexcept
{
      // store current context
      restore_cb.device_ptr  = g_display_ptr;
      restore_cb.cso_reserve = gfx_cso_reserve;
      restore_cb.cso_dispose = gfx_cso_dispose;
      restore_cb.cmo_reserve = gfx_cmo_reserve;
      restore_cb.cmo_dispose = gfx_cmo_dispose;
      restore_cb.cbo_reserve = gfx_cbo_reserve;
      restore_cb.cbo_dispose = gfx_cbo_dispose;
      restore_cb.pbo_reserve = gfx_pbo_reserve;
      restore_cb.pbo_dispose = gfx_pbo_dispose;

      // apply new context
      g_display_ptr = device;
}

void  dc::gfx_push_surface(surface* surface_ptr, surface*& restore_surface_ptr, mapping_base_t*& restore_mapping_ptr) noexcept
{
      restore_surface_ptr = gfx_surface_ptr;
      restore_mapping_ptr = gfx_mapping_ptr;
      gfx_surface_ptr = surface_ptr;
      if(gfx_surface_ptr->m_mid) {
          gfx_mapping_ptr = static_cast<mapping_base_t*>(surface_ptr->m_mid);
      } else
          gfx_mapping_ptr = nullptr;
}

void  dc::gfx_pop_surface(surface*& restore_surface_ptr, mapping_base_t*& restore_mapping_ptr) noexcept
{
      gfx_mapping_ptr = restore_mapping_ptr;
      gfx_surface_ptr = restore_surface_ptr;
}

/* gfx_pop_device()
*/
void  dc::gfx_pop_device(de& restore_cb) noexcept
{
      g_display_ptr        = restore_cb.device_ptr;
      gfx_cso_reserve = restore_cb.cso_reserve;
      gfx_cso_dispose = restore_cb.cso_dispose;
      gfx_cmo_reserve = restore_cb.cmo_reserve;
      gfx_cmo_dispose = restore_cb.cmo_dispose;
      gfx_cbo_reserve = restore_cb.cbo_reserve;
      gfx_cbo_dispose = restore_cb.cbo_dispose;
      gfx_pbo_reserve = restore_cb.pbo_reserve;
      gfx_pbo_dispose = restore_cb.pbo_dispose;
}
/*namespace gfx*/ }
