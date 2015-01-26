/*
 * vga_graphics.h
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 *  Modified on: Jan 25, 2015
 *  	Author: Alan L.
 */

#ifndef VGA_GRAPHICS_H_
#define VGA_GRAPHICS_H_

#include "../include/common.h"
#include "../include/memory_map.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

#define FALSE 							0
#define TRUE 							1

// Screen defines
#define SCREEN_WIDTH					320
#define SCREEN_HEIGHT					240

#define SCREEN_HEIGHT_BLOCKAREA			160

#define SCREEN_BACKGROUND_COLOR			0x0000

#define RENDER_OBJECT_WIDTH				8
#define RENDER_OBJECT_HEIGHT			8

#define NUM_RENDER_OBJECTS_WIDTH		((SCREEN_WIDTH) / (RENDER_OBJECT_WIDTH))
#define NUM_RENDER_OBJECTS_HEIGHT		((SCREEN_HEIGHT_BLOCKAREA) / (RENDER_OBJECT_HEIGHT))
#define NUM_RENDER_OBJECTS_TOTAL		((NUM_RENDER_OBJECTS_WIDTH) * (NUM_RENDER_OBJECTS_HEIGHT))

// Defined in terms of Render Object dimensions
#define DEFAULT_BLOCK_WIDTH				3
#define DEFAULT_BLOCK_HEIGHT			2
#define DEFAULT_BLOCK_COLOR				0xFFFF

// Theoretical max of # of blocks
#define DEFAULT_MAX_BLOCKS				((NUM_RENDER_OBJECTS_TOTAL) / ((DEFAULT_BLOCK_WIDTH) * (DEFAULT_BLOCK_HEIGHT)))

// Structures

typedef struct RenderObjectStructure {
	int color[NUM_RENDER_OBJECTS_TOTAL];
	int numRenderObjectsWidth;
	int numRenderObjectsHeight;
} RenderObjectStructure;

// BLOCK OBJECT STRUCTURES
// enum Value refers to color
typedef enum BlockType {
	Invisible = SCREEN_BACKGROUND_COLOR,
	SingleHealth = DEFAULT_BLOCK_COLOR,
	DoubleHealth = DEFAULT_BLOCK_COLOR, // REPLACE LATER
	TripleHealth = DEFAULT_BLOCK_COLOR, // REPLACE LATER
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
	int numBlocksSet;
} BlockObjectStructure;

// Drawing
void DrawBoxFPGA(int x1, int y1, int x2, int y2, int color);
void DrawFPGARenderObject(int renderObjectStartX, int renderObjectStartY,
		int color);
void DrawRenderObjectStructure(RenderObjectStructure *renderObjectStructure);

// Initialization
int InitializeVGA(alt_up_pixel_buffer_dma_dev *pixel_buffer);
void InitializeRenderObjectStructure(
		RenderObjectStructure *renderObjectStructure);
void InitializeBlockObjectStructure(BlockObjectStructure * blockObjectStructure,
		RenderObjectStructure *renderObjectStructure);

// Methods
void AddBlock(BlockObjectStructure * blockObjectStructure,
		int renderObjectXStart, int renderObjectYStart);

void MapBlockObjectStructureToRender(
		BlockObjectStructure * blockObjectStructure,
		RenderObjectStructure *renderObjectStructure);
void DrawBallObjectMovement();
void DrawFPGABallObject(int renderObjectStartX, int renderObjectStartY,
		int color);
// Test funcs
void SetRandomColors(RenderObjectStructure *renderObjectStructure);
void SetBlack(RenderObjectStructure *renderObjectStructure);
void draw_random_boxes_forever();
void draw_diagonal_line_with_character();

#endif /* VGA_GRAPHICS_H_ */
