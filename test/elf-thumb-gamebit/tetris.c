#include "tetris.h"

Tetris tetris;
uint32_t ExitTime;

const char TEXT_PAUSE[]    = "Pause";
const char TEXT_GAMEOVER[] = "Game over!";
//const char TEXT_START[]    = "Press (START) to start.";
//const char TEXT_RESTART[]  = "Press (START) to restart";
const char TEXT_SCORE[]    = " Score  ";
const char TEXT_HISCORE[]  = " Hiscore";

const char TEXT_MENU[3][6] = {
	"Start",
	"Level",
	"Trace",
};
const uint8_t MENU_TEXT_COUNT = sizeof(TEXT_MENU) / sizeof(TEXT_MENU[0]);

const char TEXT_ON[]      = "on";
const char TEXT_OFF[]     = "off";

const char TEXT_NEXT[]     = " Next   ";
const char TEXT_EXITING[]  = "Exiting...";

const char FORMAT_MENU[] = "%c %s";
const char FORMAT_LEVEL[]  = "%.2u";
const char FORMAT_TRACE[]  = "%s";
const char FORMAT_EXIT[]   = "%.*s";
const char FORMAT_SCORE[]  = " %.6u ";

const uint8_t SPLASH_SCREEN[] = {
	0,1,1,1,2,2,2,3,3,3,4,4,0,0,5,0,6,6,6,0,
	0,0,1,0,2,0,0,0,3,0,4,0,4,0,5,0,6,0,0,0,
	0,0,1,0,2,2,0,0,3,0,4,4,0,0,5,0,6,6,6,0,
	0,0,1,0,2,0,0,0,3,0,4,0,4,0,5,0,0,0,6,0,
	0,0,1,0,2,2,2,0,3,0,4,0,4,0,5,0,6,6,6,0,

};

void gb_memset(void* dst, uint8_t value, uint32_t num) {
	uint8_t* ptr = (uint8_t*)dst;
	for (uint32_t index = 0; index < num; ++index) {
		ptr[index] = value;
	}
}

void gb_memcpy(void* dst, const void* src, uint32_t count) {
	uint8_t* cdst = (uint8_t*)dst;
	const uint8_t* csrc = (const uint8_t*)src;
	for (uint32_t index = 0; index < count; ++index) {
		cdst[index] = csrc[index];
	}
}

const uint32_t Palettes[] = {
	// 0 - Gray block
	GB_RGB( 50,  50,  50),
	GB_RGB(200, 200, 200),
	GB_RGB(120, 120, 120),
	GB_RGB(  0,   0,   0),

	// 1 - Red
	GB_RGB(100,  25,  25),
	GB_RGB(200,  50,  50),
	GB_RGB(255,  64,  64),
	GB_RGB(  0,   0,   0),
	
	// 2 - Yellow
	GB_RGB(100, 100,  25),
	GB_RGB(200, 200,  50),
	GB_RGB(255, 255,  64),
	GB_RGB(  0,   0,   0),

	// 3 - Green
	GB_RGB( 25, 100,  25),
	GB_RGB( 50, 200,  50),
	GB_RGB( 64, 255,  64),
	GB_RGB(  0,   0,   0),
	
	// 4 - Teal
	GB_RGB( 25, 100, 100),
	GB_RGB( 50, 200, 200),
	GB_RGB( 64, 255, 255),
	GB_RGB(  0,   0,   0),

	// 5 - Blue
	GB_RGB( 25,  25, 100),
	GB_RGB( 50,  50, 200),
	GB_RGB( 64,  64, 255),
	GB_RGB(  0,   0,   0),

	// 6 - Violet
	GB_RGB(100,  25, 100),
	GB_RGB(200,  50, 200),
	GB_RGB(255,  64, 255),
	GB_RGB(  0,   0,   0),

	// 7 - Black/White Font
	GB_RGB(  0,   0,   0),
	GB_RGB(255, 255, 255),
	GB_RGB(  0,   0,   0),
	GB_RGB(  0,   0,   0),

	// 8 - Blue brick wall
	GB_RGB(  0,   0,   0),
	GB_RGB(  0,  50, 100),
	GB_RGB(  0,   0,   0),
	GB_RGB(  0,   0,   0),
};

#define COLOR_GRAY    0
#define COLOR_RED     1
#define COLOR_YELLOW  2
#define COLOR_GREEN   3
#define COLOR_CYAN    4
#define COLOR_BLUE    5
#define COLOR_VIOLET  6
#define COLOR_FONT    7
#define COLOR_BRICK   8


//const uint8_t PaletteCount = sizeof(Palettes) / 16;
#define PaletteCount (sizeof(Palettes) / 16)

const uint8_t Patterns[] = {
	// Brick wall
	1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,
	1,1,1,0,1,1,1,1,
	1,1,1,0,1,1,1,1,
	1,1,1,0,1,1,1,1,
	0,0,0,0,0,0,0,0,

	// Brick top left
	1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,1,
	0,0,0,2,2,2,2,2,
	0,0,0,2,2,2,2,2,
	0,0,0,2,2,2,2,2,
	0,0,0,2,2,2,2,2,
	0,0,0,2,2,2,2,2,

	// Brick top right
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	2,2,2,2,2,1,1,1,
	2,2,2,2,2,1,1,1,
	2,2,2,2,2,1,1,1,
	2,2,2,2,2,1,1,1,
	2,2,2,2,2,1,1,1,

	// Brick bottom left
	0,0,0,2,2,2,2,2,
	0,0,0,2,2,2,2,2,
	0,0,0,2,2,2,2,2,
	0,0,0,2,2,2,2,2,
	0,0,0,2,2,2,2,2,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,

	// Brick bottom right
	2,2,2,2,2,1,1,1,
	2,2,2,2,2,1,1,1,
	2,2,2,2,2,1,1,1,
	2,2,2,2,2,1,1,1,
	2,2,2,2,2,1,1,1,
	0,0,0,0,0,0,1,1,
	0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,
	
	// Right arrow
	0,0,1,1,0,0,0,0,
	0,0,1,1,1,0,0,0,
	0,0,1,1,1,1,0,0,
	0,0,1,1,1,1,1,0,
	0,0,1,1,1,1,1,0,
	0,0,1,1,1,1,0,0,
	0,0,1,1,1,0,0,0,
	0,0,1,1,0,0,0,0,
};
//const uint8_t PatternCount = sizeof(Patterns) / 64;
#define PatternCount (sizeof(Patterns) / 64)

const Piece Pieces[PIECE_COUNT] = {
	{
		.length = 3,
		.score = 3,
		.data = {
			0, 0, 0, 0,
			0, 1, 1, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
		},
	},
	{
		.length = 3,
		.score = 8,
		.data = {
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
		},
	},
	{
		.length = 3,
		.score = 2,
		.data = {
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
		},
	},
	{
		.length = 3,
		.score = 6,
		.data = {
			0, 1, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
		},
	},
	{
		.length = 3,
		.score = 7,
		.data = {
			0, 0, 1, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
		},
	},
	{
		.length = 3,
		.score = 5,
		.data = {
			1, 0, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
		},
	},
	{
		.length = 4,
		.score = 2,
		.data = {
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
		},
	},

	#warning extra I piece for mod operator
	{
		.length = 4,
		.score = 2,
		.data = {
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
		},
	},
};

const uint8_t LevelTemplate[] = {
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,2,1,1,1,1,
	1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,
};

bool CheckCollision(int16_t nx, int16_t ny, const uint8_t* pieceData) {
	if (pieceData == NULL) {
		pieceData = tetris.piece;
	}

	const int16_t px = nx >> 1;
	const int16_t py = ny >> 1;

	for (int16_t y = 0; y < 4; ++y) {
		if (py + y < 0) {
			return true;
		}

		for (int16_t x = 0; x < 4; ++x) {
			if (px + x < 0) {
				return true;
			}

			int32_t localIndex = y * 4 + x;
			int32_t globalIndex = (py + y) * tetris.tileCountX + px + x;

			if ((pieceData[localIndex] != 0) && (tetris.tiles[globalIndex] != 0)) {
				return true;
			}
		}
	}

	return false;
}

void Snap() {
	if (CheckCollision(tetris.piecePositionX, tetris.piecePositionY, tetris.piece)) {
		return;
	}

	const int px = tetris.piecePositionX >> 1;
	const int py = tetris.piecePositionY >> 1;

	for (uint32_t y = 0; y < 4; ++y) {
		for (uint32_t x = 0; x < 4; ++x) {
			int localIndex = y * 4 + x;
			int globalIndex = (py + y) * tetris.tileCountX + px + x;

			if (tetris.piece[localIndex] != 0) {
				tetris.tiles[globalIndex] = 2;//tetris.piecePalette + 1;
			}
		}
	}

	CheckLines();
}

void CheckLines() {
	int scoreMultiplier = 1;

	for (uint32_t y = 13; y > 0; --y) {
		bool complete = true;
		for (uint32_t x = 5; x < 16; ++x) {
			const int index = y * tetris.tileCountX + x;
			if (tetris.tiles[index] <= 1) {
				complete = false;
			}
		}

		if (complete) {
			tetris.score += 100 * scoreMultiplier;
			tetris.topScore = MAX(tetris.topScore, tetris.score);
			scoreMultiplier += 1;

			// Move rows down
			for (uint32_t ny = y; ny > 1; --ny) {
				for (uint32_t x = 5; x < 15; ++x) {
					const int index0 = (ny - 1) * tetris.tileCountX + x;
					const int index1 = ny * tetris.tileCountX + x;
					tetris.tiles[index1] = tetris.tiles[index0];
				}
			}
			++y;
		}
	}
}

void NewPiece() {
	tetris.piecePositionX = tetris.startX;
	tetris.piecePositionY = tetris.startY;
	gb_memcpy(tetris.piece, Pieces[tetris.nextPiece].data, 16);
	tetris.piecePalette = ((gbRandom() & 0x7) >> 1) + 1; // % 3 or % 4

	tetris.currentPiece = tetris.nextPiece;
	tetris.nextPiece = gbRandom() % PIECE_COUNT;

	tetris.lastVerticalUpdateTime = gbTime();
	tetris.lastHorizontalUpdateTime = tetris.lastVerticalUpdateTime;

	if (CheckCollision(tetris.piecePositionX, tetris.piecePositionY, tetris.piece)) {
		tetris.state = EGS_GAMEOVER;
	}
}

void ResetLevel() {
	for (int y = 0; y < 14; ++y) {
		for (int x = 5; x < 15; ++x) {
			const uint32_t index = y * tetris.tileCountX + x;
			tetris.tiles[index] = 0;
		}
	}
	tetris.score = 0;
	NewPiece();
	tetris.state = EGS_RUNNING;
}

void ProcessEvent() {
	const uint32_t currentTime = gbTime();
		
	if (gbButtonPressed(BTN_B)) {
		ExitTime = currentTime;
	}

	if (gbGetButtonState(BTN_B)) {
		if (currentTime - ExitTime >= 1000) {
			gbDrawText(18, 14, 5, FORMAT_EXIT, (currentTime - ExitTime - 1000) / 10, TEXT_EXITING);
		}
		if (currentTime >= ExitTime + 2000) {
			gbAppClose();
			return;
		}
	}

	if (tetris.state == EGS_MENU) {
		if (gbButtonPressed(BTN_UP)) {
			tetris.selectedMenuItem -= 1;
			if (tetris.selectedMenuItem < 0) {
				tetris.selectedMenuItem += 4;
			}
		}
		if (gbButtonPressed(BTN_DOWN)) {
			tetris.selectedMenuItem += 1;
			if (tetris.selectedMenuItem >= 4) {
				tetris.selectedMenuItem -= 4;
			}
		}
		
		if (gbButtonPressed(BTN_A)) {
			if (tetris.selectedMenuItem == 0) {
				tetris.state = EGS_RUNNING;
			} else if (tetris.selectedMenuItem == 1) {
				// set level / speed
			} else if (tetris.selectedMenuItem == 2) {
				// toggle trace
			} else if (tetris.selectedMenuItem == 3) {
				gbAppClose();
			}
		}
		if (gbButtonPressed(BTN_LEFT)) {
			if (tetris.selectedMenuItem == 1) {
				tetris.level -= 1;
				if (tetris.level < 1) {
					tetris.level += 10;
				}
			} else if (tetris.selectedMenuItem == 2) {
				tetris.trace = !tetris.trace;
			}
		}
		if (gbButtonPressed(BTN_RIGHT)) {
			if (tetris.selectedMenuItem == 1) {
				tetris.level += 1;
				if (tetris.level > 10) {
					tetris.level -= 10;
				}
			} else if (tetris.selectedMenuItem == 2) {
				tetris.trace = !tetris.trace;
			}
		}
	} if (tetris.state == EGS_RUNNING) {
		if (gbButtonPressed(BTN_A)) {
			tetris.state = EGS_PAUSE;
		}
		
		tetris.goLeft = gbGetButtonState(BTN_LEFT);
		if (gbGetButtonDelta(BTN_LEFT) && tetris.goLeft) {
			tetris.lastHorizontalUpdateTime = currentTime - 150;
		}

		tetris.goRight = gbGetButtonState(BTN_RIGHT);
		if (gbGetButtonDelta(BTN_RIGHT) && tetris.goRight) {
			tetris.lastHorizontalUpdateTime = currentTime - 150;
		}

		tetris.dropPiece = gbGetButtonState(BTN_DOWN);

		if (gbButtonPressed(BTN_B)) {
			uint8_t newData[16];
			gb_memset(newData, 0, 16);
			for (uint32_t y = 0; y < Pieces[tetris.currentPiece].length; ++y) {
				for (uint32_t x = 0; x < Pieces[tetris.currentPiece].length; ++x) {
					const int index0 = y * 4 + x;
					const int index1 = (Pieces[tetris.currentPiece].length - 1 - x) * 4 + y;
					newData[index0] = tetris.piece[index1];
				}
			}

			if (CheckCollision(tetris.piecePositionX, tetris.piecePositionY, (const uint8_t*)newData) == false) {
				gb_memcpy(tetris.piece, newData, 16);
			}
		}
	} if (tetris.state == EGS_PAUSE) {
		if (gbButtonPressed(BTN_A)) {
			tetris.state = EGS_RUNNING;
		}
	} if (tetris.state == EGS_GAMEOVER) {
		if (gbButtonPressed(BTN_A)) {
			ResetLevel();
		}
	}
}

void Update() {
	if (tetris.state == EGS_RUNNING) {
		const uint32_t currentTime = gbTime();
		while (currentTime >= tetris.lastVerticalUpdateTime + (tetris.dropPiece ? 100 : 1000 - (tetris.level * 9))) {
			const int newPositionY = tetris.piecePositionY + 2;
			if (CheckCollision(tetris.piecePositionX, newPositionY, 0)) {
				tetris.score += Pieces[tetris.currentPiece].score;
				tetris.topScore = MAX(tetris.topScore, tetris.score);
				Snap();
				NewPiece();
			} else {
				tetris.piecePositionY = newPositionY;
			}
			tetris.lastVerticalUpdateTime = currentTime;
		}

		while (currentTime >= tetris.lastHorizontalUpdateTime + 150) {
			if (tetris.goLeft) {
				const int newPositionX = tetris.piecePositionX - 2;
				if (CheckCollision(newPositionX, tetris.piecePositionY, 0) == false) {
					tetris.piecePositionX = newPositionX;
				}
			} else
			if (tetris.goRight) {
				const int newPositionX = tetris.piecePositionX + 2;
				if (CheckCollision(newPositionX, tetris.piecePositionY, 0) == false) {
					tetris.piecePositionX = newPositionX;
				}
			}
			tetris.lastHorizontalUpdateTime += 150;
		}
	}
}

void DrawScore(int16_t x, int16_t y) {
	gbDrawText(x, y + 0, COLOR_FONT, TEXT_SCORE);
	gbDrawText(x, y + 0, COLOR_FONT, TEXT_SCORE);
	gbDrawText(x, y + 1, COLOR_FONT, FORMAT_SCORE, tetris.score);
	gbDrawText(x, y + 3, COLOR_FONT, TEXT_HISCORE);
	gbDrawText(x, y + 4, COLOR_FONT, FORMAT_SCORE, tetris.topScore);
}

void DrawNextPiece(int16_t px, int16_t py) {
	gbDrawText(px, py, COLOR_FONT, TEXT_NEXT);
	
	// Draw next piece
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			const uint32_t index = y * 4 + x;
			if (Pieces[tetris.nextPiece].data[index] == 1) {
				DrawMetaTile(px + x * 2, py + 1 + y * 2, COLOR_YELLOW, 1, 2, 3, 4);
			}
		}
	}
}
void Draw() {
	if (tetris.state != EGS_MENU) {
		for (uint16_t y = 0; y < tetris.tileCountY; ++y) {
			for (uint16_t x = 0; x < tetris.tileCountX; ++x) {
				const uint16_t index = y * tetris.tileCountX + x;
				switch (tetris.tiles[index]) {
				case 0 :
					break;
				case 1 :
					DrawMetaTile(x * 2, y * 2, COLOR_BRICK, 0, 0, 0, 0);
					break;
				case 2 :
				default :
					DrawMetaTile(x * 2, y * 2, COLOR_GRAY, 1, 2, 3, 4);
					break;
				}
			}
		}
		
		DrawScore(0, 1);
		DrawNextPiece(32, 1);
	}

	if (tetris.state == EGS_MENU) {
		for (int8_t y = 0; y < 5; ++ y) {
			for (int8_t x = 1; x < 20; ++x) {
				const int8_t tile = SPLASH_SCREEN[y * 20 + x];
				if (tile > 0) {
					DrawMetaTile(x * 2, 4 + y * 2, COLOR_GRAY + tile, 1, 2, 3, 4);
				}
			}
		}

		for (uint8_t index = 0; index < MENU_TEXT_COUNT; ++index) {
			gbDrawText(15, 19 + index, COLOR_FONT, FORMAT_MENU, ((tetris.selectedMenuItem == index) ? '>' : ' '), TEXT_MENU[index]);
		}
		gbDrawText(24, 20, COLOR_FONT, FORMAT_LEVEL, tetris.level);
		gbDrawText(24, 21, COLOR_FONT, FORMAT_TRACE, (tetris.trace ? TEXT_ON : TEXT_OFF));
	} else if (tetris.state == EGS_RUNNING) {
		// Draw current piece
		for (int8_t y = 0; y < 4; ++y) {
			for (int8_t x = 0; x < 4; ++x) {
				const uint8_t index = y * 4 + x;
				if (tetris.piece[index]) {
					DrawMetaTile(tetris.piecePositionX + x * 2, tetris.piecePositionY + y * 2, tetris.piecePalette, 1, 2, 3, 4);
				}
			}
		}
	} else if (tetris.state == EGS_PAUSE) {
		gbDrawText(18, 14, COLOR_FONT, TEXT_PAUSE);
	} else if (tetris.state == EGS_GAMEOVER) {
		gbDrawText(16, 14, COLOR_FONT, TEXT_GAMEOVER);
		// gbDrawText(15, 14, COLOR_FONT, TEXT_START);
	}
}

void DrawMetaTile(int16_t x, int16_t y, uint8_t palette, uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
	NameTable[(y + 0) * 40 + (x + 0)] = a + 1;
	NameTable[(y + 0) * 40 + (x + 1)] = b + 1;
	NameTable[(y + 1) * 40 + (x + 0)] = c + 1;
	NameTable[(y + 1) * 40 + (x + 1)] = d + 1;

	AttrTable[(y + 0) * 40 + (x + 0)] = palette;
	AttrTable[(y + 0) * 40 + (x + 1)] = palette;
	AttrTable[(y + 1) * 40 + (x + 0)] = palette;
	AttrTable[(y + 1) * 40 + (x + 1)] = palette;
}

int init() {
	// Setup random generator
	const uint32_t time = gbTime();
	gbRandomSeed(time);
	
	// Upload graphics
    gbPaletteData(0, PaletteCount, Palettes);
    gbPatternData(0, PatternCount, Patterns);
	
	// Game init
	tetris.state = EGS_MENU;
	tetris.selectedMenuItem = 0;
	tetris.level = 1;
	tetris.trace = false;
	tetris.topScore = tetris.score = 0;

	tetris.tileCountX = 20,
	tetris.tileCountY = 15,
	tetris.startX = 18,
	tetris.startY = 0,
	gb_memcpy(tetris.tiles, LevelTemplate, tetris.tileCountX * tetris.tileCountY);

	tetris.lastVerticalUpdateTime = time;
	tetris.lastHorizontalUpdateTime = time;
	tetris.currentPiece = 0;
	tetris.piecePalette = 0;
	tetris.nextPiece = gbRandom() % PIECE_COUNT;
	tetris.dropPiece = false;
	tetris.goLeft = false;
	tetris.goRight = false;

	NewPiece();

	return 0;
}

void update() {
	ProcessEvent(&tetris);
	Update(&tetris);
	Draw(&tetris);
}

