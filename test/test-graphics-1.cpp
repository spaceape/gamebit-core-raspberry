#include "pico.h"
#include <stdio.h>
#include <elf.h>
#include <sys.h>
#include <sys/adt/directory.h>
#include <uld.h>
#include <uld/target.h>
#include <uld/image.h>
#include <blackops.h>
#include <dev/ili9341s.h>
#include "share/function.h"
#include "share/sys.h"
#include "share/file.h"
#include "share/gfx.h"
#include "hw_config.h"
#include <dbg.h>

extern gfx::device*   g_display_ptr;
static gfx::surface*  g_header_surface;
static gfx::surface*  g_surface;
static int            g_display_sx;
static int            g_display_sy;

bool  __prepare()
{
      g_display_sx = gfx_get_display_sx();
      g_display_sy = gfx_get_display_sy();
      g_header_surface = gfx_make_surface();
      if(g_header_surface != nullptr) {
          if(gfx_map(g_header_surface, 0, 0, g_display_sx, 8)) {
              gfx_set_option_flags(g_header_surface, gfx::surface::opt_request_tile_graphics);
              gfx_set_render_flags(g_header_surface, gfx::surface::map_copy);
              gfx_set_window_size(g_header_surface, g_display_sx, 1);
              gfx_set_format(g_header_surface, gfx::mode_lb | gfx::mode_argb_4444, 256, 8, 8);
              return true;
          }
      }
      return false;
}

void  __suspend()
{
      if(g_surface != NULL) {
          gfx_free_surface(g_surface);
      }
}

int   __aux()
{
}

void  boot(const char* process) noexcept
{
      // summon the display driver from the ADT
      g_display_ptr = sys::adt_cast<dev::ili9341s>("/dev/display");
      if(g_display_ptr == nullptr) {
          printf("/!\\ FAILED: %s %d: Display `%s` not found.\n", __FILE__, __LINE__, "/dev/display");
          return;
      }

      __prepare();
      __suspend();

      // if(l_image.load(process)) {
      //     g_display_ptr = l_display_ptr;
      //     if(auto
      //         l_prepare_call = get_function<bool>(l_image, "prepare");
      //         l_prepare_call == nullptr) {
      //         printf("/!\\ FAILED: %s %d: %s: `prepare()` call not found.\n", __FILE__, __LINE__, process);
      //         return;
      //     } else
      //     if(bool
      //         l_prepare_success = l_prepare_call();
      //         l_prepare_success == false) {
      //         printf("/!\\ FAILED: %s %d: %s: `prepare()` executed unsuccessfully.\n", __FILE__, __LINE__, process);
      //     }
      //     auto l_dispose_call = get_function<void>(l_image, "dispoase");
      //     while(true) {
      //         g_adt->sync(1);
      //     }
      // }
}

int   main(int, char**)
{
      initialise();

      // set up SD card for ULD
      sd_card_t*  pSD = sd_get_by_num(0);
      FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
      if(fr != FR_OK) {
          printf("/!\\ FAILED: %s %d: Unable to mount filesystem.\n", __FILE__, __LINE__, "/dev/display");
          return 1;
      }

      boot("midi.o");

      f_unmount(pSD->pcName);

      dispose();
      return 0;
}
