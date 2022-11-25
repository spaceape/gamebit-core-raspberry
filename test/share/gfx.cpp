#include "gfx.h"

extern gfx::device* g_display_ptr;

gfx::surface* gfx_make_surface() noexcept
{
      return new(std::nothrow) gfx::surface(false);
}

bool  gfx_map(gfx::surface* surface_ptr, int px, int py, int sx, int sy) noexcept
{
      return g_display_ptr->map(surface_ptr, px, py, sx, sy);
}

bool  gfx_set_format(gfx::surface* surface_ptr, unsigned int format, int cc, int glyph_sx, int glyph_sy) noexcept
{
      return g_display_ptr->set_format(surface_ptr, format, cc, glyph_sx, glyph_sy);
}

void  gfx_set_option_flags(gfx::surface* surface_ptr, unsigned int option_flags) noexcept
{
      return g_display_ptr->set_option_flags(surface_ptr, option_flags);
}

void  gfx_set_render_flags(gfx::surface* surface_ptr, unsigned int render_flags) noexcept
{
      return g_display_ptr->set_option_flags(surface_ptr, render_flags);
}

void  gfx_set_window_size(gfx::surface* surface_ptr, int sx, int sy) noexcept
{
      g_display_ptr->set_window_size(surface_ptr, sx, sy);
}

std::uint8_t* gfx_get_lb_ptr(gfx::surface* surface_ptr) noexcept
{
      return g_display_ptr->get_lb_ptr(surface_ptr);
}

std::uint8_t* gfx_get_hb_ptr(gfx::surface* surface_ptr) noexcept
{
      return g_display_ptr->get_hb_ptr(surface_ptr);
}

std::uint8_t* gfx_get_xb0_ptr(gfx::surface* surface_ptr) noexcept
{
      return g_display_ptr->get_xb0_ptr(surface_ptr);
}

std::uint8_t* gfx_get_xb1_ptr(gfx::surface* surface_ptr) noexcept
{
      return g_display_ptr->get_xb1_ptr(surface_ptr);
}

void  gfx_scroll_rel(gfx::surface* surface_ptr, int dx, int dy) noexcept
{
      return g_display_ptr->scroll_rel(surface_ptr, dx, dy);
}

void  gfx_scroll_abs(gfx::surface* surface_ptr, int dx, int dy) noexcept
{
      return g_display_ptr->scroll_abs(surface_ptr, dx, dy);
}

bool  gfx_unmap(gfx::surface* surface_ptr) noexcept
{
      return g_display_ptr->unmap(surface_ptr);
}

void  gfx_free_surface(gfx::surface* surface_ptr) noexcept
{
      delete surface_ptr;
}

void  gfx_get_display_sx() noexcept
{
      g_display_ptr->get_display_sx();
}

void  gfx_get_display_sy() noexcept
{
      g_display_ptr->get_display_sy();
}
