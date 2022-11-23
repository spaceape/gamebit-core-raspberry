#include <stdio.h>
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"

#include "uld/uld.h"

#include "uld/target.h"
#include "uld/image.h"

#include "uld/image/pool.h"

#include "uld/bfd/raw.h"
#include "uld/bfd/bin.h"
#include "uld/bfd/elf32.h"
//
#include "pico.h"
#include <stdio.h>
#include "hw_config.h"
#include <dbg.h>

int   sumxy(int x, int y) noexcept
{
      return x + y;
}

int   sumxyz(int x, int y, int z) noexcept
{
      int lhs = sumxy(x, y);
      int ret = sumxy(lhs, z);
      return ret;
}

void  test_dummy_01() noexcept
{
      struct node_type
      {
        int  v0;
        int  v1;
        int  v2;
        int  v3;
      };

      struct dummy_base_t
      {
        int                 m_used;         // count of occupied elements (and also index of the first free element)
        int                 m_gto_base;     // global table offset at the beginning of the page
        int                 m_gto_next;     // global table offset at the end of the allocated space
        int                 m_size;         // capacity
        dummy_base_t*       m_page_prev;
        dummy_base_t*       m_page_next;
      };

      printf("--- sizeof(dummy_base_t) == %d\n", sizeof(dummy_base_t));

      int ret = sumxyz(1, 2, 3);

      printf("--- ret == %d\n", ret);
}

void  test_dummy_02() noexcept
{
      union u_t {
        std::int32_t value_32;
        std::int16_t value_16[2];
        std::int8_t  bytes[4];
      };

      u_t u;
      u.value_32 = 0x12345678;
      printf("--- value:32 = %.8x\n", u.value_32);
      printf("--- value:16 = %.4x %.4x\n", u.value_16[0], u.value_16[1]);
      printf("--- value:8 = %.2x %.2x %.2x %.2x\n", u.bytes[0], u.bytes[1], u.bytes[2], u.bytes[3]);
}

void  test_cache_01() noexcept
{
    sd_card_t*  pSD = sd_get_by_num(0);
    FRESULT fr;
    if(fr = f_mount(&pSD->fatfs, pSD->pcName, 1); fr == FR_OK) {
        auto l_file = uld::util::file_ptr::make_file_cb();
        if(fr = f_open(l_file, "sumxy.o", FA_OPEN_EXISTING | FA_READ); fr == FR_OK) {
            int        l_data_size = 1024;
            void*      l_void_ptr  = malloc(l_data_size);
            if(l_void_ptr) {
                auto                    l_data_ptr = reinterpret_cast<std::uint8_t*>(l_void_ptr);
                uld::util::data_cache_t l_data_cache(l_file, 0);
                l_data_cache.raw_get(l_data_ptr, l_data_size);
                dbg_dump_hex(l_data_ptr, l_data_size);
                f_close(l_file);
            }
        } else
            printf("--- [FAIL] test_cache_01: open\n");
    } else
        printf("--- [FAIL] test_cache_01: mount\n");

    
    f_unmount(pSD->pcName);
}

void  test_pool_01() noexcept
{
      int                 l_index = 0;
      uld::string_table_t l_strtab;
      for(l_index = 0; l_index < 4096; l_index++) {
          l_strtab.make_string("01234567");
      }
      printf("--- str[127] = \"%s\".\n", l_strtab.get_string(127));
      printf("--- str[128] = \"%s\".\n", l_strtab.get_string(128));
      printf("--- str[129] = \"%s\".\n", l_strtab.get_string(129));
}

void  test_pool_02() noexcept
{
}

void  test_elf_01() noexcept
{
    time_init();
    sd_init_driver();

    puts("Hello, world!");

    // See FatFs - Generic FAT Filesystem Module, "Application Interface",
    // http://elm-chan.org/fsw/ff/00index_e.html
    sd_card_t *pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (FR_OK != fr) panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    FIL fil;
    const char* const filename = "hello.txt";
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    if (f_printf(&fil, "Hello, world!\n") < 0) {
        printf("f_printf failed\n");
    }
    fr = f_close(&fil);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    f_unmount(pSD->pcName);

    puts("Goodbye, world!");
}

void  test_elf_sumxy_01() noexcept
{
    sd_card_t*  pSD = sd_get_by_num(0);
    FRESULT   fr   = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (fr == FR_OK) {
        uld::raw_bfd_t l_raw_file("sumxy.o");
        if(l_raw_file.has_type(uld::file_type_elf)) {
            uld::bin_bfd_t l_object_file(l_raw_file);
            if(l_object_file.has_class(uld::bin_32)) {
                uld::elf32_bfd_t l_elf32_file(l_object_file);
                if(l_elf32_file.has_machine_type(EM_ARM)) {
                    printf("YAY\n");
                }
            }
        }
        // fr = f_open(&fil, sumxy.o, FA_OPEN_EXISTING | FA_READ);
        // if(fr == FR_OK) {
        // }
        // fr = f_close(&fil);
        f_unmount(pSD->pcName);
    }
}

void  test_elf_sumxy_02() noexcept
{
    sd_card_t*  pSD = sd_get_by_num(0);
    FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if(fr == FR_OK) {
        uld::target  l_target(EM_ARM, ELFCLASS32, true, true);
        uld::image   l_image(std::addressof(l_target));
        if(l_image.load("sumxy.o") == true) {
            uld::symbol_t* l_call_ptr = l_image.find_symbol("sumxy");
            if(l_call_ptr) {
                int (*p_call)(int, int);
                int l_ret = 0;
                printf("%s():\n", l_call_ptr->name);
                printf("    type: %.8x\n", l_call_ptr->type);
                printf("    flags: %.8x\n", l_call_ptr->flags);
                printf("    size: %d\n", l_call_ptr->size);
                printf("    address: %p\n", l_call_ptr->ra);
                if(l_call_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_call_ptr->ea, l_call_ptr->size);
                    printf("---\n");
                }
                p_call = reinterpret_cast<int(*)(int, int)>(l_call_ptr->ra);
                l_ret  = (p_call)(5, 3);
                printf("ret=%d\n", l_ret);
            }
        }
        f_unmount(pSD->pcName);
    }
}

void  test_elf_sumxy_03() noexcept
{
    sd_card_t*  pSD = sd_get_by_num(0);
    FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if(fr == FR_OK) {
        uld::target  l_target(EM_ARM, ELFCLASS32, true, true);
        uld::image   l_image(std::addressof(l_target));
        if(l_image.load("sumxy.o")) {
            uld::symbol_t* l_call_ptr = l_image.find_symbol("sumxy");
            if(l_call_ptr) {
                // int (*p_call)(int, int);
                // int l_ret = 0;
                printf("%s():\n", l_call_ptr->name);
                printf("    type: %.8x\n", l_call_ptr->type);
                printf("    flags: %.8x\n", l_call_ptr->flags);
                printf("    size: %d\n", l_call_ptr->size);
                printf("    address: %p\n", l_call_ptr->ra);
                if(l_call_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_call_ptr->ea, l_call_ptr->size);
                    printf("---\n");
                }
                // p_call = reinterpret_cast<int(*)(int, int)>(l_call_ptr->location + 1);
                // l_ret  = (p_call)(5, 3);
                // printf("ret=%d\n", l_ret);
            }
        }
        if(l_image.load("sumxyz.o")) {
            uld::symbol_t* l_call_ptr = l_image.find_symbol("sumxyz");
            if(l_call_ptr) {
                int (*p_call)(int, int, int);
                int l_ret = 0;
                printf("%s():\n", l_call_ptr->name);
                printf("    type: %.8x\n", l_call_ptr->type);
                printf("    flags: %.8x\n", l_call_ptr->flags);
                printf("    size: %d\n", l_call_ptr->size);
                printf("    address: %p\n", l_call_ptr->ra);
                if(l_call_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_call_ptr->ea, l_call_ptr->size);
                    printf("---\n");
                }
                p_call = reinterpret_cast<int(*)(int, int, int)>(l_call_ptr->ra);
                l_ret  = (p_call)(5, 3, 1);
                printf("ret=%d\n", l_ret);
            }
        }
        f_unmount(pSD->pcName);
    }
}

extern "C"
int   sumextern(int x, int y)
{
      return x + y;
}

void  test_elf_sumxy_04() noexcept
{
    sd_card_t*  pSD = sd_get_by_num(0);
    FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if(fr == FR_OK) {
        uld::target  l_target(EM_ARM, ELFCLASS32, true, true);
        uld::image   l_image(std::addressof(l_target));
        uld::symbol_t* l_ext_ptr = l_image.make_symbol("sumextern", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(sumextern));
        if(l_image.load("sumextern.o")) {
            printf("%s():\n", l_ext_ptr->name);
            printf("    type: %.8x\n", l_ext_ptr->type);
            printf("    flags: %.8x\n", l_ext_ptr->flags);
            printf("    size: %d\n", l_ext_ptr->size);
            printf("    address: %p\n", l_ext_ptr->ra);
            uld::symbol_t* l_call_ptr = l_image.find_symbol("sumxy");
            if(l_call_ptr) {
                int (*p_call)(int, int);
                int l_ret = 0;
                printf("%s():\n", l_call_ptr->name);
                printf("    type: %.8x\n", l_call_ptr->type);
                printf("    flags: %.8x\n", l_call_ptr->flags);
                printf("    size: %d\n", l_call_ptr->size);
                printf("    address: %p\n", l_call_ptr->ra);
                if(l_call_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_call_ptr->ea, l_call_ptr->size);
                    printf("---\n");
                }
                p_call = reinterpret_cast<int(*)(int, int)>(l_call_ptr->ra);
                l_ret  = (p_call)(2, 1);
                printf("ret=%d\n", l_ret);
            }
        }
        f_unmount(pSD->pcName);
    }
}

void  test_elf_data_01() noexcept
{
    sd_card_t*  pSD = sd_get_by_num(0);
    FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if(fr == FR_OK) {
        uld::target  l_target(EM_ARM, ELFCLASS32, true, true);
        uld::image   l_image(std::addressof(l_target));
        if(l_image.load("localdata.o") == true) {
            std::uint8_t*  l_data_ptr = nullptr;
            if(uld::segment*
                l_segment_ptr = l_image.get_segment_by_attributes(
                                    uld::section_t::type_nobits,
                                    uld::section_t::data_write | uld::section_t::data_alloc
                                );
                l_segment_ptr != nullptr) {
                l_data_ptr = l_segment_ptr->get_table_ptr(0);
                if(l_data_ptr != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_data_ptr, 256);
                    printf("---\n");
                }
            }
            uld::symbol_t* l_call_ptr = l_image.find_symbol("clear");
            if(l_call_ptr) {
                void  (*p_call)();
                printf("%s():\n", l_call_ptr->name);
                printf("    type: %.8x\n", l_call_ptr->type);
                printf("    flags: %.8x\n", l_call_ptr->flags);
                printf("    size: %d\n", l_call_ptr->size);
                printf("    address: %p\n", l_call_ptr->ra);
                if(l_call_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_call_ptr->ea, l_call_ptr->size);
                    printf("---\n");
                }
                p_call = reinterpret_cast<void(*)()>(l_call_ptr->ra);
                p_call();
                if(l_data_ptr) {
                    printf("---\n");
                    dbg_dump_hex(l_data_ptr, 256);
                    printf("---\n");
                }
            }
        }
        f_unmount(pSD->pcName);
    }
}

void  test_elf_data_02() noexcept
{
    sd_card_t*  pSD = sd_get_by_num(0);
    FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if(fr == FR_OK) {
        uld::target  l_target(EM_ARM, ELFCLASS32, true, true);
        uld::image   l_image(std::addressof(l_target));
        if(l_image.load("globaldata.o") == true) {
            uld::symbol_t* l_data_ptr = l_image.find_symbol("data");
            if(l_data_ptr) {
                printf("%s (before):\n", l_data_ptr->name);
                printf("    type: %.8x\n", l_data_ptr->type);
                printf("    flags: %.8x\n", l_data_ptr->flags);
                printf("    size: %d\n", l_data_ptr->size);
                printf("    address: %p\n", l_data_ptr->ea);
                if(l_data_ptr->ea != nullptr) {
                    // set all zero bytes to 255
                    {
                        std::uint8_t* l_data_base = l_data_ptr->ea;
                        std::uint8_t* l_data_tail = l_data_ptr->ea + l_data_ptr->size;
                        for(std::uint8_t* i_data = l_data_base; i_data != l_data_tail; i_data++) {
                            if(*i_data == 0) {
                                *i_data = 255;
                            }
                        }
                    }
                    // dump data
                    printf("---\n");
                    dbg_dump_hex(l_data_ptr->ea, l_data_ptr->size);
                    printf("---\n");
                }
            }
            uld::symbol_t* l_call_ptr = l_image.find_symbol("clear");
            if(l_call_ptr) {
                void  (*p_call)();
                printf("%s():\n", l_call_ptr->name);
                printf("    type: %.8x\n", l_call_ptr->type);
                printf("    flags: %.8x\n", l_call_ptr->flags);
                printf("    size: %d\n", l_call_ptr->size);
                printf("    address: %p\n", l_call_ptr->ra);
                if(l_call_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_call_ptr->ea, l_call_ptr->size);
                    printf("---\n");
                }
                p_call = reinterpret_cast<void(*)()>(l_call_ptr->ra);
                p_call();
            }
            if(l_data_ptr) {
                printf("%s (after):\n", l_data_ptr->name);
                if(l_data_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_data_ptr->ea, l_data_ptr->size);
                    printf("---\n");
                }
            }
        }
        f_unmount(pSD->pcName);
    }
}

void  test_elf_call_01() noexcept
{
    sd_card_t*  pSD = sd_get_by_num(0);
    FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if(fr == FR_OK) {
        uld::target  l_target(EM_ARM, ELFCLASS32, true, true);
        uld::image   l_image(std::addressof(l_target));
        if(l_image.load("localcall.o")) {
            uld::symbol_t* l_data_ptr = l_image.find_symbol("data");
            if(l_data_ptr) {
                printf("%s (before):\n", l_data_ptr->name);
                printf("    type: %.8x\n", l_data_ptr->type);
                printf("    flags: %.8x\n", l_data_ptr->flags);
                printf("    size: %d\n", l_data_ptr->size);
                printf("    address: %p\n", l_data_ptr->ea);
                if(l_data_ptr->ea != nullptr) {
                    // set all zero bytes to 255
                    {
                        std::uint8_t* l_data_base = l_data_ptr->ea;
                        std::uint8_t* l_data_tail = l_data_ptr->ea + l_data_ptr->size;
                        for(std::uint8_t* i_data = l_data_base; i_data != l_data_tail; i_data++) {
                            if(*i_data == 0) {
                                *i_data = 255;
                            }
                        }
                    }
                    // dump data
                    printf("---\n");
                    dbg_dump_hex(l_data_ptr->ea, l_data_ptr->size);
                    printf("---\n");
                }
            }
            if(uld::symbol_t*
                l_call_ptr = l_image.find_symbol("clear");
                l_call_ptr != nullptr) {
                printf("%s():\n", l_call_ptr->name);
                printf("    type: %.8x\n", l_call_ptr->type);
                printf("    flags: %.8x\n", l_call_ptr->flags);
                printf("    size: %d\n", l_call_ptr->size);
                printf("    address: %p\n", l_call_ptr->ra);
                if(l_call_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_call_ptr->ea, l_call_ptr->size);
                    printf("---\n");
                }
            }
            if(uld::symbol_t*
                l_call_ptr = l_image.find_symbol("fill");
                l_call_ptr != nullptr) {
                printf("%s():\n", l_call_ptr->name);
                printf("    type: %.8x\n", l_call_ptr->type);
                printf("    flags: %.8x\n", l_call_ptr->flags);
                printf("    size: %d\n", l_call_ptr->size);
                printf("    address: %p\n", l_call_ptr->ra);
                if(l_call_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_call_ptr->ea, l_call_ptr->size);
                    printf("---\n");
                }
            }
            if(uld::symbol_t*
                l_call_ptr = l_image.find_symbol("run");
                l_call_ptr != nullptr) {
                void (*p_call)();
                printf("%s():\n", l_call_ptr->name);
                printf("    type: %.8x\n", l_call_ptr->type);
                printf("    flags: %.8x\n", l_call_ptr->flags);
                printf("    size: %d\n", l_call_ptr->size);
                printf("    address: %p\n", l_call_ptr->ra);
                if(l_call_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_call_ptr->ea, l_call_ptr->size);
                    printf("---\n");
                }
                p_call = reinterpret_cast<void(*)()>(l_call_ptr->ra);
                p_call();
            }
            if(l_data_ptr) {
                printf("%s (after):\n", l_data_ptr->name);
                if(l_data_ptr->ea != nullptr) {
                    printf("---\n");
                    dbg_dump_hex(l_data_ptr->ea, l_data_ptr->size);
                    printf("---\n");
                }
            }
        }
        f_unmount(pSD->pcName);
    }
}

int   main(int, char**)
{
      time_init();
      stdio_init_all();

      test_dummy_01();
      test_dummy_02();
      // test_cache_01();
      // test_pool_01();
      // // test_pool_02();
      // // test_elf_sumxy_01();
      // test_elf_sumxy_02();
      // test_elf_sumxy_03();
      // test_elf_sumxy_04();
      // test_elf_data_01();
      // test_elf_data_02();
      test_elf_call_01();

      return 0;
}
