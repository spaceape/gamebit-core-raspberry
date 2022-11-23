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
#include <util.h>

namespace gfx {

      device::device() noexcept:
      surface(true)
{
}

      device::~device()
{
}

void  device::sdr_surface_push(surface* surface_ptr, surface*& save_surface, mapping_base_t*& save_mapping) noexcept
{
      save_surface = s_surface_ptr;
      save_mapping = s_surface_mapping;
      s_surface_ptr = surface_ptr;
      if(s_surface_ptr->m_mid) {
          s_surface_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
      } else
          s_surface_mapping = nullptr;
}

bool  device::sdr_surface_prepare(surface* surface_ptr, mapping_base_t* mapping_ptr)  noexcept
{
      bool  l_result;
      auto  l_save_surface = s_surface_ptr;
      auto  l_save_mapping = s_surface_mapping;
      sdr_surface_push(surface_ptr, l_save_surface, l_save_mapping);
      l_result = surface_ptr->gfx_prepare(this);
      sdr_surface_pop(surface_ptr, l_save_surface, l_save_mapping);
      return l_result;
}

void  device::sdr_surface_pop(surface* surface_ptr, surface*& restore_surface, mapping_base_t*& restore_mapping) noexcept
{
      s_surface_mapping = restore_mapping;
      s_surface_ptr     = restore_surface;
}

bool  device::sdr_surface_release(surface* surface_ptr) noexcept
{
      bool             l_result;
      surface*         l_save_surface;
      mapping_base_t*  l_save_mapping;
      sdr_surface_push(surface_ptr, l_save_surface, l_save_mapping);
      l_result = surface_ptr->gfx_release(this);
      sdr_surface_pop(surface_ptr, l_save_surface, l_save_mapping);
      return l_result;
}

auto  device::sdr_make_mapping(surface* surface_ptr, int px, int py, int sx, int sy) noexcept -> mapping_t*
{
      auto p_surface = surface_ptr;
      auto p_mapping = new(std::nothrow) mapping_t;
      if(p_mapping != nullptr) {
          // initialize the new mapping
          p_mapping->source = p_surface;
          p_mapping->format = fmt_mono_bcs;
          p_mapping->render_flags = surface::map_copy;
          p_mapping->option_flags = surface::opt_request_tile_graphics;
          p_mapping->px = 0;
          p_mapping->py = 0;
          p_mapping->sx = 0;
          p_mapping->sy = 0;
          p_mapping->dx = 0;
          p_mapping->dy = 0;
          p_mapping->wsx = 0;  // default to 'auto' - `gfx::set_window_size()` will change if explicitely called
          p_mapping->wsy = 0;  // default to 'auto' - `gfx::set_window_size()` will change if explicitely called
          p_mapping->gsx = 8;
          p_mapping->gsy = 8;
          p_mapping->cc = 256;
          p_mapping->wsa = true;
          p_mapping->valid_bit = false;
          p_mapping->ready_bit = false;
          if(bool
              l_prepare_success = sdr_surface_prepare(surface_ptr, p_mapping);
              l_prepare_success == true) {
              // unsigned int l_format       = p_mapping->format;
              // unsigned int l_render_flags = p_mapping->render_flags;
              unsigned int l_option_flags      = p_mapping->option_flags;
              if(l_option_flags & surface::opt_graphics_flags) {
                  if(l_option_flags & surface::opt_request_tile_graphics) {
                      if(l_option_flags & surface::opt_request_raster_graphics) {
                          p_mapping->cm = m_dev_cmo;
                      }
                      p_mapping->cs[0] = m_dev_cso;
                  }
                  if(l_option_flags & surface::opt_request_raster_graphics) {
                  }
                  if(l_option_flags & surface::opt_request_vector_graphics) {
                  }
                  if(bool
                      l_remap_success = sdr_reset_mapping(p_surface, p_mapping, px, py, sx, sy);
                      l_remap_success == true) {
                      p_mapping->ready_bit = true;
                  }
              }
          }
      }
      return p_mapping;
}

bool  device::sdr_reset_mapping(surface* surface_ptr, mapping_t* mapping_ptr, int px, int py, int sx, int sy) noexcept
{
      mapping_ptr->px = px;
      mapping_ptr->py = py;
      mapping_ptr->sx = sx;
      mapping_ptr->sy = sy;
      mapping_ptr->valid_bit = false;
      if((mapping_ptr->sx > 0) &&
          (mapping_ptr->sy > 0)) {
          bool l_stored_wsx = mapping_ptr->wsx;
          bool l_stored_wsy = mapping_ptr->wsy;
          bool l_cbo_success = true;
          bool l_pbo_success = true;
          // update the 'window size' parameters, as follows:
          // - if `wsa` is set - window size should always equal the exposed size
          // - if `wsa` is not set and window size is smaller than the exposed size - adjust the window size to match
          if(mapping_ptr->wsa) {
              mapping_ptr->wsx = sx;
              mapping_ptr->wsy = sy;
          } else
          if((mapping_ptr->wsx < mapping_ptr->sx) &&
              (mapping_ptr->wsy < mapping_ptr->sy)) {
              mapping_ptr->wsx = sx;
              mapping_ptr->wsy = sy;
          }
          // update the data objects to match the window size parameters, if those have been updated
          if((mapping_ptr->wsx != l_stored_wsx) &&
              (mapping_ptr->wsy != l_stored_wsy)) {
              unsigned int l_format       = mapping_ptr->format;
              unsigned int l_render_flags = mapping_ptr->render_flags;
              unsigned int l_option_flags = mapping_ptr->option_flags;
              if(l_option_flags & surface::opt_request_tile_graphics) {
                  int   l_csx = get_div_ub(mapping_ptr->wsx, mapping_ptr->gsx);
                  int   l_csy = get_div_ub(mapping_ptr->wsy, mapping_ptr->gsy);
                  auto& l_cbo = mapping_ptr->cb;
                  if(l_cbo_success = l_cbo.reset(l_format, l_csx, l_csy);
                      l_cbo_success == true) {
                      //... fill with preset data
                  }
              }
              if(l_option_flags & surface::opt_request_raster_graphics) {
                  auto& l_pbo = mapping_ptr->pb;
                  if(l_pbo_success = l_pbo.reset(l_format, l_pbo, l_pbo);
                      l_pbo_success == true) {
                      //... fill with preset data
                  }
              }
              if(l_option_flags & surface::opt_request_vector_graphics) {
              }
          }
          // update the limits on the horizontal scrolling parameter
          if(mapping_ptr->dx + mapping_ptr->sx < mapping_ptr->wsx) {
              mapping_ptr->dx = mapping_ptr->wsx - mapping_ptr->sx;
          }
          // update the limits on the vertical scrolling parameter
          if(mapping_ptr->dy + mapping_ptr->sy < mapping_ptr->wsy) {
              mapping_ptr->dy = mapping_ptr->wsy - mapping_ptr->sy;
          }
          mapping_ptr->valid_bit = l_cbo_success && l_pbo_success;
      }
      return mapping_ptr->valid_bit;
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
      surface*        l_save_surface;
      mapping_base_t* l_save_mapping;
      if(dt > 0) {
          i_surface = m_attach_head;
          while(i_surface) {
              sdr_surface_push(i_surface, l_save_surface, l_save_mapping);
              gfx_render();
              sdr_surface_pop(i_surface, l_save_surface, l_save_mapping);
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
          if(surface_ptr->m_manager == this) {
              p_mapping = static_cast<mapping_t*>(surface_ptr->m_mid);
              if(p_mapping != nullptr) {
                  return sdr_reset_mapping(surface_ptr, p_mapping, px, py, sx, sy);
              } else
              if(p_mapping = sdr_make_mapping(surface_ptr, px, py, sx, sy); p_mapping != nullptr) {
                  surface_ptr->m_mid = p_mapping;
                  return true;
              }
          }
      }
      return false;
}

bool  device::set_format(surface*, unsigned int) noexcept
{
      return false;
}

bool  device::set_tile_options(surface*, int, int) noexcept
{
      return false;
}

bool  device::set_raster_options(surface*, int, int) noexcept
{
      return false;
}

bool  device::set_vector_options(surface*, int, int) noexcept
{
      return false;
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
