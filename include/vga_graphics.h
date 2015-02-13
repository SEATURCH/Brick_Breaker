/*
 * vga_graphics.h
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 *  Modified on: Jan 25, 2015
 *  	Author: Alan L.
 *  Modified on: Jan 28, 2015
 *	Author: Francis
 */

#ifndef VGA_GRAPHICS_H_
#define VGA_GRAPHICS_H_

#include "../include/common.h"
#include "../include/ir_sensor.h"
#include "../include/memory_map.h"
#include "../include/sd_card.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

#define BUFFERLEN 500
#define FALSE 							0
#define TRUE 							1
// Screen defines
#define SCREEN_WIDTH					320
#define SCREEN_HEIGHT					240
#define SCREEN_TOTAL_PIXELS				((SCREEN_WIDTH)*(SCREEN_HEIGHT))

#define SCREEN_HEIGHT_BLOCKAREA			160

#define SCREEN_BACKGROUND_COLOR			0x0000

#define RENDER_OBJECT_WIDTH				8
#define RENDER_OBJECT_HEIGHT			8

#define RENDER_TO_BALL_RATIO			2	// must be a whole number
#define BALL_OBJECT_WIDTH				RENDER_OBJECT_WIDTH/RENDER_TO_BALL_RATIO
#define BALL_OBJECT_HEIGHT				BALL_OBJECT_WIDTH 		// enforce it to be square

#define X_LIMIT							(SCREEN_WIDTH - BALL_OBJECT_WIDTH)
#define Y_LIMIT							(SCREEN_HEIGHT - BALL_OBJECT_HEIGHT)

#define NUM_RENDER_OBJECTS_WIDTH		((SCREEN_WIDTH) / (RENDER_OBJECT_WIDTH))
#define NUM_RENDER_OBJECTS_HEIGHT		((SCREEN_HEIGHT_BLOCKAREA) / (RENDER_OBJECT_HEIGHT))
#define NUM_RENDER_OBJECTS_TOTAL		((NUM_RENDER_OBJECTS_WIDTH) * (NUM_RENDER_OBJECTS_HEIGHT))

// Defined in terms of Render Object dimensions
#define DEFAULT_BLOCK_WIDTH				2
#define DEFAULT_BLOCK_HEIGHT			1
#define DEFAULT_BLOCK_COLOR				0xFFFF

// Theoretical max of # of blocks
#define DEFAULT_MAX_BLOCKS				((NUM_RENDER_OBJECTS_TOTAL) / ((DEFAULT_BLOCK_WIDTH) * (DEFAULT_BLOCK_HEIGHT)))

// Paddle default values
#define DEFAULT_PADDLE_WIDTH			64 // must be multiple of ball width
#define DEFAULT_PADDLE_HEIGHT			2
#define INITIAL_PADDLE_Y_POS			220
#define INITIAL_PADDLE_X_POS			((SCREEN_WIDTH >> 1) - DEFAULT_PADDLE_WIDTH)
#define DEFAULT_PADDLE_COLOR			0x0F0F

//Drawing boxes with/without borders
#define WITHOUT_BORDERS 0
#define WITH_BORDERS 1
// Structures

typedef struct RenderObjectStructure {
	int color[NUM_RENDER_OBJECTS_TOTAL]; // need new struct with array and int that tells if there's a block to find out invisible block
	int blockNumber[NUM_RENDER_OBJECTS_TOTAL]; // need new struct with array and int that tells if there's a block to find out invisible block
	int numRenderObjectsWidth;
	int numRenderObjectsHeight;
} RenderObjectStructure;

// BLOCK OBJECT STRUCTURES
// enum Value refers to color
typedef enum BlockType {
	Invisible = SCREEN_BACKGROUND_COLOR,
	SingleHealth = 0x6000,
	DoubleHealth = 0xA000, // REPLACE LATER
	TripleHealth = 0xF000, // REPLACE LATER
	Unbreakable = 0x2329, // REPLACE LATER
} BlockType;

typedef enum PowerUpType {
	None,
// ...

} PowerUpType;

typedef struct BlockObject {
	int isActive;
	// Block types
	BlockType blockType;
	PowerUpType powerUpType;

	// Rendering
	// Stride used is NUM_RENDER_OBJECTS_WIDTH
	int blockXStart;
	int blockYStart;
	int blockWidthRenderObjects;
	int blockHeightRenderObjects;

} BlockObject;

// This BlockObjectStructure acts as a scratch buffer for blocks. It starts out empty, and is then occupied in the beginning
typedef struct BlockObjectStructure {
	BlockObject blockObjects[DEFAULT_MAX_BLOCKS];
	unsigned char numBlocksSet;
	unsigned char numBlocksLeft; // Used for the remaining blocks
} BlockObjectStructure;

// Structure for the paddle
typedef struct Paddle {
	int x_pos;
	int y_pos;
	int width;
	int height;
	int color;
} Paddle;

typedef struct Ball {
	// Contain the current cartesian position of the ball
	int x_pos;
	int y_pos;

	// Determines the frequency of frames in which to move 1 pixel
	int x_frequency;
	int y_frequency;

	// Must be a value in the set of {-1, 1}
	int x_dir;
	int y_dir;

	int color;

	// Is Active
	int isActive;

} Ball;

#define MAX_BALLS	10

typedef struct BallObjectStructure{
	Ball balls[MAX_BALLS];
	int numBallsSet;
} BallObjectStructure;


#define CHAR_SIZE			4
#define LINE_GAP			3
#define COLOR_HEIGHT		3 * CHAR_SIZE
#define ONE_SIDE_MARGIN		CHAR_SIZE
#define MARGIN				2 * ONE_SIDE_MARGIN
#define CHAR_X				33
#define	CHAR_Y				20
#define	CHAR_BUFF			20
#define COLOR_X				(CHAR_X * CHAR_SIZE) - (ONE_SIDE_MARGIN)
#define COLOR_Y				(CHAR_Y * CHAR_SIZE) - (ONE_SIDE_MARGIN)
#define CURSOR_COLOR		0x7BEF
#define TOTAL_MENU_OPTIONS	5
#define LEVELS				2
typedef struct MenuContent{
	char str[CHAR_BUFF];
	int strLen;
}MenuContent;

typedef struct ScreenMenu{
	MenuContent block[TOTAL_MENU_OPTIONS];
	int movedUp;
	int track;
	int cursor_pos;

}ScreenMenu;

int DrawStartMenu(int restart);
int DrawLevelMenu();
int DrawPauseMenu();
int SaveGame();
//void SaveGame();
void InitializeScreenMenu(ScreenMenu *Menu, alt_up_char_buffer_dev *char_buffer);
void DrawLoadMenu(RenderObjectStructure *renderObjectStructure, BlockObjectStructure * blockObjectStructure);
void reDrawMenu(ScreenMenu *Menu);
void LoadNewGame(RenderObjectStructure *renderObjectStructure, BlockObjectStructure * blockObjectStructure, int level);
int DrawSavedGameMenu();

void ClearScreen(ScreenMenu *Menu);

// Drawing
void DrawBoxFPGA(int x1, int y1, int x2, int y2, int color, int borders);
//void DrawBoxFPGA(int x1, int y1, int x2, int y2, int color);
void DrawFPGARenderObject(int renderObjectStartX, int renderObjectStartY,
		int color);
void DrawRenderObjectStructure(RenderObjectStructure *renderObjectStructure);
void DrawFPGAPaddleObject(Paddle* paddle, int color);

// Initialization
int InitializeVGA(alt_up_pixel_buffer_dma_dev *pixel_buffer);
void InitializeRenderObjectStructure(
		RenderObjectStructure *renderObjectStructure);
void InitializeBlockObjectStructure(BlockObjectStructure * blockObjectStructure,
		RenderObjectStructure *renderObjectStructure);
void InitializePaddle(Paddle* paddle);

// Methods
void AddBlock(BlockObjectStructure * blockObjectStructure,
		int renderObjectXStart, int renderObjectYStart, int blockWidth,
		int blockHeight, BlockType blockType);

void MapBlockObjectStructureToRender(
		BlockObjectStructure * blockObjectStructure,
		RenderObjectStructure *renderObjectStructure);
void DrawBallObjectMovement(BlockObjectStructure *blockObjectStructure,
		RenderObjectStructure *renderObjectStructure, Paddle *paddle);
void DrawFPGABallObject(int renderObjectStartX, int renderObjectStartY,
		int color);
void MovePaddle(Paddle* paddle, int paddleNextPosX);

void EraseBall(Ball *ball);
void DrawBall(Ball *ball);
void HitBlockFromRenderObject(BlockObjectStructure *blockObjectStructure, RenderObjectStructure *renderObjectStructure, int renderObjectIndex);

// Test funcs
void SetRandomColors(RenderObjectStructure *renderObjectStructure);
void SetBlack(RenderObjectStructure *renderObjectStructure);
void draw_random_boxes_forever();
void draw_diagonal_line_with_character();
void EraseBlockAndResetRenderObjects(int blockCursor,
		BlockObjectStructure * blockObjectStructure,
		RenderObjectStructure *renderObjectStructure);
#endif /* VGA_GRAPHICS_H_ */
