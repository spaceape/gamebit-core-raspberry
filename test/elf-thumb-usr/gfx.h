#ifndef usr_gfx_h
#define usr_gfx_h
#include "sys.h"

/* tile format flags
 * [ - - - -  F F - - ]
 * 
 * LB:  [ C7 C6 C5 C4 C3 C2 C1 C0 ]
 * HB:  [ -- Z2 Z1 Z0 -- CA C9 C8 ]
 * XB0: [ B7 B6 B5 B4 B3 B2 B1 B0 ]
 * XB1: [ F7 F6 F5 F4 F3 F2 F1 F0 ]
 * 
 * Where:
 * CX - character index
 * BX - background colour index
 * FX - foreground colour index
 * ZX - fusion mode: 8x8 and 16x16 tile size only
 *      on specifically designed charsets, up to eight characters can be grouped together in order to form a multicolour tile; 
 *      each character in the group will encode one colour bit, with bit 0 having the lower index in the charset
*/
static const uint8_t mode_lb = 0u;
static const uint8_t fmt_mono_bcs = mode_lb;      // monochrome and basic charset

static const uint8_t mode_hb = 4u;
static const uint8_t fmt_mono_xcs = mode_hb;      // monochrome and extended charset

static const uint8_t mode_cb = 12u;                // 256 colour, extended charset
static const uint8_t fmt_colour_xcs = mode_cb;

static const uint8_t fmt_tile = 0b00001100;

/* raster format flags
 * [ - F F F  - - S S ]
*/
static const uint8_t fmt_undef = 0;
static const uint8_t fmt_argb  = 16;
static const uint8_t mode_argb_1111 = fmt_argb | 0 | 1;
static const uint8_t mode_argb_2222 = fmt_argb | 0 | 0;
static const uint8_t mode_argb_4444 = fmt_argb | 2 | 1;
static const uint8_t mode_argb_8888 = fmt_argb | 2 | 0;

static const uint8_t fmt_grayscale = 32;
static const uint8_t mode_grayscale = fmt_grayscale;
static const uint8_t mode_grayscale_8 = fmt_grayscale;

static const uint8_t fmt_indexed = 64;
static const uint8_t mode_indexed = fmt_indexed;
static const uint8_t mode_indexed_8 = fmt_indexed;

/* map_*
    surface render flags
*/
static const unsigned int  map_none = 0;       // no mapping, surface will not be rendered
static const unsigned int  map_auto = 0;       // choose the default mapping (copy)
static const unsigned int  map_copy = 1;
static const unsigned int  map_opaque = 2;
static const unsigned int  map_volatile = 128;

/* smp_*
    surface option flags
*/
static const unsigned int  opt_none = 0;
static const unsigned int  opt_request_tile_graphics = 1;
static const unsigned int  opt_request_raster_graphics = 2;
static const unsigned int  opt_request_vector_graphics = 4;
static const unsigned int  opt_opaque = 16;

/* gfx_*
 * core graphics API
*/
void*    gfx_make_surface(uint8_t, unsigned int, unsigned int) __attribute__((long_call));
void     gfx_set_colour_count(void*, int) __attribute__((long_call));
void     gfx_set_glyph_size(void*, int, int) __attribute__((long_call));
void     gfx_map(void*, int, int) __attribute__((long_call));

// bool     gfx_set_format(unsigned int, int, int, int) __attribute__((long_call));
// void     gfx_set_option_flags(unsigned int) __attribute__((long_call));
// void     gfx_set_render_flags(unsigned int) __attribute__((long_call));
// bool     gfx_set_window_size(int, int) __attribute__((long_call));
// 
// uint8_t* gfx_get_lb_ptr() __attribute__((long_call));
// uint8_t* gfx_get_hb_ptr() __attribute__((long_call));
// uint8_t* gfx_get_xb0_ptr() __attribute__((long_call));
// uint8_t* gfx_get_xb1_ptr() __attribute__((long_call));
// 
// void     gfx_scroll_rel(int, int) __attribute__((long_call));
// void     gfx_scroll_abs(int, int) __attribute__((long_call));

void     gfx_unmap(void*) __attribute__((long_call));
void     gfx_free_surface(void*) __attribute__((long_call));

int      gfx_get_display_sx() __attribute__((long_call));
int      gfx_get_display_sy() __attribute__((long_call));

#endif
