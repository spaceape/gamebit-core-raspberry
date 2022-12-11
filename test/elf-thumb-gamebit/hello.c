#include "game.h"
#include "eabi.h"

const uint32_t Palettes[4 * 7] = {
	// 0 - Red
    GB_RGB(  0,   0,   0),
    GB_RGB(255,   0,   0),
    GB_RGB(  0,   0,   0),
    GB_RGB(  0,   0,   0),
    // 1 - Yellow
    GB_RGB(  0,   0,   0),
    GB_RGB(255, 255,   0),
    GB_RGB(  0,   0,   0),
    GB_RGB(  0,   0,   0),
    // 2 - Green
    GB_RGB(  0,   0,   0),
    GB_RGB(  0, 255,   0),
    GB_RGB(  0,   0,   0),
    GB_RGB(  0,   0,   0),
    // 3 - Teal
    GB_RGB(  0,   0,   0),
    GB_RGB(  0, 255, 255),
    GB_RGB(  0,   0,   0),
    GB_RGB(  0,   0,   0),
	// 4 - Blue
    GB_RGB(  0,   0,   0),
    GB_RGB(  0,   0, 255),
    GB_RGB(  0,   0,   0),
    GB_RGB(  0,   0,   0),
    // 5 - Purble
    GB_RGB(  0,   0,   0),
    GB_RGB(255,   0, 255),
    GB_RGB(  0,   0,   0),
    GB_RGB(  0,   0,   0),
    // 6 - White
    GB_RGB(  0,   0,   0),
    GB_RGB(255, 255, 255),
    GB_RGB(  0,   0,   0),
    GB_RGB(  0,   0,   0),
};

const char Message1[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\0'};
// const char Message2[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\0'};
const char Message2[] = "Hello World!";
const char* Message3 = "Hello World!"; // Not working
const char* Message4 = Message2;

const char FORMAT_MSG[] = "%s %d";
const char FORMAT_TIME[] = "%.2u:%.2u";

//#define BUG_UNABLE_TO_MAP
#ifdef BUG_UNABLE_TO_MAP
uint32_t ExitTimer;
#else
uint32_t ExitTimer = 0;
#endif

int init() {
    gbPaletteData(0, 7, Palettes);
    return 0;
}

void update() {
	if (gbButtonPressed(BTN_B)) {
		ExitTimer = gbTime();
	}
	if (gbGetButtonState(BTN_B) && (gbTime() >= ExitTimer + 1000)) {
		gbAppClose();
		return;
	}

//#define BUG_OUT_OF_MEMORY
#ifdef BUG_OUT_OF_MEMORY
	const char Message5[] = "Hello World!";
	const char* Message6 = "Hello World!";
#endif

	int16_t y = 0;

	y += 1;
	gbDrawText(14, y += 1, 0, (const char*)Message1);
	gbDrawText(14, y += 1, 1, (const char*)Message2);
	gbDrawText(14, y += 1, 2, (const char*)Message3);
	gbDrawText(14, y += 1, 3, (const char*)Message4);
#ifdef BUG_OUT_OF_MEMORY
	gbDrawText(14, y += 1, 4, (const char*)Message5);
	gbDrawText(14, y += 1, 5, (const char*)Message6);
#endif

	y += 1;
	gbDrawText(14, y += 1, 0, "%s %d", Message1, 1);
	gbDrawText(14, y += 1, 1, "%s %d", Message2, 2);
	gbDrawText(14, y += 1, 2, "%s %d", Message3, 3);
	gbDrawText(14, y += 1, 3, "%s %d", Message4, 4);
#ifdef BUG_OUT_OF_MEMORY
	gbDrawText(14, y += 1, 4, "%s %d", Message5, 5);
	gbDrawText(14, y += 1, 5, "%s %d", Message6, 6);
#endif

	y += 1;
	gbDrawText(14, y += 1, 0, FORMAT_MSG, Message1, 1);
	gbDrawText(14, y += 1, 1, FORMAT_MSG, Message2, 2);
	gbDrawText(14, y += 1, 2, FORMAT_MSG, Message3, 3);
	gbDrawText(14, y += 1, 3, FORMAT_MSG, Message4, 4);
#ifdef BUG_OUT_OF_MEMORY
	gbDrawText(14, y += 1, 4, FORMAT_MSG, Message5, 5);
	gbDrawText(14, y += 1, 5, FORMAT_MSG, Message6, 6);
#endif

	const uint32_t ms = gbTime();
	const uint32_t total_sec = ms / 1000;
	const uint32_t min = total_sec / 60;
//#define BUG_OP_MOD
#ifdef BUG_OP_MOD
	uint32_t sec = total_sec % 60;
#else
	uint32_t sec = total_sec;
	while (sec >= 60) {
		sec -= 60;
	}	
#endif
	gbDrawText(14, y += 1, 6, FORMAT_TIME, min, sec);
}
