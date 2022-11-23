/** 
**/
#include "gfx-surface.h"

      gfx_surface::gfx_surface(gfx::device* device, unsigned int option_flags, unsigned int render_flags) noexcept:
      gfx::surface(false),
      m_format(gfx::fmt_mono_bcs | gfx::fmt_indexed),
      m_colour_count(256),
      m_glyph_sx(8),
      m_glyph_sy(8),
      m_option_flags(option_flags),
      m_render_flags(render_flags)
{
}

      gfx_surface::~gfx_surface()
{
}

bool  gfx_surface::gfx_prepare(gfx::device*) noexcept
{
      gfx_set_format(m_format, m_colour_count, m_glyph_sx, m_glyph_sy);
      gfx_set_option_flags(m_option_flags);
      gfx_set_render_flags(m_render_flags);
      return true;
}

void  gfx_surface::gfx_draw() noexcept
{
}

void  gfx_surface::gfx_sync(int) noexcept
{
}

bool  gfx_surface::gfx_release(gfx::device*) noexcept
{
      return true;
}

void  gfx_surface::set_format(std::uint8_t format) noexcept
{
      m_format = format;
}

void  gfx_surface::set_colour_count(int colour_count) noexcept
{
      m_colour_count = colour_count;
}

void  gfx_surface::set_glyph_size(int gsx, int gsy) noexcept
{
      m_glyph_sx = gsx;
      m_glyph_sy = gsy;
}
