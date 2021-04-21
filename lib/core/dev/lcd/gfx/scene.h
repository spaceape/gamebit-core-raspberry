#ifndef dev_gfx_scene_h
#define dev_gfx_scene_h
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
#include "metrics.h"
#include <atomic>
#include <vector>

namespace dev {
namespace gfx {

class scene: public gfx::dc
{
  gfx::box2d_t    m_viewport_geometry;
  std::uint8_t    m_background_colour;
  std::int16_t    m_canvas_count;
  std::int16_t    m_sprite_count;

  std::uint8_t*   m_screen_ptr;
  std::uint32_t   m_screen_size;

  struct draw_node_t
  {
    std::uint8_t  m_type;
    std::uint8_t  m_fill;
    surface*      m_surface;
    gfx::box2d_t  m_source_geometry;
    gfx::box2d_t  m_target_geometry;
  };

  std::atomic<bool>         m_draw_lock;  // core1 -> core0: signals that the render loop is currently drawing
  std::atomic<bool>         m_draw_wait;  // core0 -> core1: locks the render loop in a wait state
  std::atomic<bool>         m_draw_loop;  // core0 -> core1: signals the render loop to keep running
  std::vector<draw_node_t>  m_draw_list;

  surface*        m_surface_head;
  surface*        m_surface_tail;
  bool            m_geometry_dirty:1;
  bool            m_graphics_dirty:1;
  bool            m_running:1;

  private:
          void  dwl_make_colour(std::uint16_t) noexcept;
          void  dwl_make_canvas(canvas*, box2d_t&, box2d_t&) noexcept;
          void  dwl_make_sprite(sprite*, box2d_t&, box2d_t&) noexcept;
          bool  gfx_update_geometry() noexcept;
          void  gfx_update_graphics() noexcept;
          void  gfx_flush() noexcept;
          void  gfx_free_surface(surface*) noexcept;

  protected:
  static  void  gfx_render_loop() noexcept;

  public:
          scene(int, int) noexcept;
          scene(const scene&) noexcept = delete;
          scene(scene&&) noexcept = delete;
  virtual ~scene();
          bool    set_viewport_position(std::int16_t, std::int16_t) noexcept;
          bool    set_viewport_size(std::uint16_t, std::uint16_t) noexcept;
          bool    set_viewport_geometry(std::int16_t, std::int16_t, std::int16_t, std::int16_t) noexcept;
          void    set_fill_colour(std::uint16_t) noexcept;
          canvas* make_canvas(std::uint8_t, std::int16_t, std::int16_t, std::uint16_t, std::uint16_t) noexcept;
          canvas* free_canvas(canvas*) noexcept;
          sprite* make_sprite(std::uint8_t, std::uint8_t, std::uint8_t) noexcept;
          sprite* free_sprite(sprite*) noexcept;
          bool    resume() noexcept;
          void    render() noexcept;
          bool    suspend() noexcept;
          bool    sync(float) noexcept;
          void    draw() noexcept;
          scene&  operator=(const scene&) noexcept = delete;
          scene&  operator=(scene&&) noexcept = delete;
};

/*namespace gfx*/ }
/*namespace dev*/ }
#endif
