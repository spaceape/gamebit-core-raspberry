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
#include "process.h"
#include "function.h"

// #include "tile/cbo.h"
// #include "tile/cmo.h"
// #include "tile/cso.h"

      process::process(uld::target* target, gfx::device* display, const char* object) noexcept:
      surface(false),
      m_image(target),
      m_display_ptr(display)
{
      m_image.make_symbol("malloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)malloc);
      m_image.make_symbol("realloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)realloc);
      m_image.make_symbol("free", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)free);
      m_image.make_symbol("memcpy", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)memcpy);

      // m_image.make_symbol("gfx_set_format", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_set_format);
      // m_image.make_symbol("gfx_set_option_flags", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_set_format);
      // m_image.make_symbol("gfx_set_render_flags", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_set_render_flags);
      // m_image.make_symbol("gfx_set_window_size", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_set_window_size);

      // m_image.make_symbol("gfx_get_lb_ptr", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_get_lb_ptr);
      // m_image.make_symbol("gfx_get_hb_ptr", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_get_hb_ptr);
      // m_image.make_symbol("gfx_get_xb0_ptr", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_get_xb0_ptr);
      // m_image.make_symbol("gfx_get_xb1_ptr", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_get_xb1_ptr);

      // m_image.make_symbol("gfx_scroll_rel", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_scroll_rel);
      // m_image.make_symbol("gfx_scroll_abs", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_scroll_abs);

      // m_image.make_symbol("signal", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_signal);
      // m_image.make_symbol("abort", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_abort);
      // m_image.make_symbol("exit", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_exit);

      if(m_image.load(object)) {
          m_display_sx = m_display_ptr->get_display_sx();
          m_display_sy = m_display_ptr->get_display_sy();
          m_display_ptr->map(this, 0, 0, m_display_sx, m_display_sy);
      }
}

      process::~process()
{
}

bool  process::gfx_prepare(gfx::device*) noexcept
{
      gfx_set_format(gfx::fmt_mono_bcs | gfx::fmt_indexed, 256, 8, 8);
      gfx_set_option_flags(gfx::surface::opt_request_tile_graphics);             // default option flags, can be ommitted
      gfx_set_render_flags(gfx::surface::map_copy);                              // default render flags, can be ommitted
      return true;
}

void  process::gfx_draw() noexcept
{
}

void  process::gfx_sync(int) noexcept
{
}

bool  process::gfx_release(gfx::device*) noexcept
{
      return true;
}
