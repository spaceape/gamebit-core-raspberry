#include <stdio.h>
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"

#include "elf.h"
#include "uld/uld.h"
#include "uld/target.h"
#include "uld/image.h"

#include "elf-thumb-gamebit/game.h"

#include "pico.h"
#include "hw_config.h"
#include <dbg.h>

uint8_t   PatternTable[1024];
uint8_t   NameTable[1024];
uint8_t   AttrTable[1024];
uint16_t  PaletteTable[1024];

typedef int(*init_call_type)();
typedef void(*update_call_type)(std::uint32_t);
typedef int(*dispose_call_type)();

// void  dbg_dump_hex(void* bytes, int size) noexcept
// {
//       int  i_byte = 0;
//       int  l_width = 16;
//       char l_hex_cache[128];
//       char l_ascii_cache[128];
//       auto l_data_ptr = reinterpret_cast<std::uint8_t*>(bytes);
//       while(i_byte < size) {
//           int  i_column       = 0;
//           int  l_data_offset  = i_byte;
//           int  l_print_offset = 0;
//           // print hex dump
//           while(i_column < l_width) {
//               l_ascii_cache[i_column] = '.';
//               if(i_byte < size) {
//                   l_print_offset += snprintf(l_hex_cache + l_print_offset, 4, "%.2x ",  *l_data_ptr);
//                   if((*l_data_ptr > 32) &&
//                       (*l_data_ptr < 128)) {
//                       l_ascii_cache[i_column] = *l_data_ptr;
//                   }
//                   l_data_ptr++;
//               } else
//                   l_print_offset += snprintf(l_hex_cache + l_print_offset, 4, "   ");
//               i_column++;
//               i_byte++;
//           }
//           l_hex_cache[l_print_offset] = 0;
//           l_ascii_cache[i_column] = 0;
//           printf("    %.8x: %s %s\n", l_data_offset, l_hex_cache, l_ascii_cache);
//       }
// }

void  boot(const char* game) noexcept
{
    sd_card_t*  pSD = sd_get_by_num(0);
    FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if(fr == FR_OK) {
        uld::target l_target(EM_ARM, ELFCLASS32, true, true);
        uld::image  l_image(std::addressof(l_target));
        printf("--- PatternTable: %p\n", std::addressof(PatternTable[0]));
        l_image.make_symbol("PatternTable", uld::symbol_t::type_object, uld::symbol_t::bind_global, PatternTable);
        printf("--- NameTable: %p\n", std::addressof(NameTable[0]));
        l_image.make_symbol("NameTable", uld::symbol_t::type_object, uld::symbol_t::bind_global, NameTable);
        printf("--- AddrTable: %p\n", std::addressof(AttrTable[0]));
        l_image.make_symbol("AttrTable", uld::symbol_t::type_object, uld::symbol_t::bind_global, AttrTable);
        printf("--- PaletteTable: %p\n", std::addressof(PaletteTable[0]));
        l_image.make_symbol("PalletteTable", uld::symbol_t::type_object, uld::symbol_t::bind_global, PaletteTable);
        if(l_image.load(game)) {
            int               l_rc;
            init_call_type    p_init_call = nullptr;
            update_call_type  p_update_call = nullptr;
            dispose_call_type p_dispose_call = nullptr;

            if(uld::symbol_t*
                l_data_ptr = l_image.find_symbol("Patterns");
                l_data_ptr != nullptr) {
                printf("%s: %p\n", l_data_ptr->name, l_data_ptr->ea);
            }
            if(uld::symbol_t*
                l_data_ptr = l_image.find_symbol("Palettes");
                l_data_ptr != nullptr) {
                printf("%s: %p\n", l_data_ptr->name, l_data_ptr->ea);
            }

            // show the internal game tables
            if(uld::symbol_t*
                l_call_ptr = l_image.find_symbol("UploadU8PatternData");
                l_call_ptr != nullptr) {
                printf("%s(): %p\n", l_call_ptr->name, l_call_ptr->ra);
            }
            if(uld::symbol_t*
                l_call_ptr = l_image.find_symbol("ConvertU8toU2");
                l_call_ptr != nullptr) {
                printf("%s(): %p\n", l_call_ptr->name, l_call_ptr->ra);
            }

            // show the game functions
            if(uld::symbol_t*
                l_call_ptr = l_image.find_symbol("init");
                l_call_ptr != nullptr) {
                printf("%s(): %p\n", l_call_ptr->name, l_call_ptr->ra);
                p_init_call = reinterpret_cast<init_call_type>(l_call_ptr->ra);
            }
            if(uld::symbol_t*
                l_call_ptr = l_image.find_symbol("update");
                l_call_ptr != nullptr) {
                printf("%s(): %p\n", l_call_ptr->name, l_call_ptr->ra);
                p_update_call = reinterpret_cast<update_call_type>(l_call_ptr->ra);
            }
            if(uld::symbol_t*
                l_call_ptr = l_image.find_symbol("uninit");
                l_call_ptr != nullptr) {
                printf("%s(): %p\n", l_call_ptr->name, l_call_ptr->ra);
                p_dispose_call = reinterpret_cast<dispose_call_type>(l_call_ptr->ra);
            }

            // run the program: init()
            if(p_init_call != nullptr) {
                l_rc = p_init_call();
                printf("--- init: rc=%d\n", l_rc);
                if(l_rc != 0) {
                    printf("-!- FAILED.\n");
                    return;
                }
            }

            // run the program: update() a few times
            if(p_update_call == nullptr) {
                printf("-!- FAILED: update() is not defined or inaccessible.\n");
                return;
            }
            p_update_call(128);

            // run the program: goodbye
            if(p_dispose_call != nullptr) {
                l_rc = p_dispose_call();
                printf("--- dispose: rc=%d\n", l_rc);
                if(l_rc != 0) {
                    printf("-!- FAILED.\n");
                    return;
                }
            }

            printf("bye.\n");
        }
        f_unmount(pSD->pcName);
    }
}

int   main(int, char**)
{
      time_init();
      stdio_init_all();
      boot("game.o");
      return 0;
}
