#include "pico.h"
#include <stdio.h>
#include <blackops.h>
#include <elf.h>
#include <uld.h>
#include <uld/target.h>
#include <uld/image.h>
#include "share/function.h"
#include "share/sys.h"
#include "share/string.h"
#include "share/file.h"
#include "hw_config.h"
#include <dbg.h>

int   idiv(int lhs, int rhs)
{
      return lhs / rhs;
}

int   imod(int lhs, int rhs)
{
      return lhs % rhs;
}

void  user_abort() noexcept
{
      printf("/!\\ CAUGHT: abort() from user program.\n");
}

void  user_exit(int rc) noexcept
{
      printf("/!\\ CAUGHT: exit(%d) from user program.\n", rc);
}

void  user_signal(int signal, void* data) noexcept
{
      printf("/!\\ CAUGHT: signal(%d, %p) from user program.\n", signal, data);
}

void  exec_round_01() noexcept
{
      uld::target l_target(EM_ARM, ELFCLASS32, true, true);
      uld::image  l_image(std::addressof(l_target));

      l_image.make_symbol("__aeabi_idiv", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)idiv);
      l_image.make_symbol("__aeabi_idivmod", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)imod);

      l_image.make_symbol("malloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)malloc);
      l_image.make_symbol("realloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)realloc);
      l_image.make_symbol("memcpy", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)memcpy);
      l_image.make_symbol("memset", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)memset);
      l_image.make_symbol("free", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)free);

      l_image.make_symbol("sprintf", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)sprintf);
      l_image.make_symbol("snprintf", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)snprintf);

      l_image.make_symbol("open", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)open);
      l_image.make_symbol("read", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)read);
      l_image.make_symbol("write", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)write);
      l_image.make_symbol("seek", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)seek);
      l_image.make_symbol("tell", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)tell);
      l_image.make_symbol("eof", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)eof);
      l_image.make_symbol("close", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)close);

      l_image.make_symbol("signal", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_signal);
      l_image.make_symbol("abort", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_abort);
      l_image.make_symbol("exit", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_exit);
      if(l_image.load("main.01.o")) {
          if(auto
              l_call_ptr = get_function<bool>(l_image, "test_01");
              l_call_ptr != nullptr) {
              l_call_ptr();
          }
          if(auto
              l_call_ptr = get_function<bool>(l_image, "test_02");
              l_call_ptr != nullptr) {
              l_call_ptr();
          }
          if(auto
              l_call_ptr = get_function<bool>(l_image, "test_03");
              l_call_ptr != nullptr) {
              l_call_ptr();
          }
          if(auto
              l_call_ptr = get_function<char*>(l_image, "test_04");
              l_call_ptr != nullptr) {
              char* message = l_call_ptr();
              printf("--- message=%p \"%s\"\n", message, message);
          }
          if(auto
              l_call_ptr = get_function<char*, int>(l_image, "test_05");
              l_call_ptr != nullptr) {
              char* message = l_call_ptr('a');
              printf("--- message=%p \"%s\"\n", message, message);
              dbg_dump_hex(message, 64, true);
              printf("---\n");
          }
          if(auto
              l_call_ptr = get_function<char*, int>(l_image, "test_06");
              l_call_ptr != nullptr) {
              char* message = l_call_ptr(192);
              printf("--- message=%p \"%s\"\n", message, message);
              dbg_dump_hex(message, 64, true);
              printf("---\n");
          }
          if(auto
              l_call_ptr = get_function<bool, std::uint8_t*, int>(l_image, "test_33");
              l_call_ptr != nullptr) {
              std::uint8_t* l_data_ptr = reinterpret_cast<std::uint8_t*>(malloc(256));
              l_call_ptr(l_data_ptr, 256);
              dbg_dump_hex(l_data_ptr, 256);
          }
          if(auto
              l_call_ptr = get_function<int, int, int>(l_image, "test_divxy");
              l_call_ptr != nullptr) {
              int result = l_call_ptr(8, 2);
              printf("--- result=%d\n", result);
          }
          if(auto
              l_call_ptr = get_function<int, int, int>(l_image, "test_modxy");
              l_call_ptr != nullptr) {
              int result = l_call_ptr(10, 2);
              printf("--- result=%d\n", result);
          }
          if(auto
              l_call_ptr = get_function<int>(l_image, "main");
              l_call_ptr != nullptr) {
              int l_rc = l_call_ptr();
              printf("    [%s] rc=%d\n", __PRETTY_FUNCTION__, l_rc);
          }
      } else
          printf("--- EXEC ROUND 01: FAILED\n");
}

void  exec_round_02() noexcept
{
      uld::target l_target(EM_ARM, ELFCLASS32, true, true);
      uld::image  l_image(std::addressof(l_target));

      l_image.make_symbol("malloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)malloc);
      l_image.make_symbol("realloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)realloc);
      l_image.make_symbol("memcpy", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)memcpy);
      l_image.make_symbol("memset", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)memset);
      l_image.make_symbol("free", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)free);

      l_image.make_symbol("sprintf", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)sys_sprintf);
      l_image.make_symbol("snprintf", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)sys_snprintf);

      l_image.make_symbol("open", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)open);
      l_image.make_symbol("read", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)read);
      l_image.make_symbol("write", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)write);
      l_image.make_symbol("seek", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)seek);
      l_image.make_symbol("tell", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)tell);
      l_image.make_symbol("eof", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)eof);
      l_image.make_symbol("close", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)close);

      l_image.make_symbol("signal", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_signal);
      l_image.make_symbol("abort", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_abort);
      l_image.make_symbol("exit", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_exit);
      if(l_image.load("main.02.o")) {
          if(auto
              l_call_ptr = get_function<char*>(l_image, "test_01");
              l_call_ptr != nullptr) {
              char* message = l_call_ptr();
              snprintf(message, 64, "value=%d", 128);
              printf("--- message=%p \"%s\"\n", message, message);
          }
          if(auto
              l_call_ptr = get_function<char*>(l_image, "test_02");
              l_call_ptr != nullptr) {
              char* message = l_call_ptr();
              printf("---\n");
              dbg_dump_hex(message, 64, true);
              printf("---\n");
          }
      } else
          printf("--- EXEC ROUND 02: FAILED\n");
}

int   main(int, char**)
{
      initialise();
      sd_card_t*  pSD = sd_get_by_num(0);
      FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
      if(fr == FR_OK) {
          exec_round_01();
          exec_round_02();
          printf("bye.\n");
          f_unmount(pSD->pcName);
      }
      dispose();
      return 0;
}
