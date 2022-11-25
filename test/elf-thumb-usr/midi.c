#include "sys.h"
#include "memory.h"
#include "gfx.h"

void* g_header_surface;
void* g_surface;
int   g_display_sx;
int   g_display_sy;

bool  prepare()
{
      g_display_sx = gfx_get_display_sx();
      g_display_sy = gfx_get_display_sy();
      g_header_surface = gfx_make_surface();
      if(g_header_surface != NULL) {
          if(gfx_map(g_header_surface, 0, 0, g_display_sx, 8)) {
              gfx_set_format(g_header_surface, mode_lb | mode_argb_4444, 256, 8, 8);
              gfx_set_option_flags(g_header_surface, opt_request_tile_graphics);
              gfx_set_render_flags(g_header_surface, map_copy);
              gfx_set_window_size(g_header_surface, g_display_sx, 1);
              return true;
          }
      }
      return false;
}

void  dispose()
{
      if(g_surface != NULL) {
          gfx_free_surface(g_surface);
      }
}

int   main()
{
}
