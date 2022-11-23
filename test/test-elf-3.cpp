#include "pico.h"
#include <stdio.h>
#include <blackops.h>
#include <elf.h>
#include <uld.h>
#include <uld/target.h>
#include <uld/image.h>
#include "hw_config.h"
#include <dbg.h>

template<typename Rt, typename... Args>
struct function
{
  using type = Rt(*)(Args...);
};

template<typename Rt, typename... Args>
function<Rt, Args...>::type get_function(const char* name, uld::image& image) noexcept
{
      if(uld::symbol_t*
          l_symbol_ptr = image.find_symbol(name);
          l_symbol_ptr != nullptr) {
          if(l_symbol_ptr->ra != nullptr) {
              return (typename function<Rt, Args...>::type)l_symbol_ptr->ra;
          } else
              printf("/!\\ FAILED: %s %d: Symbol `%s` is undefined\n", __FILE__, __LINE__, name);
      } else
          printf("/!\\ FAILED: %s %d: Symbol `%s` not found.\n", __FILE__, __LINE__, name);
      return nullptr;
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
      l_image.make_symbol("malloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)malloc);
      l_image.make_symbol("realloc", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)realloc);
      l_image.make_symbol("free", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)free);
      l_image.make_symbol("memcpy", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)memcpy);
      l_image.make_symbol("signal", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_signal);
      l_image.make_symbol("abort", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_abort);
      l_image.make_symbol("exit", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)user_exit);
      if(l_image.load("main.01.o")) {
          if(auto
              l_call_ptr = get_function<bool>("test_01", l_image);
              l_call_ptr != nullptr) {
              l_call_ptr();
          }
          if(auto
              l_call_ptr = get_function<bool>("test_02", l_image);
              l_call_ptr != nullptr) {
              l_call_ptr();
          }
          if(auto
              l_call_ptr = get_function<bool>("test_03", l_image);
              l_call_ptr != nullptr) {
              l_call_ptr();
          }
          if(auto
              l_call_ptr = get_function<bool, std::uint8_t*, int>("test_33", l_image);
              l_call_ptr != nullptr) {
              std::uint8_t* l_data_ptr = reinterpret_cast<std::uint8_t*>(malloc(256));
              l_call_ptr(l_data_ptr, 256);
              dbg_dump_hex(l_data_ptr, 256);
          }
          if(auto
              l_call_ptr = get_function<int>("main", l_image);
              l_call_ptr != nullptr) {
              int l_rc = l_call_ptr();
              printf("    [%s] rc=%d\n", __PRETTY_FUNCTION__, l_rc);
          }
      } else
          printf("--- EXEC ROUND 01: FAILED\n");
}

int   main(int, char**)
{
      initialise();
      sd_card_t*  pSD = sd_get_by_num(0);
      FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
      if(fr == FR_OK) {
          exec_round_01();
          printf("bye.\n");
          f_unmount(pSD->pcName);
      }
      dispose();
      return 0;
}
