#ifndef __TETIRS_H__
#define __TETRIS_H__

#define GB_API_IMPORT
#include "game.h"
#include "eabi.h"

//#include <string.h>

enum GameState {
	EGS_MENU,
	EGS_RUNNING,
	EGS_PAUSE,
	EGS_GAMEOVER
};

#ifndef NULL
#define NULL ((void*)0)
#endif

#define TILE_WIDTH  16
#define TILE_HEIGHT 16

#define TILE_COUNT_X  (GB_SCREEN_WIDTH / TILE_WIDTH)
#define TILE_COUNT_Y  (GB_SCREEN_HEIGHT / TILE_HEIGHT)
#define TILE_COUNT    (TILE_COUNT_X * TILE_COUNT_Y)

#define TILE_EMPTY   0
#define TILE_BRICK   1
#define TILE_BLOCK   2

#define PIECE_COUNT 8


#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) < (b)) ? (b) : (a))

#define BOARD_POSTION_X  3
#define BOARD_POSTION_Y  0
#define BOARD_WIDTH      10
#define BOARD_HEIGHT     14
#define BOARD_LENGTH     (BOARD_WIDTH * BOARD_HEIGHT)

int init();
void update();

typedef struct {
	uint8_t length;
	uint8_t score;
	uint8_t data[4 * 4];
} Piece;

typedef struct {
	uint8_t state;
	uint32_t score;
	uint32_t topScore;

	uint8_t tileCountX;
	uint8_t tileCountY;
	uint8_t startX;
	uint8_t startY;
	uint8_t tiles[TILE_COUNT];

	int16_t piecePositionX;
	int16_t piecePositionY;
	uint8_t piecePalette;
	uint8_t piece[4 * 4];
	uint8_t currentPiece;
	uint8_t nextPiece;
	uint64_t lastVerticalUpdateTime;
	uint64_t lastHorizontalUpdateTime;
	bool dropPiece;
	bool goLeft;
	bool goRight;
	int8_t selectedMenuItem;
	int8_t level;
	int8_t trace;
} Tetris;

bool CheckCollision(int16_t x, int16_t y, const uint8_t* pieceData);
void Snap();
void CheckLines();
void NewPiece();
void ResetLevel();
void ProcessEvent();
void Update();
void Draw();
void DrawMetaTile(int16_t x, int16_t y, uint8_t palette, uint8_t a, uint8_t b, uint8_t c, uint8_t d);

#endif // __TETRIS_H__
