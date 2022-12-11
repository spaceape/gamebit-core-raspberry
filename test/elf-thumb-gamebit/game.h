#ifndef game_h
#define game_h
#include <stdint.h>
#include <stdbool.h>

#ifndef MIN
#define MIN(a, b) ((a < b) ? a : b)
#endif
#ifndef MAX
#define MAX(a, b) ((a > b) ? a : b)
#endif

#define GB_SCREEN_WIDTH   320
#define GB_SCREEN_HEIGHT  240

#define GB_2BPP           0x01
#define GB_8BPP           0x02

// gbClear flags
#define GB_NAME_BIT       (1 << 0)
#define GB_ATTR_BIT       (1 << 1)
#define GB_SPRT_BIT       (1 << 2)
#define GB_TILE_BIT       (GB_NAME_BIT | GB_ATTR_BIT)
#define GB_FULL_BIT       (GB_TILE_BIT | GB_SPRT_BIT)

// Sprite draw flags
#define SF_NONE           0
#define SF_FLIP_X         (1 << 0)
#define SF_FLIP_Y         (1 << 1)
#define SF_SWAP           (1 << 2)
#define SF_UNUSED         (1 << 3)
#define SF_ROT_90         (SF_FLIP_X | SF_SWAP)
#define SF_ROT_180        (SF_FLIP_Y | SF_SWAP)
#define SF_ROT_270        (SF_FLIP_X | SF_FLIP_Y)

// Gampad buttons
#define BTN_LEFT          0x00
#define BTN_DOWN          0x01
#define BTN_RIGHT         0x02
#define BTN_UP            0x03
#define BTN_START         0x04
#define BTN_SELECT        0x05
#define BTN_B             0x06
#define BTN_A             0x07
#define BTN_COUNT         0x08

#define	FA_READ           0x01
#define	FA_WRITE          0x02
#define	FA_OPEN_EXISTING  0x00
#define	FA_CREATE_NEW     0x04
#define	FA_CREATE_ALWAYS  0x08
#define	FA_OPEN_ALWAYS    0x10
#define	FA_OPEN_APPEND    0x30

extern uint8_t PatternTable[];
extern uint8_t NameTable[];
extern uint8_t AttrTable[];
extern uint16_t PaletteTable[];
extern uint8_t GlobalMem[];

/*
  System
*/
int gbAppInit() __attribute__((long_call));
bool gbAppRunning() __attribute__((long_call));
void gbAppClose() __attribute__((long_call));
void gbAppFree()  __attribute__((long_call));
void gbPanic(uint32_t code, const char* message) __attribute__((long_call));
void gbRandomSeed(uint32_t) __attribute__((long_call));
uint32_t gbRandom() __attribute__((long_call));
uint32_t gbTime()  __attribute__((long_call));

/*
  Tile renderer
*/
void gbSetTransparentColor(uint8_t red, uint8_t green, uint8_t blue)  __attribute__((long_call));
void gbClear(uint8_t mask) __attribute__((long_call));
void gbSwap() __attribute__((long_call));
void gbPatternData(uint8_t id, uint8_t count, const uint8_t* data) __attribute__((long_call));
void gbPatternData2(uint8_t id, uint8_t count, const uint8_t* data) __attribute__((long_call));
void gbDrawSprite(uint8_t id, uint8_t palette, int16_t x, int16_t y, uint32_t flags) __attribute__((long_call));
void gbPaletteData(uint8_t id, uint8_t count, const uint32_t* colors) __attribute__((long_call));
void gbDrawChar(int16_t x, int16_t y, uint8_t palette, char c) __attribute__((long_call));
void gbDrawText(int16_t x, int16_t y, uint8_t palette, const char* format, ...) __attribute__((long_call));
void gbDrawTileNames(const uint8_t* names, uint16_t countX, uint16_t countY) __attribute__((long_call));
void gbDrawTileAttributes(const uint8_t* attributes, uint16_t countX, uint16_t countY) __attribute__((long_call));

/*
  Gamepad
*/
bool gbGetButtonState(uint8_t) __attribute__((long_call));
bool gbGetButtonDelta(uint8_t) __attribute__((long_call));
#define gbButtonPressed(button) (gbGetButtonDelta(button) && gbGetButtonState(button))
#define gbButtonReleased(button) (gbGetButtonDelta(button) && !gbGetButtonState(button))

/*
  SDCard filesystem
*/
int gbFileOpen(int fd, const char* path, int flags) __attribute__((long_call));
int gbFileClose(int fd) __attribute__((long_call)); 
uint32_t gbFileSeek(int fd, uint32_t offset, int position) __attribute__((long_call));
uint32_t gbFileTell(int fd) __attribute__((long_call));
uint32_t gbFileSize(int fd) __attribute__((long_call));
uint32_t gbFileRead(int fd, void* buffer, uint32_t length) __attribute__((long_call));
uint32_t gbFileWrite(int fd, void* buffer, uint32_t length) __attribute__((long_call));
uint32_t gbGetSDCardFileCount() __attribute__((long_call));
bool gbGetSDCardFileName(char* filename, uint32_t size, uint32_t index) __attribute__((long_call));

// Utilitity functions
#define GB_SWAP_U16(value) ((((value) >> 8 ) | ((value) << 8)) & 0xFFFF)
#define GB_RGB_TO_U32(red, green, blue) ((uint32_t)(0xFF000000 | ((red) & 0xFF) << 16 | ((green) & 0xFF) << 8 | ((blue) & 0xFF) << 0))
#define GB_RGB_TO_U16(red, green, blue) GB_U32_TO_U16(GB_RGB_TO_U32((red), (green), (blue)))
#define GB_U32_TO_U16(value) ((uint16_t)((value & 0x00F80000) >> 8 | ( value & 0x0000FC00) >> 5 | ( value & 0x000000F8) >> 3))
#define GB_U16_TO_U32(value) ((uint32_t)(0xFF000000 | (((value & 0xF800) << 3 | (value & 0xF800) >> 2) << 8) | ((value & 0x07E0) << 5)| ((value & 0x001F) << 3)))
#define GB_RGB GB_RGB_TO_U32

#endif
