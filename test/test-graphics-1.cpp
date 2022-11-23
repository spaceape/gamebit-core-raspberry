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
#include "share/gfx.h"
#include "hw_config.h"
#include <dbg.h>

      gfx::device* g_display_ptr;

void  boot(const char* process) noexcept
{
      // summon the display driver from the ADT
      dev::ili9341s*  l_display_ptr = sys::adt_cast<dev::ili9341s>("/dev/display");
      if(l_display_ptr == nullptr) {
          printf("/!\\ FAILED: %s %d: Display `%s` not found.\n", __FILE__, __LINE__, "/dev/display");
          return;
      }

      // set up the sequencer task
      uld::target   l_target(EM_ARM, ELFCLASS32, true, true);
      uld::image    l_image(std::addressof(l_target));

      l_image.make_symbol("malloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)malloc);
      l_image.make_symbol("realloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)realloc);
      l_image.make_symbol("free", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)free);
      l_image.make_symbol("memcpy", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)memcpy);

      l_image.make_symbol("gfx_make_surface", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_make_surface);
      l_image.make_symbol("gfx_set_colour_count", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_set_colour_count);
      l_image.make_symbol("gfx_set_glyph_size", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_set_glyph_size);
      l_image.make_symbol("gfx_map", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_map);
      l_image.make_symbol("gfx_unmap", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_unmap);
      l_image.make_symbol("gfx_free_surface", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_free_surface);

      l_image.make_symbol("gfx_get_display_sx", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_get_display_sx);
      l_image.make_symbol("gfx_get_display_sy", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)gfx_get_display_sy);

      l_image.make_symbol("signal", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)sys_signal);
      l_image.make_symbol("abort", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)sys_abort);
      l_image.make_symbol("exit", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)sys_exit);

      if(l_image.load(process)) {
          g_display_ptr = l_display_ptr;
          if(auto
              l_prepare_call = get_function<bool>(l_image, "prepare");
              l_prepare_call == nullptr) {
              printf("/!\\ FAILED: %s %d: %s: `prepare()` call not found.", __FILE__, __LINE__, process);
              return;
          } else
          if(bool
              l_prepare_success = l_prepare_call();
              l_prepare_success == false) {
              printf("/!\\ FAILED: %s %d: %s: `prepare()` executed unsuccessfully.", __FILE__, __LINE__, process);
          }
          auto l_dispose_call = get_function<void>(l_image, "dispoase");
          while(true) {
              g_adt->sync(1);
          }
      }


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
