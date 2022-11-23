#include "sys.h"
#include "memory.h"
#include "gfx.h"

void* g_surface;
int   g_display_sx;
int   g_display_sy;

bool  prepare()
{
      g_surface = gfx_make_surface(fmt_mono_bcs | fmt_indexed, opt_request_tile_graphics, map_copy);
      if(g_surface != NULL) {
          g_display_sx = gfx_get_display_sx();
          g_display_sy = gfx_get_display_sy();
          return true;
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
