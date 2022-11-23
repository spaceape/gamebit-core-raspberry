#ifndef game_h
#define game_h
#include <stdint.h>
#include <stdlib.h>

extern uint8_t PatternTable[];
extern uint8_t NameTable[];
extern uint8_t AttrTable[];
extern uint16_t PaletteTable[];

#ifndef MIN
#define MIN(a, b) ((a < b) ? a : b)
#endif
#ifndef MAX
#define MAX(a, b) ((a > b) ? a : b)
#endif

#endif
