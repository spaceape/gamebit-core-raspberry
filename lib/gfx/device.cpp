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
#include "device.h"
#include "tile.h"
#include "raster.h"
#include "vector.h"
#include "driver/hmm.h"
#include <util.h>

namespace gfx {

      device::device() noexcept:
      surface(true)
{
}

      device::~device()
{
}

bool  device::sdr_surface_prepare(surface* surface_ptr, mapping_base_t* mapping_ptr)  noexcept
{
      bool            l_result = true;
      surface*        p_restore_surface;
      mapping_base_t* p_restore_mapping;
      gfx_push_surface(surface_ptr, p_restore_surface, p_restore_mapping);
      // l_result = surface_ptr->gfx_prepare(this);
      gfx_pop_surface(p_restore_surface, p_restore_mapping);
      return l_result;
}

bool  device::sdr_surface_release(surface* surface_ptr) noexcept
{
      bool            l_result;
      surface*        p_restore_surface;
      mapping_base_t* p_restore_mapping;
      gfx_push_surface(surface_ptr, p_restore_surface, p_restore_mapping);
      l_result = surface_ptr->gfx_release(this);
      gfx_pop_surface(p_restore_surface, p_restore_mapping);
      return l_result;
}

auto  device::sdr_make_mapping(surface* surface_ptr, int px, int py, int sx, int sy) noexcept -> mapping_t*
{
      auto p_surface = surface_ptr;
      auto p_mapping = new(std::nothrow) mapping_t;
      if(p_mapping != nullptr) {
          // initialize the new mapping
          p_mapping->source = p_surface;
          p_mapping->format = fmt_mono_bcs | m_dev_cmo.get_colour_count();
          p_mapping->render_flags = surface::map_copy;
          p_mapping->option_flags = surface::opt_request_tile_graphics;
          p_mapping->px = px;
          p_mapping->py = py;
          p_mapping->sx = sx;
          p_mapping->sy = sy;
          p_mapping->dx = 0;
          p_mapping->dy = 0;
          p_mapping->wsx = 0;  // default to 'auto' - `gfx::set_window_size()` will change if explicitely called
          p_mapping->wsy = 0;  // default to 'auto' - `gfx::set_window_size()` will change if explicitely called
          p_mapping->gsx = 8;
          p_mapping->gsy = 8;
          p_mapping->cc = m_dev_cmo.get_colour_count();
          p_mapping->wsa = true;
          p_mapping->valid_bit = false;
          p_mapping->ready_bit = false;
          if(bool
              l_prepare_success = sdr_surface_prepare(surface_ptr, p_mapping);
              l_prepare_success == true) {
              if(p_mapping->option_flags & surface::opt_graphics_flags) {
                  p_mapping->cm = m_dev_cmo;
                  if(p_mapping->option_flags & surface::opt_request_tile_graphics) {
                      p_mapping->cs[0] = m_dev_cso;
                  }
                  if(bool
                      l_remap_success = sdr_reset_mapping(p_surface, p_mapping);
                      l_remap_success == true) {
                      p_mapping->ready_bit = true;
                  }
              }
          }
      }
      return p_mapping;
}

bool  device::sdr_reset_mapping(surface* surface_ptr, mapping_t* mapping_ptr) noexcept
{
      mapping_ptr->valid_bit =
          (mapping_ptr->sx >= 0) &&
          (mapping_ptr->sy >= 0);
      mapping_ptr->ready_bit = 
          (mapping_ptr->valid_bit == true) &&
          (mapping_ptr->sx > 0) &&
          (mapping_ptr->sy > 0);
      return mapping_ptr->valid_bit;
}

bool  device::sdr_reset_format(surface* surface_ptr, mapping_t* mapping_ptr, unsigned int format, int cc, int glyph_sx, int glyph_sy) noexcept
{
      auto&  l_cbo    = mapping_ptr->cb;
      auto&  l_pbo    = mapping_ptr->pb;
      auto&  l_cmo    = mapping_ptr->cm;
      bool   l_cbo_rq = false;
      bool   l_cbo_up = false;
      bool   l_pbo_rq = false;
      bool   l_pbo_up = false;
      bool   l_cmo_rq = false;
      bool   l_cmo_up = false;
      // additive setup
      if(mapping_ptr->option_flags & opt_request_tile_graphics) {
          int  l_csx = get_div_ub(mapping_ptr->wsx, glyph_sx);
          int  l_csy = get_div_ub(mapping_ptr->wsy, glyph_sy);
          if(bool
              l_cbo_success = l_cbo.reset(format, l_csx, l_csy);
              l_cbo_success == true) {
              l_cbo_up = true;
          }
          l_cbo_rq = true;
      }
      if(mapping_ptr->option_flags & opt_request_raster_graphics) {
          if(bool
              l_pbo_success = l_pbo.reset(fmt_indexed, mapping_ptr->wsx, mapping_ptr->wsy);
              l_pbo_success == true) {
              l_pbo_up = true;
          }
          l_pbo_rq = true;
      }
      // if the current colour map is empty and the requested format and number of colours matches the device palette,
      // then set the device palette;
      if(l_cmo.has_colour_count(0)) {
          if(m_dev_cmo.has_format(format)) {
              if(m_dev_cmo.has_colour_count(cc)) {
                  l_cmo    = m_dev_cmo;
                  l_cmo_up = true;
              }
          }
          l_cmo_rq = true;
      }
      // ...otherwise reset the surface palette to the specified colour parameters
      if(l_cmo_rq == true) {
          if(l_cmo_up == false) {
              l_cmo_up = l_cmo.reset(format, cc);
          }
      }
      mapping_ptr->format = format;
      mapping_ptr->gsx = glyph_sx;
      mapping_ptr->gsy = glyph_sy;
      mapping_ptr->cc = cc;
      if((l_cbo_rq == l_cbo_up) &&
          (l_pbo_rq == l_pbo_up) &&
          (l_cmo_rq == l_cmo_up)) {
          return sdr_reset_mapping(surface_ptr, mapping_ptr);
      }
      return false;
}

bool  device::sdr_reset_geometry(surface* surface_ptr, mapping_t* mapping_ptr, int px, int py, int sx, int sy) noexcept
{
      mapping_ptr->px = px;
      mapping_ptr->py = py;
      // set surface horizontal size
      if(mapping_ptr->px + sx < 0) {
          mapping_ptr->sx = 0;
      } else
      if(mapping_ptr->px + sx > get_display_sx()) {
          mapping_ptr->sx = get_display_sx() - mapping_ptr->px;
      } else
          mapping_ptr->sx = sx;
      // set surface vertical size
      if(mapping_ptr->py + sy < 0) {
          mapping_ptr->sy = 0;
      } else
      if(mapping_ptr->py + sy > get_display_sy()) {
          mapping_ptr->sy = get_display_sy() - mapping_ptr->py;
      } else
          mapping_ptr->sy = sy;

      if((mapping_ptr->sx > 0) &&
          (mapping_ptr->sy > 0)) {
          // if `wsa` is set - adjust window size to match the mapped size
          if(mapping_ptr->wsa) {
              sdr_reset_window_size(surface_ptr, mapping_ptr, mapping_ptr->sx, mapping_ptr->sy);
          } else
          if((mapping_ptr->wsx < mapping_ptr->sx) &&
              (mapping_ptr->wsy < mapping_ptr->sy)) {
              sdr_reset_window_size(surface_ptr, mapping_ptr, mapping_ptr->sx, mapping_ptr->sy);
          }
      }
      return sdr_reset_mapping(surface_ptr, mapping_ptr);
}

void  device::sdr_reset_window_size(surface* surface_ptr, mapping_t* mapping_ptr, int sx, int sy) noexcept
{
      mapping_ptr->wsx = sx;
      mapping_ptr->wsy = sy;
      if((mapping_ptr->wsx < mapping_ptr->sx) ||
          (mapping_ptr->wsx < mapping_ptr->sy)) {
          sdr_reset_format(surface_ptr, mapping_ptr, mapping_ptr->format, mapping_ptr->cc, mapping_ptr->sx, mapping_ptr->sy);
      }
}

void  device::sdr_reset_option_flags(surface* surface_ptr, mapping_t* mapping_ptr, unsigned int option_flags) noexcept
{
      mapping_ptr->option_flags = option_flags;
      sdr_reset_mapping(surface_ptr, mapping_ptr);
}

void  device::sdr_reset_render_flags(surface* surface_ptr, mapping_t* mapping_ptr, unsigned int render_flags) noexcept
{
      mapping_ptr->render_flags = render_flags;
      sdr_reset_mapping(surface_ptr, mapping_ptr);
}

auto  device::sdr_free_mapping(surface* surface_ptr, mapping_t* mapping_ptr) noexcept -> mapping_t*
{
      surface*    p_surface = surface_ptr;
      mapping_t*  p_mapping = static_cast<mapping_t*>(mapping_ptr);
      if(p_mapping != nullptr) {
          if(p_mapping->render_flags != map_none) {
              sdr_surface_release(p_surface);
          }
          delete p_mapping;
      }
      return nullptr;
}

bool  device::gfx_attach_surface(surface*) noexcept
{
      return true;
}

void  device::gfx_render() noexcept
{
}

bool  device::gfx_detach_surface(surface* surface_ptr) noexcept
{
      unmap(surface_ptr);
      return true;
}

void  device::gfx_sync(int dt) noexcept
{
      surface*        i_surface;
      surface*        p_restore_surface;
      mapping_base_t* p_restore_mapping;
      if(dt > 0) {
          i_surface = m_attach_head;
          while(i_surface) {
              gfx_push_surface(i_surface, p_restore_surface, p_restore_mapping);
              gfx_render();
              gfx_pop_surface(p_restore_surface, p_restore_mapping);
              i_surface = i_surface->m_attach_next;
          }
      }
}

bool  device::map(surface* surface_ptr, int px, int py, int sx, int sy) noexcept
{
      return remap(surface_ptr, px, py, sx, sy);
}

bool  device::remap(surface* surface_ptr, int px, int py, int sx, int sy) noexcept
{
      mapping_t* p_mapping;
      if((surface_ptr != this) &&
          (surface_ptr != nullptr)) {
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          if(p_mapping != nullptr) {
              return sdr_reset_geometry(surface_ptr, p_mapping, px, py, sx, sy);
          } else
          if(p_mapping = sdr_make_mapping(surface_ptr, px, py, sx, sy); p_mapping != nullptr) {
              surface_ptr->m_mid = p_mapping;
              return true;
          }
      }
      return false;
}

/* set_format()
   reset surface format, colours and glyph size;
*/
bool  device::set_format(surface* surface_ptr, unsigned int format, int cc, int glyph_sx, int glyph_sy) noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          return sdr_reset_format(surface_ptr, p_mapping, format, cc, glyph_sx, glyph_sy);
      }
      return false;
}

/* set_option_flags()
   reset surface option flags
*/
void  device::set_option_flags(surface* surface_ptr, unsigned int option_flags) noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          sdr_reset_option_flags(surface_ptr, p_mapping, option_flags);
      }
}

/* set_render_flags()
   reset surface rendering flags
*/
void  device::set_render_flags(surface* surface_ptr, unsigned int render_flags) noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          sdr_reset_render_flags(surface_ptr, p_mapping, render_flags);
      }
}

void  device::set_window_size(surface* surface_ptr, int sx, int sy) noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          sdr_reset_window_size(surface_ptr, p_mapping, sx, sy);
      }
}

std::uint8_t* device::get_lb_ptr(surface* surface_ptr) const noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          return p_mapping->cb.get_lb_ptr();
      }
      return nullptr;
}

std::uint8_t* device::get_hb_ptr(surface* surface_ptr) const noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          return p_mapping->cb.get_lb_ptr();
      }
      return nullptr;
}

std::uint8_t* device::get_xb0_ptr(surface* surface_ptr) const noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          return p_mapping->cb.get_lb_ptr();
      }
      return nullptr;
}

std::uint8_t* device::get_xb1_ptr(surface* surface_ptr) const noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          return p_mapping->cb.get_lb_ptr();
      }
      return nullptr;
}

void  device::scroll_rel(surface* surface_ptr, int dx, int dy) noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          // update the limits on the horizontal scrolling parameter
          p_mapping->dx += dx;
          if(p_mapping->dx + p_mapping->sx < p_mapping->wsx) {
              p_mapping->dx = p_mapping->wsx - p_mapping->sx;
          }
          // update the limits on the vertical scrolling parameter
          p_mapping->dy += dy;
          if(p_mapping->dy + p_mapping->sy < p_mapping->wsy) {
              p_mapping->dy = p_mapping->wsy - p_mapping->sy;
          }
      }
}

void  device::scroll_abs(surface* surface_ptr, int dx, int dy) noexcept
{
      if(mapping_t*
          p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          // update the limits on the horizontal scrolling parameter
          p_mapping->dx = dx;
          if(p_mapping->dx + p_mapping->sx < p_mapping->wsx) {
              p_mapping->dx = p_mapping->wsx - p_mapping->sx;
          }
          // update the limits on the vertical scrolling parameter
          p_mapping->dy = dy;
          if(p_mapping->dy + p_mapping->sy < p_mapping->wsy) {
              p_mapping->dy = p_mapping->wsy - p_mapping->sy;
          }
      }
}

bool  device::unmap(surface* surface_ptr) noexcept
{
      mapping_t* p_mapping;
      if((surface_ptr != this) &&
          (surface_ptr != nullptr)) {
          if(surface_ptr->m_manager == this) {
              p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
              if(p_mapping != nullptr) {
                  surface_ptr->m_mid = sdr_free_mapping(surface_ptr, p_mapping);
                  return true;
              }
          }
      }
      return false;
}

/*namespace gfx*/ }
