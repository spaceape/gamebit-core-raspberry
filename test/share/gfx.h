#ifndef cor_gfx_h
#define cor_gfx_h
// gfx.h
// bindings for the GFX library
#include <stdlib.h>
#include <gfx.h>
#include <gfx/device.h>
#include <gfx/surface.h>

gfx::surface* gfx_make_surface() noexcept;

bool   gfx_map(gfx::surface*, int, int, int, int) noexcept;

bool   gfx_set_format(gfx::surface*, unsigned int, int, int, int) noexcept;
void   gfx_set_option_flags(gfx::surface*, unsigned int) noexcept;
void   gfx_set_render_flags(gfx::surface*, unsigned int) noexcept;
void   gfx_set_window_size(gfx::surface*, int, int) noexcept;

std::uint8_t* gfx_get_lb_ptr(gfx::surface*) noexcept;
std::uint8_t* gfx_get_hb_ptr(gfx::surface*) noexcept;
std::uint8_t* gfx_get_xb0_ptr(gfx::surface*) noexcept;
std::uint8_t* gfx_get_xb1_ptr(gfx::surface*) noexcept;

void   gfx_scroll_rel(gfx::surface*, int, int) noexcept;
void   gfx_scroll_abs(gfx::surface*, int, int) noexcept;

bool   gfx_unmap(gfx::surface*) noexcept;

void   gfx_free_surface(gfx::surface*) noexcept;

int    gfx_get_display_sx() noexcept;
int    gfx_get_display_sy() noexcept;

#endif
