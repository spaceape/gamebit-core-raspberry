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

std::uint8_t  PatternTable[1024];
std::uint8_t  NameTable[1024];
std::uint8_t  AttrTable[1024];
std::uint16_t PaletteTable[1024];
std::uint8_t  Scratch[4096];

template<typename Rt, typename... Args>
struct function
{
  using type = Rt(*)(Args...);
};

template<typename Rt, typename... Args>
function<Rt, Args...>::type get_function(uld::image& image, const char* name) noexcept
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

void gbPanic(uint32_t code, const char* message)
{
}

uint32_t gbTime()
{
	return 0;
}

void gbRandomSeed(uint32_t seed)
{ 
}

uint32_t gbRandom()
{
    return 0;
}

int gbAppInit()
{
    return 0;
}

void gbAppFree()
{
}

bool gbAppRunning()
{
      return true;
}

void gbAppClose()
{
}

void gbPatternData(uint8_t index, uint8_t count, const uint8_t* data)
{
}

void gbDrawSprite(uint8_t id, uint8_t palette, int16_t x, int16_t y, uint32_t flags)
{
}

void gbPaletteData(uint8_t id, uint8_t count, const uint32_t* colors)
{
}

void gbDrawTileNames(const uint8_t* names, uint16_t countX, uint16_t countY)
{
}

void gbDrawTileAttributes(const uint8_t* palettes, uint16_t countX, uint16_t countY)
{
}

void gbDrawChar(int16_t x, int16_t y, uint8_t palette, char c) {
}

void gbDrawText(int16_t x, int16_t y, uint8_t palette, const char* format, ...)
{
}

void gbDrawText2(int16_t x, int16_t y, uint8_t palette, const uint8_t* text)
{
}

bool gbGetButtonState(uint8_t button)
{
     return false;
}

bool gbGetButtonDelta(uint8_t button)
{
     return false;
}

void  boot(const char* game) noexcept
{
      uld::target    target(EM_ARM, ELFCLASS32, true, true);
      uld::image     image(std::addressof(target));

      image.make_symbol("PatternTable", uld::symbol_t::type_object, uld::symbol_t::bind_global, PatternTable);
      image.make_symbol("NameTable",    uld::symbol_t::type_object, uld::symbol_t::bind_global, NameTable);
      image.make_symbol("AttrTable",    uld::symbol_t::type_object, uld::symbol_t::bind_global, AttrTable);
      image.make_symbol("PaletteTable", uld::symbol_t::type_object, uld::symbol_t::bind_global, PaletteTable);
      image.make_symbol("GlobalMem",    uld::symbol_t::type_object, uld::symbol_t::bind_global, Scratch);

    //    image.make_symbol("print", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(print));
            
      image.make_symbol("gbAppInit", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbAppInit));
    //    image.make_symbol("gbAppRunning", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbAppRunning));
      image.make_symbol("gbAppClose", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbAppClose));
    //    image.make_symbol("gbPanic", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbPanic));
      image.make_symbol("gbRandomSeed", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbRandomSeed));
      image.make_symbol("gbRandom", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbRandom));
      image.make_symbol("gbTime", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbTime));
    //    image.make_symbol("gbSetTransparentColor", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbSetTransparentColor));
    //    image.make_symbol("gbClear", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbClear));
    //    image.make_symbol("gbSwap", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbSwap));
      image.make_symbol("gbPatternData", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbPatternData));
      image.make_symbol("gbDrawSprite", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbDrawSprite));
      image.make_symbol("gbPaletteData", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbPaletteData));
      image.make_symbol("gbDrawChar", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbDrawChar));
      image.make_symbol("gbDrawText", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbDrawText));
    //    image.make_symbol("gbDrawTileNames", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbDrawTileNames));
    //    image.make_symbol("gbDrawTileAttributes", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbDrawTileAttributes));
      image.make_symbol("gbGetButtonState", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbGetButtonState));
      image.make_symbol("gbGetButtonDelta", uld::symbol_t::type_function, uld::symbol_t::bind_global, (void*)std::addressof(gbGetButtonDelta));

      if(image.load(game)) {
          auto init = get_function<int>(image, "init");
          if(init == nullptr) {
              return;
          }
          auto update = get_function<void>(image, "update");
          if(update == nullptr) {
              return;
          }
          init();
          update();
      }
}

int   main(int, char**)
{
      time_init();
      stdio_init_all();
      sd_card_t*  pSD = sd_get_by_num(0);
      FRESULT     fr  = f_mount(&pSD->fatfs, pSD->pcName, 1);
      if(fr == FR_OK) {
          boot("tetris.o");
          printf("bye.\n");
          f_unmount(pSD->pcName);
      }
      return 0;
}
