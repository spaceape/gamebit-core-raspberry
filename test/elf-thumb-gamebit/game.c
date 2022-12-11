#include "game.h"

// #define GB_RGB_TO_U32(red, green, blue) ((uint32_t)(0xFF000000 | ((red) & 0xFF) << 16 | ((green) & 0xFF) << 8 | ((blue) & 0xFF) << 0))
// #define GB_RGB_TO_U16(red, green, blue) GB_U32_TO_U16(GB_RGB_TO_U32((red), (green), (blue)))
// #define GB_U32_TO_U16(value) ((uint16_t)((value & 0x00F80000) >> 8 | ( value & 0x0000FC00) >> 5 | ( value & 0x000000F8) >> 3))
// #define GB_U16_TO_U32(value) ((uint32_t)(0xFF000000 | (((value & 0xF800) << 3 | (value & 0xF800) >> 2) << 8) | ((value & 0x07E0) << 5)| ((value & 0x001F) << 3)))

const uint8_t Patterns[8 * 8 * 1] = {
    // Brick wall pattern
    1,1,1,1,1,1,1,0,
    1,1,1,1,1,1,1,0,
    1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    1,1,1,0,1,1,1,1,
    1,1,1,0,1,1,1,1,
    1,1,1,0,1,1,1,1,
    0,0,0,0,0,0,0,0,
};

const uint16_t Palettes[4 * 1] = {
    // Brick wall palette
    GB_RGB( 10,  10,  10),
    GB_RGB(150,  70,  40),
    GB_RGB(  0,   0,   0),
    GB_RGB(  0,   0,   0),
};

void ConvertU8toU2(uint8_t* dst, const uint8_t* src, uint16_t length)
{
    for (uint16_t index = 0; index < length / 4; index += 1) {
        dst[index] = ((src[index * 4 + 0] & 0x3) << 6) | 
                     ((src[index * 4 + 1] & 0x3) << 4) | 
                     ((src[index * 4 + 2] & 0x3) << 2) | 
                     ((src[index * 4 + 3] & 0x3) << 0);
    }
}

void UploadU8PatternData(uint8_t index, uint8_t count, const uint8_t* data)
{
    ConvertU8toU2(PatternTable + index * 16, data, MIN(count, 255 - index)); // 16 bytes per pattern
}

uint8_t getNameIndex(uint32_t time)
{
    return time % 16;
}

void UpdateNameTable(uint32_t time)
{
    // Fill the nametable with the brick wall pattern
    for (uint8_t y = 0; y < 30; ++ y) {
        for (uint8_t x = 0; x < 40; ++x) {
            const uint16_t index = y * 40 + x;
            NameTable[index] = getNameIndex(time); // 0 is used to specify an empty tile
        }
    }
}

void UpdateAttrTable()
{
    // Fill the nametable with the brick wall pattern
    for (uint8_t y = 0; y < 30; ++ y) {
        for (uint8_t x = 0; x < 40; ++x) {
            const uint16_t index = y * 40 + x;
            AttrTable[index] = 0; // 0 is used to specify an empty tile
        }
    }
}

// This function will be called from the kernel space
int init()
{
    // Make sure the tables were linked
    // if (PatternTable == NULL) {
    //     return 1;
    // }
    // if (NameTable == NULL) {
    //     return 2;
    // }
    // if (AttrTable == NULL) {
    //     return 3;
    // }
    // if (PaletteTable == NULL) {
    //     return 4;
    // }
 
    // Setup pattern tables
    UploadU8PatternData(0, 64, Patterns);
 
    // Setup palette tables
    for (uint16_t index = 0; index < 4; ++index) {
        PaletteTable[index] = Palettes[index];
    }
    return 0;
}
 
// This function will be called from the kernel space
// Params:
//      time - milliseconds since console is running
void update(uint32_t time)
{
      UpdateNameTable(time);
      UpdateAttrTable();
}
 
// This function will be called from the kernel space
int uninit()
{
    return 0;
}
