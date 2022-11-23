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

namespace gfx {

      device*             dc::s_device;
      surface*            dc::s_surface_ptr;
      dc::mapping_base_t* dc::s_surface_mapping;
  
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
      if((tile_size_x < glyph_sx_min) ||
          (tile_size_x > glyph_sx_max)) {
          return false;
      }
      if((tile_size_y < glyph_sy_min) ||
          (tile_size_y > glyph_sy_max)) {
          return false;
      }
      s_surface_mapping->format = format;
      s_surface_mapping->gsx = tile_size_x;
      s_surface_mapping->gsy = tile_size_y;
      s_surface_mapping->cc  = colour_count;
      return true;
}

/* gfx_set_option_flags()
   set the option flags for the current surface when called from surface::gfx_prepare();
*/
void  dc::gfx_set_option_flags(unsigned int flags) noexcept
{
      s_surface_mapping->option_flags = flags;
}

/* gfx_set_render_flags()
   set the rendering flags for the current surface when called from surface::gfx_prepare();
*/
void  dc::gfx_set_render_flags(unsigned int flags) noexcept
{
      s_surface_mapping->render_flags = flags;
}

/* gfx_set_window_size()
   set the logical size of the current surface; currently only the first call (during surface::gfx_prepare()) will succeed
*/
bool  dc::gfx_set_window_size(int sx, int sy) noexcept
{
      if((s_surface_mapping->wsx == 0) &&
          (s_surface_mapping->wsy == 0)) {
          if((sx < 0) ||
              (sx > std::numeric_limits<short int>::max())) {
              return false;
          }
          if((sy < 0) ||
              (sy > std::numeric_limits<short int>::max())) {
              return false;
          }
          s_surface_mapping->wsx = sx;
          s_surface_mapping->wsy = sy;
          s_surface_mapping->wsa = false;
          return true;
      }
      return false;
}

void  dc::gfx_scroll_rel(int dx, int dy) noexcept
{
      gfx_scroll_abs(s_surface_mapping->dx + dx, s_surface_mapping->dy + dy);
}

void  dc::gfx_scroll_abs(int dx, int dy) noexcept
{
      s_surface_mapping->dx = dx;
      s_surface_mapping->dy = dy;
}

// /* gfx_cmo_reset()
// */
// void  dc::gfx_cmo_reset(unsigned int format, int colour_count) noexcept
// {
//       auto& l_cmo = s_surface_mapping->cm;
//       l_cmo.reset(format, colour_count);
// }

// /* gfx_cmo_load()
// */
// bool  dc::gfx_cmo_load(int preset, unsigned int format) noexcept
// {
//       auto& l_cmo = s_surface_mapping->cm;
//       return gfx_load_cmo_preset(l_cmo, preset, format);
// }

// /* gfx_cmo_load()
// */
// bool  dc::gfx_cmo_load(const char* file_name, unsigned int format, int colour_count) noexcept
// {
//       auto& l_cmo = s_surface_mapping->cm;
//       return gfx_load_cmo(l_cmo, file_name, format, colour_count);
// }

// /* gfx_cso_reset()
// */
// void  dc::gfx_cso_reset(int index, unsigned int format, int sx, int sy) noexcept
// {
//       auto& l_cso = s_surface_mapping->cs[index];
//       l_cso.reset(format, sx, sy);
// }

// /* gfx_cso_load()
// */
// bool  dc::gfx_cso_load(int index, const char* file_name, unsigned int format, int sx, int sy) noexcept
// {
//       auto& l_cso = s_surface_mapping->cs[index];
//       return gfx_load_cso(l_cso, file_name, format, sx, sy);
// }

// bool  dc::gfx_cbo_resize(int sx, int sy) noexcept
// {
//       auto& l_cbo = s_surface_mapping->cb;
//       return l_cbo.reset(s_surface_mapping->format, sx, sy);
// }

/* gfx_get_lb_ptr()
   get a pointer to the 'low byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_lb_ptr() noexcept
{
      return s_surface_mapping->cb.get_lb_ptr();
}

/* gfx_get_lb_ptr()
   get a pointer to the 'high byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_hb_ptr() noexcept
{
      return s_surface_mapping->cb.get_hb_ptr();
}

/* gfx_get_xb0_ptr()
   get a pointer to the least significant 'extended byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_xb0_ptr() noexcept
{
      return s_surface_mapping->cb.get_xb0_ptr();
}

/* gfx_get_xb0_ptr()
   get a pointer to the most significant 'extended byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_xb1_ptr() noexcept
{
      return s_surface_mapping->cb.get_xb1_ptr();
}

/* gfx_save()
   pack the current context variables to a `de` object
*/
void  dc::gfx_save(de& ce, device* device) noexcept
{
      // store current context
      ce.restore_device      = s_device;
      ce.restore_cso_reserve = gfx_cso_reserve;
      ce.restore_cso_dispose = gfx_cso_dispose;
      ce.restore_cmo_reserve = gfx_cmo_reserve;
      ce.restore_cmo_dispose = gfx_cmo_dispose;
      ce.restore_cbo_reserve = gfx_cbo_reserve;
      ce.restore_cbo_dispose = gfx_cbo_dispose;
      ce.restore_pbo_reserve = gfx_pbo_reserve;
      ce.restore_pbo_dispose = gfx_pbo_dispose;

      // apply new context
      s_device = device;
}

void  dc::gfx_restore(de& ce) noexcept
{
      s_device        = ce.restore_device;
      gfx_cso_reserve = ce.restore_cso_reserve;
      gfx_cso_dispose = ce.restore_cso_dispose;
      gfx_cmo_reserve = ce.restore_cmo_reserve;
      gfx_cmo_dispose = ce.restore_cmo_dispose;
      gfx_cbo_reserve = ce.restore_cbo_reserve;
      gfx_cbo_dispose = ce.restore_cbo_dispose;
      gfx_pbo_reserve = ce.restore_pbo_reserve;
      gfx_pbo_dispose = ce.restore_pbo_dispose;
}

/*namespace gfx*/ }
