#ifndef cor_gfx_h
#define cor_gfx_h
// gfx.h
// bindings for the GFX library
#include <stdlib.h>
#include <gfx.h>
#include <gfx/device.h>
#include "gfx-surface.h"

gfx_surface*  gfx_make_surface(std::uint8_t, unsigned int, unsigned int) noexcept;
void          gfx_set_colour_count(gfx_surface*, int) noexcept;
void          gfx_set_glyph_size(gfx_surface*, int, int) noexcept;
void          gfx_map(gfx_surface*, int, int) noexcept;
void          gfx_unmap(gfx_surface*) noexcept;
void          gfx_free_surface(gfx_surface*) noexcept;
void          gfx_get_display_sx() noexcept;
void          gfx_get_display_sy() noexcept;

#endif
