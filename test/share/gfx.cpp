#include "gfx.h"

extern gfx::device* g_display_ptr;

gfx_surface* gfx_make_surface(std::uint8_t format, unsigned int option_flags, unsigned int render_flags) noexcept
{
      return new(std::nothrow) gfx_surface(g_display_ptr, option_flags, render_flags);
}

void  gfx_set_colour_count(gfx_surface* surface_ptr, int colour_count) noexcept
{
      surface_ptr->set_colour_count(colour_count);
}

void  gfx_set_glyph_size(gfx_surface* surface_ptr, int glyph_size_x, int glyph_size_y) noexcept
{
      surface_ptr->set_glyph_size(glyph_size_x, glyph_size_y);
}

void  gfx_map(gfx_surface*, int, int) noexcept
{
}

void  gfx_unmap(gfx_surface*) noexcept
{
}

void  gfx_free_surface(gfx_surface* surface_ptr) noexcept
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
