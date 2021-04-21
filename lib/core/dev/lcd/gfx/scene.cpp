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
#include "scene.h"
#include "canvas.h"
#include "sprite.h"
#include <pico/multicore.h>
#include <cstring>

      constexpr int   s_canvas_max   = 8;
      constexpr int   s_sprite_max   = 24;

/* s_frame_redraw
   force redraw every frame: good for palette animations, bad for battery life
*/
      constexpr bool  s_frame_redraw = true;

namespace dev {
namespace gfx {

      static scene*   s_scene;

      scene::scene(int sx, int sy) noexcept:
      dc(),
      m_viewport_geometry(
        static_cast<std::int16_t>(0 - sx / 2),
        static_cast<std::int16_t>(0 - sy / 2),
        static_cast<std::int16_t>(0 + sx / 2),
        static_cast<std::int16_t>(0 + sy / 2)
      ),
      m_background_colour(1),
      m_canvas_count(0),
      m_sprite_count(0),
      m_screen_ptr(nullptr),
      m_screen_size(0),
      m_surface_head(nullptr),
      m_surface_tail(nullptr),
      m_geometry_dirty(true),
      m_graphics_dirty(true),
      m_running(false)
{
      m_draw_list.reserve((s_canvas_max + s_sprite_max) * 4);
}

      scene::~scene()
{
      surface* l_surface_iter = m_surface_head;
      surface* l_surface_next;
      suspend();
      while(l_surface_iter) {
          l_surface_next = l_surface_iter->p_next;
          delete l_surface_next;
          l_surface_iter = l_surface_next;
      }
}

/* dwl_make_colour
   add a full screen fill node to the draw list;
   completely clears the list as well, as it will obscure everything behind it
*/
void  scene::dwl_make_colour(std::uint16_t colour) noexcept
{
      m_draw_list.clear();
      draw_node_t& l_node = m_draw_list.emplace_back();
      l_node.m_type     = surface::type_colour;
      l_node.m_fill     = colour;
      l_node.m_target_geometry = box2d_t(0, 0, m_viewport_geometry.get_sx(), m_viewport_geometry.get_sy());
}

/* dwl_make_canvas
   add a canvas node to the draw list;
   this is meant to perform quad tree draw optimisation - rendering only the canvas regions that are not obscured by other
   opaque canvases;
   NOTE: I have the code for that in the vault somewhere from a previous design, but I will not bother to port it now, as
   this feature is only effective against somewhat pathological client graphics 
*/
void  scene::dwl_make_canvas(canvas* surface, box2d_t& source, box2d_t& target) noexcept
{
      auto&  l_node = m_draw_list.emplace_back();
      l_node.m_type = surface::type_canvas;
      l_node.m_fill = 0;
      l_node.m_surface = surface;
      l_node.m_source_geometry = source;
      l_node.m_target_geometry = target;
}

/* dwl_make_sprite
   add a sprite node to the draw list
*/
void  scene::dwl_make_sprite(sprite* surface, gfx::box2d_t& source, gfx::box2d_t& target) noexcept
{
      auto&  l_node = m_draw_list.emplace_back();
      l_node.m_type = surface::type_sprite;
      l_node.m_fill = 0;
      l_node.m_surface = surface;
      l_node.m_source_geometry = source;
      l_node.m_target_geometry = target;
}

bool  scene::gfx_update_geometry() noexcept
{
      surface* l_surface_iter;
      dwl_make_colour(m_background_colour);
      if(m_surface_head) {
          l_surface_iter = m_surface_head;
          while(l_surface_iter) {
              int     l_surface_type     = l_surface_iter->m_type;
              box2d_t l_surface_geometry = l_surface_iter->get_geometry();
              box2d_t l_source_geometry;
              box2d_t l_target_geometry;
              box2d_t l_compare_geometry;

              // compute intersection with the viewport
              l_surface_geometry.compare(m_viewport_geometry, l_compare_geometry);

              // compute source geometry: the region of the surface to be rendered
              if(l_compare_geometry.get_px0() < 0) {
                  l_source_geometry.set_px0(0 - l_compare_geometry.get_px0());
              }
              if(l_compare_geometry.get_py0() < 0) {
                  l_source_geometry.set_py0(0 - l_compare_geometry.get_py0());
              }
              if(l_compare_geometry.get_px1() < 0) {
                  l_source_geometry.set_px1(l_surface_geometry.get_px1() - l_surface_geometry.get_px0());
              } else
                  l_source_geometry.set_px1(l_surface_geometry.get_px1() - l_surface_geometry.get_px0() + l_compare_geometry.get_px1());

              if(l_compare_geometry.get_py1() < 0) {
                  l_source_geometry.set_py1(l_surface_geometry.get_py1() - l_surface_geometry.get_py0());
              } else
                  l_source_geometry.set_py1(l_surface_geometry.get_py1() - l_surface_geometry.get_py0() + l_compare_geometry.get_py1());

              if((l_source_geometry.get_px0() < l_source_geometry.get_px1()) &&
                  (l_source_geometry.get_py0() < l_source_geometry.get_py1())) {

                  // compute target geometry: screen coordinates where to render the surface
                  if(l_compare_geometry.get_px0() > 0) {
                      l_target_geometry.set_px0(l_surface_geometry.get_px0() - m_viewport_geometry.get_px0());
                  } else
                      l_target_geometry.set_px0(0);

                  if(l_compare_geometry.get_py0() > 0) {
                      l_target_geometry.set_py0(l_surface_geometry.get_py0() - m_viewport_geometry.get_py0());
                  } else
                      l_target_geometry.set_py0(0);

                  if(l_compare_geometry.get_px1() < 0) {
                      l_target_geometry.set_px1(l_surface_geometry.get_px1() - m_viewport_geometry.get_px0());
                  } else
                      l_target_geometry.set_px1(l_surface_geometry.get_px1() - m_viewport_geometry.get_px0() - l_compare_geometry.get_px1());

                  if(l_compare_geometry.get_py1() < 0) {
                      l_target_geometry.set_py1(l_surface_geometry.get_py1() - m_viewport_geometry.get_py0());
                  } else
                      l_target_geometry.set_py0(l_surface_geometry.get_py1() - m_viewport_geometry.get_py0() - l_compare_geometry.get_py1());

                  // generate draw nodes
                  if(l_surface_type == surface::type_canvas) {
                      dwl_make_canvas(static_cast<canvas*>(l_surface_iter), l_source_geometry, l_target_geometry);
                  } else
                  if(l_surface_type == surface::type_sprite) {
                      dwl_make_sprite(static_cast<sprite*>(l_surface_iter), l_source_geometry, l_target_geometry);
                  }
              }
              l_surface_iter = l_surface_iter->p_next;
          }
      }
      return true;
}


void  scene::gfx_update_graphics() noexcept
{
      for(auto l_draw_iter = m_draw_list.begin(); l_draw_iter != m_draw_list.end(); l_draw_iter++) {
          if(l_draw_iter->m_type == surface::type_colour) {
              auto& l_target = l_draw_iter->m_target_geometry;
              auto  l_py0    = m_screen_ptr + l_target.get_px() + l_target.get_py() * m_viewport_geometry.get_sx();
              auto  l_py1    = l_py0;
              auto  l_fill   = l_draw_iter->m_fill;
              int   l_width  = l_target.get_sx();
              while(l_py0  < l_py1) {
                  std::memset(l_py0, l_fill, l_width);
                  l_py0 += m_viewport_geometry.get_sx();
              }
          } else
          if(l_draw_iter->m_type == surface::type_canvas) {
              // if(l_draw_iter->m_surface->has_flags(surface::sf_visible)) {
              //     auto& l_canvas = *static_cast<canvas*>(l_draw_iter->m_surface);
              //     auto& l_target = l_draw_iter->m_target_geometry;
              //     auto& l_source = l_draw_iter->m_source_geometry;
              //     auto  l_char_buffer = l_canvas.get_data_ptr();
              //     bool  l_opaque = l_draw_iter->m_surface->has_attributes(surface::sa_opaque);

              //     // compute line count
              //     int   l_lc  = l_target.get_height() / s_charset_sy;
              //     // compute column count
              //     int   l_cc  = l_target.get_width() / s_charset_sx;
              //     // compute how many lines we can draw at once using the preallocated cache_ptr buffer
              //     int   l_max = m_cache_size / (l_cc * s_charset_sx * s_charset_sy * sizeof(std::uint16_t));

              //     int   l_li  = 0;                       // line pixel iterator
              //     std::uint16_t* l_lp = m_cache_ptr;
              //     int   l_ci;                           // column pixel iterator
              //     std::uint16_t* l_cp;

              //     int   l_cy  = l_canvas.get_line_size() * (l_source.get_py0() / s_charset_sy);   // line character iterator
              //     int   l_cx;

              //     // make sure we flush the buffer at the end of the line loop
              //     if(l_max > l_lc) {
              //         l_max = l_lc;
              //     }
              //     // render the text buffer: iterate over each line and column of the text window and render
              //     while(l_li < l_lc) {
              //         l_ci = 0;
              //         l_cx = l_cy + l_source.get_px0() / s_charset_sx;
              //         l_cp = l_lp;
              //         while(l_ci < l_cc) {
              //             std::uint8_t*  l_lb          = l_canvas.get_lb_ptr(l_cx);
              //             std::uint8_t*  l_hb          = l_canvas.get_hb_ptr(l_cx);
              //             std::uint8_t*  l_fx          = l_canvas.get_fx_ptr(l_cx);
              //             std::uint16_t* l_pp          = l_cp; // target pixel pointer
              //             std::uint8_t*  l_char_ptr;
              //             std::uint16_t  l_char_index  =*l_lb;
              //             std::uint16_t  l_char_fg     = 0xffff;
              //             std::uint16_t  l_char_bg     = 0x0000;
              //             if(l_canvas.has_format(mode_hb)) {
              //                 l_char_index |=(*l_hb & hbi_char_bits) << 8;
              //             }
              //             l_char_ptr = g_charmap + (l_char_index * s_charset_sy);
              //             if(l_canvas.has_format(mode_fg)) {
              //             }
              //             if(l_canvas.has_format(mode_bg) && l_opaque) {
              //             }
              //             for(int l_py = 0; l_py < s_charset_sy; l_py++) {
              //                 int l_mx = 1;
              //                 for(int l_px = 0; l_px < s_charset_sx; l_px++) {
              //                     if(l_char_ptr[l_py] & l_mx) {
              //                         *l_pp = l_char_fg;
              //                     } else
              //                         *l_pp = l_char_bg;
              //                     l_mx = l_mx << 1;
              //                     l_pp++;
              //                 }
              //                 l_pp += l_target.get_width() - s_charset_sx - 2;
              //             }
              //             // for(int l_py = 0; l_py < s_charset_sy; l_py++) {
              //             //     for(int l_px = 0; l_px < s_charset_sx; l_px++) {
              //             //         if(l_px == 4) {
              //             //             *l_pp = l_char_fg;
              //             //         } else
              //             //             *l_pp = l_char_bg;
              //             //         l_pp++;
              //             //     }
              //             //     l_pp += l_target.get_width() - s_charset_sx - 2;
              //             // }
              //             l_cx++;
              //             l_cp += s_charset_sx;
              //             l_ci++;
              //         }
              //         l_cy += l_canvas.get_line_size();
              //         l_lp += l_cc * s_charset_sx * s_charset_sy;
              //         l_li++;
              //         if(l_li == l_max) {
              //             g_driver->fill(
              //                 l_target.get_px0(),
              //                 l_target.get_py0(),
              //                 l_target.get_px1(),
              //                 l_target.get_py1(),
              //                 m_cache_ptr
              //             );
              //             l_lp = m_cache_ptr;
              //         }
              //     }
              // }
          } else
          if(l_draw_iter->m_type == surface::type_sprite) {
          // not yet sure what I wand a sprite to be exactly
          }
      }
      gfx_flush();
}

void  scene::gfx_flush()
{
      g_driver->fill(0, 0, 320, 240, m_screen_ptr);
}

void  scene::gfx_free_surface(surface* object) noexcept
{
      if(object) {
          surface* l_surface_iter = m_surface_head;
          surface* l_surface_prev = nullptr;
          surface* l_surface_next;
          while(l_surface_iter) {
              l_surface_next = l_surface_iter->p_next;
              if(l_surface_iter == object) {
                  if(l_surface_prev != nullptr) {
                      l_surface_prev->p_next = l_surface_iter->p_next;
                  } else
                      m_surface_head = l_surface_iter->p_next;
                  if(l_surface_next == nullptr) {
                      m_surface_tail = l_surface_prev;
                  }
                  delete l_surface_next;
                  break;
              }
              l_surface_prev = l_surface_iter;
              l_surface_iter = l_surface_next;
          }
      }
}

void  scene::gfx_render_loop()
{
      scene& scene =*s_scene;
      while(scene.m_draw_loop.load()) {
          scene.m_draw_lock = true;
          scene.gfx_update_graphics();
          scene.m_draw_lock.store(false);
          while(scene.m_draw_wait.load()) {
              asm volatile ("nop\n");
              asm volatile ("nop\n");
              asm volatile ("nop\n");
              asm volatile ("nop\n");
          }
      }
}

bool  scene::set_viewport_position(std::int16_t, std::int16_t) noexcept
{
      return false;
}

bool  scene::set_viewport_size(std::uint16_t, std::uint16_t) noexcept
{
      return false;
}

bool  scene::set_viewport_geometry(std::int16_t, std::int16_t, std::int16_t, std::int16_t) noexcept
{
      return false;
}

canvas* scene::make_canvas(std::uint8_t mode, std::int16_t px, std::int16_t py, std::uint16_t sx, std::uint16_t sy) noexcept
{
      canvas* l_canvas = new(std::nothrow) canvas(this, mode, px, py, sx, sy);
      if(l_canvas) {
          if(m_surface_tail) {
              m_surface_tail->p_next = l_canvas;
          } else
              m_surface_head = l_canvas;
          l_canvas->p_next  = nullptr;
          m_surface_tail = l_canvas;
          if((sx > 0) && (sy > 0)) {
              m_geometry_dirty = true;
              m_graphics_dirty = true;
          }
      }
      return l_canvas;
}

canvas* scene::free_canvas(canvas* object) noexcept
{
      gfx_free_surface(object);
      return nullptr;
}

sprite* scene::make_sprite(std::uint8_t mode, std::uint8_t sx, std::uint8_t sy) noexcept
{
      sprite* l_sprite = new(std::nothrow) sprite(this, mode, sx, sy);
      if(l_sprite) {
          if(m_surface_tail) {
              m_surface_tail->p_next = l_sprite;
          } else
              m_surface_head = l_sprite;
          l_sprite->p_next = nullptr;
          m_surface_tail = l_sprite;
          if((sx > 0) && (sy > 0)) {
              m_geometry_dirty = true;
              m_graphics_dirty = true;
          }
      }
      return l_sprite;
}

sprite* scene::free_sprite(sprite* object) noexcept
{
      gfx_free_surface(object);
      return nullptr;
}

bool  scene::resume() noexcept
{
      if(m_running == false) {
          if(s_scene == nullptr) {
              s_scene = this;
              m_screen_size = m_viewport_geometry.get_width() * m_viewport_geometry.get_height();
              if(m_screen_size > 0) {
                  m_screen_ptr  = reinterpret_cast<std::uint8_t*>(malloc(m_screen_size));
              }
              if(m_screen_ptr) {
                  m_draw_wait.store(false);
                  m_draw_lock.store(false);
                  m_draw_loop.store(true);
                  // multicore_launch_core1(gfx_render_loop);
                  m_graphics_dirty = true;
                  m_graphics_dirty = true;
                  m_running = true;
              }
          }
      }
      return m_running == true;
}

void  scene::render() noexcept
{
      if(m_running) {
          if(m_geometry_dirty) {
              gfx_update_geometry();
          }
          if(s_frame_redraw || m_graphics_dirty) {
              gfx_update_graphics();
          }
      }
}

bool  scene::suspend() noexcept
{
      if(m_running == true) {
          m_draw_loop.store(false);
          m_running = false;
          free(m_screen_ptr);
          m_screen_ptr  = nullptr;
          m_screen_size = 0;
          s_scene   = nullptr;
      }
      return m_running == false;
}

bool  scene::sync(float dt) noexcept
{
      bool l_result = false;
      if(m_running) {
          if(dt > 0.0f) {
              surface* l_surface_iter = m_surface_head;
              while(l_surface_iter) {
                  l_result |= l_surface_iter->sync(dt);
                  l_surface_iter = l_surface_iter->p_next;
              }
              m_graphics_dirty = l_result;
          }
      }
      return l_result;
}

void  scene::draw() noexcept
{
      if(m_running) {
          gfx_update_graphics();
      }
}

/*namespace gfx*/ }
/*namespace dev*/ }
