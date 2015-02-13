/*
 * vga_graphics.c
 *
 *  Created on: Feb 12, 2015
 *      Author: Himanshu
 */
#include "../include/vga_graphics.h"
#include "../include/timestamp_timer.h"

// x - 9 bits
// y - 8 bits
void DrawBoxFPGA(int x1, int y1, int x2, int y2, int color, int borders) {
	IOWR_32DIRECT(DRAWER_BASE, 0, x1);
	// Set x1
	IOWR_32DIRECT(DRAWER_BASE, 4, y1);
	// Set y1
	IOWR_32DIRECT(DRAWER_BASE, 8, x2);
	// Set x2
	IOWR_32DIRECT(DRAWER_BASE, 12, y2);
	// Set y2
	IOWR_32DIRECT(DRAWER_BASE, 16, color);
	IOWR_32DIRECT(DRAWER_BASE, 24, borders);
	// Set colour
	IOWR_32DIRECT(DRAWER_BASE, 20, 1);
	// Start drawing
	while (IORD_32DIRECT(DRAWER_BASE, 20) == 0)
		; // wait until done
}
void DrawFPGABallObject(int renderObjectStartX, int renderObjectStartY,
		int color) {
	// Set the coordinates of the render object
	IOWR_32DIRECT(DRAWER_BASE, 0, renderObjectStartX);
	IOWR_32DIRECT(DRAWER_BASE, 4, renderObjectStartY);

	IOWR_32DIRECT(DRAWER_BASE, 8, renderObjectStartX + BALL_OBJECT_WIDTH);
	IOWR_32DIRECT(DRAWER_BASE, 12, renderObjectStartY + BALL_OBJECT_HEIGHT);

	// Set colour
	IOWR_32DIRECT(DRAWER_BASE, 16, color);
	IOWR_32DIRECT(DRAWER_BASE, 24, 0);
	// Start drawing
	IOWR_32DIRECT(DRAWER_BASE, 20, 1);

	// wait until done
	while (IORD_32DIRECT(DRAWER_BASE, 20) == 0)
		;
}

void DrawFPGAPaddleObject(Paddle* paddle, int color) { //The color is added just so we can erase the paddle
	DrawBoxFPGA(paddle->x_pos, paddle->y_pos, paddle->x_pos + paddle->width,
			paddle->y_pos + paddle->height, color, WITHOUT_BORDERS);
}

void DrawFPGARenderObject(int renderObjectStartX, int renderObjectStartY,
		int color) {
	// Set the coordinates of the render object
	IOWR_32DIRECT(DRAWER_BASE, 0, renderObjectStartX);
	IOWR_32DIRECT(DRAWER_BASE, 4, renderObjectStartY);

	IOWR_32DIRECT(DRAWER_BASE, 8, renderObjectStartX + RENDER_OBJECT_WIDTH);
	IOWR_32DIRECT(DRAWER_BASE, 12, renderObjectStartY + RENDER_OBJECT_HEIGHT);

	// Set colour
	IOWR_32DIRECT(DRAWER_BASE, 16, color);
	if(color == SCREEN_BACKGROUND_COLOR) {
		IOWR_32DIRECT(DRAWER_BASE,24, WITHOUT_BORDERS);
	}
	// Start drawing
	IOWR_32DIRECT(DRAWER_BASE, 20, 1);

	// wait until done
	while (IORD_32DIRECT(DRAWER_BASE, 20) == 0)
		;
}

// Draws the current render object layout to screen
// This should only need to be called once before the main loop, to draw all the blocks
void DrawRenderObjectStructure(RenderObjectStructure *renderObjectStructure) {
	int stride = renderObjectStructure->numRenderObjectsWidth;
	int cursorY;
	int cursorX;
	int offset;

	for (cursorY = 0; cursorY < renderObjectStructure->numRenderObjectsHeight;
			cursorY++) {
		offset = (cursorY * stride);
		for (cursorX = 0;
				cursorX < renderObjectStructure->numRenderObjectsWidth;
				cursorX++) {
			DrawFPGARenderObject((cursorX * RENDER_OBJECT_WIDTH),
					(cursorY * RENDER_OBJECT_HEIGHT),
					renderObjectStructure->color[offset + cursorX]);
		}
	}
}

int InitializeVGA(alt_up_pixel_buffer_dma_dev *pixel_buffer) {
	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	if (pixel_buffer == NULL) {
		return 0;
	}
	// Set the background buffer address – Although we don’t use the background,
	// they only provide a function to change the background buffer address, so
	// we must set that, and then swap it to the foreground.
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buff_base);
	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	// Wait for the swap to complete
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;

	// Clear the screen
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	return 1;
}

void InitializeRenderObjectStructure(
		RenderObjectStructure *renderObjectStructure) {
	// Set the color of all render objects to 0
	int renderObjectCursor;
	for (renderObjectCursor = 0; renderObjectCursor < NUM_RENDER_OBJECTS_TOTAL;
			renderObjectCursor++) {
		renderObjectStructure->color[renderObjectCursor] =
				SCREEN_BACKGROUND_COLOR; //renderObjectCursor * 800;
		renderObjectStructure->blockNumber[renderObjectCursor] = -1;
	}

	// Set the stride so that the block object structure can stride through multiple rows
	renderObjectStructure->numRenderObjectsWidth = NUM_RENDER_OBJECTS_WIDTH;
	renderObjectStructure->numRenderObjectsHeight = NUM_RENDER_OBJECTS_HEIGHT;

}

void InitializeBlockObjectStructure(BlockObjectStructure * blockObjectStructure,
		RenderObjectStructure *renderObjectStructure) {
	// Iterate through the entire possible scratchpad, making each block inactive
	int blockObjectCursor;
	for (blockObjectCursor = 0; blockObjectCursor < DEFAULT_MAX_BLOCKS;
			blockObjectCursor++) {
		blockObjectStructure->blockObjects[blockObjectCursor].isActive = FALSE;
	}
	blockObjectStructure->numBlocksSet = 0;
	blockObjectStructure->numBlocksLeft = 0;
}

void InitializePaddle(Paddle* paddle) {
	paddle->x_pos = INITIAL_PADDLE_X_POS;
	paddle->y_pos = INITIAL_PADDLE_Y_POS;
	paddle->height = DEFAULT_PADDLE_HEIGHT;
	paddle->width = DEFAULT_PADDLE_WIDTH;
	paddle->color = DEFAULT_PADDLE_COLOR;
}

// Adds a block to the screen at (renderObjectXStart, renderObjectYStart), with default attributes
// This function does NOT do error checking for collision with other blocks!
// Implement error-checking outside of this function if error-checking is needed.
void AddBlock(BlockObjectStructure *blockObjectStructure,
		int renderObjectXStart, int renderObjectYStart, int blockWidth,
		int blockHeight, BlockType blockType) {
	BlockObject *blockObject =
			&blockObjectStructure->blockObjects[blockObjectStructure->numBlocksSet];

	blockObject->blockXStart = renderObjectXStart;
	blockObject->blockYStart = renderObjectYStart;
	blockObject->blockWidthRenderObjects = blockWidth;
	blockObject->blockHeightRenderObjects = blockHeight;

	blockObject->blockType = blockType;
	blockObject->powerUpType = None;

	blockObject->isActive = TRUE;

	++blockObjectStructure->numBlocksSet;
	if (blockObject->blockType != Unbreakable) {
		++blockObjectStructure->numBlocksLeft;
	}
	DrawBoxFPGA(blockObject->blockXStart * RENDER_OBJECT_WIDTH,
			blockObject->blockYStart * RENDER_OBJECT_HEIGHT,
			(blockObject->blockXStart + blockObject->blockWidthRenderObjects) * RENDER_OBJECT_WIDTH - 1,
			(blockObject->blockYStart + blockObject->blockHeightRenderObjects) * RENDER_OBJECT_HEIGHT - 1,
			blockObject->blockType, WITH_BORDERS);

}

void MapBlockObjectStructureToRender(
		BlockObjectStructure * blockObjectStructure,
		RenderObjectStructure *renderObjectStructure) {
	int numBlocks = blockObjectStructure->numBlocksSet;
	int blockCursor = 0;
	int index = -1;

	for (blockCursor = 0; blockCursor < numBlocks; blockCursor++) {
		BlockObject *blockObject =
				&blockObjectStructure->blockObjects[blockCursor];
		int renderObjectWidth = blockObject->blockWidthRenderObjects;
		int renderObjectHeight = blockObject->blockHeightRenderObjects;
		int renderObjectColor = (int) blockObject->blockType;
		int blockXStart = blockObject->blockXStart;
		int blockYStart = blockObject->blockYStart;
		int xCursor, yCursor;

		for (yCursor = 0; yCursor < renderObjectHeight; yCursor++) {
			int renderObjectCursor = (yCursor + blockYStart)
					* renderObjectStructure->numRenderObjectsWidth;
			for (xCursor = 0; xCursor < renderObjectWidth; xCursor++) {
				index = renderObjectCursor + (xCursor + blockXStart);
				renderObjectStructure->color[index] = renderObjectColor;
				renderObjectStructure->blockNumber[index] = blockCursor;
				//printf("index = %d    Cursor = %d\n", index, blockCursor);
			}
		}

	}
}


// Paddle movement functions

void MovePaddle(Paddle* paddle, int paddleNextPosX) {
	// Update paddle x position with error checking, ONLY clear the previous paddle position if we have a legitimate movement
	if ((paddleNextPosX >= 0)
			&& ((paddleNextPosX + paddle->width) <= (SCREEN_WIDTH - 1))
			&& ((paddleNextPosX + paddle->width) >= 0)) {
		// Clear out the paddle in the current position
		DrawFPGAPaddleObject(paddle, SCREEN_BACKGROUND_COLOR);
		paddle->x_pos = paddleNextPosX;
	}

	// Draw the paddle in the new position
	DrawFPGAPaddleObject(paddle, paddle->color);
}

void EraseBall(Ball *ball) {
	DrawBoxFPGA(ball->x_pos, ball->y_pos, ball->x_pos + BALL_OBJECT_WIDTH,
			ball->y_pos + BALL_OBJECT_HEIGHT, SCREEN_BACKGROUND_COLOR, WITHOUT_BORDERS);
}
void DrawBall(Ball *ball) {
	DrawBoxFPGA(ball->x_pos, ball->y_pos, ball->x_pos + BALL_OBJECT_WIDTH,
			ball->y_pos + BALL_OBJECT_HEIGHT, ball->color, WITHOUT_BORDERS);
}

void HitBlockFromRenderObject(BlockObjectStructure *blockObjectStructure,
		RenderObjectStructure *renderObjectStructure, int renderObjectIndex) {
	BlockObject *blockObject;
	int blockObjectIndex = renderObjectStructure->blockNumber[renderObjectIndex];
	int xCursor, yCursor;
	int renderObjectCursor;

	blockObject = &blockObjectStructure->blockObjects[blockObjectIndex];

	if (blockObject->isActive == FALSE)
		return;

	switch (blockObject->blockType) {
	case TripleHealth:
		blockObject->blockType = DoubleHealth;
		break;
	case DoubleHealth:
		blockObject->blockType = SingleHealth;
		break;
	case SingleHealth:
		blockObject->blockType = Invisible;
		blockObject->isActive = FALSE;
		blockObjectStructure->numBlocksLeft--;
		break;
	default:
		break;
	}

	// Update Render Objects
	for(yCursor = blockObject->blockYStart; yCursor < (blockObject->blockYStart + blockObject->blockHeightRenderObjects); yCursor++)
	{
		for(xCursor = blockObject->blockXStart; xCursor < (blockObject->blockXStart + blockObject->blockWidthRenderObjects); xCursor++)
		{
			renderObjectCursor = (yCursor * renderObjectStructure->numRenderObjectsWidth) + xCursor;
			renderObjectStructure->color[renderObjectCursor] = blockObject->blockType;
		}
	}


	if(blockObject->blockType == Invisible) {
		// Draw new block
		DrawBoxFPGA(blockObject->blockXStart * RENDER_OBJECT_WIDTH,
				blockObject->blockYStart * RENDER_OBJECT_HEIGHT,
				(blockObject->blockXStart + blockObject->blockWidthRenderObjects) * RENDER_OBJECT_WIDTH - 1,
				(blockObject->blockYStart + blockObject->blockHeightRenderObjects) * RENDER_OBJECT_HEIGHT - 1,
				blockObject->blockType, WITHOUT_BORDERS);
	} else {
		DrawBoxFPGA(blockObject->blockXStart * RENDER_OBJECT_WIDTH,
				blockObject->blockYStart * RENDER_OBJECT_HEIGHT,
				(blockObject->blockXStart + blockObject->blockWidthRenderObjects) * RENDER_OBJECT_WIDTH - 1,
				(blockObject->blockYStart + blockObject->blockHeightRenderObjects) * RENDER_OBJECT_HEIGHT - 1,
				blockObject->blockType, WITH_BORDERS);
	}


}

// Test func
void SetRandomColors(RenderObjectStructure *renderObjectStructure) {
	// Set the color of all render objects to a pseudo-random color based off the cursor
	int renderObjectCursor;
	for (renderObjectCursor = 0; renderObjectCursor < NUM_RENDER_OBJECTS_TOTAL;
			renderObjectCursor++) {
		renderObjectStructure->color[renderObjectCursor] = renderObjectCursor
				* 800;
	}
}

void SetBlack(RenderObjectStructure *renderObjectStructure) {
	// Set the color of all render objects to a pseudo-random color based off the cursor
	int renderObjectCursor;
	for (renderObjectCursor = 0; renderObjectCursor < NUM_RENDER_OBJECTS_TOTAL;
			renderObjectCursor++) {
		renderObjectStructure->color[renderObjectCursor] = 0x0000;
	}
}

void draw_random_boxes_forever() {
	srand(time(NULL));

	// VGA
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	//alt_up_char_buffer_dev *char_buffer;

	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");

	// Set the background buffer address – Although we don’t use the background,
	// they only provide a function to change the background buffer address, so
	// we must set that, and then swap it to the foreground.
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buff_base);

	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

	// Wait for the swap to complete
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;

	// Clear the screen
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	int hw = 1;
	unsigned int* seed = (unsigned int*) 0x666;

	int x1, y1;
	int x2, y2;

	int color;

	for (;;) {
		// Generate Random Values
		x1 = 0x1FF & rand_r(seed);
		x2 = 0x1FF & rand_r(seed);
		y1 = 0xFF & rand_r(seed);
		y2 = 0xFF & rand_r(seed);
		color = 0xFFFF & rand_r(seed);

		// Check within bounds
		if (x1 >= SCREEN_WIDTH)
			x1 -= SCREEN_WIDTH;
		if (x2 >= SCREEN_WIDTH)
			x2 -= SCREEN_WIDTH;
		if (y1 >= SCREEN_HEIGHT)
			y1 -= SCREEN_HEIGHT;
		if (y2 >= SCREEN_HEIGHT)
			y2 -= SCREEN_HEIGHT;

		// Draw random values

		if (hw) {
			DrawBoxFPGA(x1, y1, x2, y2, color, WITHOUT_BORDERS);
		} else {
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x1, y1, x2, y2,
					color, 0);
		}
	}
}

void draw_diagonal_line_with_character() {
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev *char_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer");
	alt_up_char_buffer_init(char_buffer);
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buff_base);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 0, 320, 240, 0xFFFF, 0);
	alt_up_char_buffer_string(char_buffer, "EECE 381", 40, 30);
}

void InitializeScreenMenu(ScreenMenu *Menu, alt_up_char_buffer_dev *char_buffer) {
	Menu->track = -1;
	Menu->cursor_pos = -1;
	Menu->movedUp = -1;
	alt_up_char_buffer_string(char_buffer, "KEY3: SELECT        KEY2: UP        KEY1: DOWN", 25, 59);

}
void AddMenuOptions(ScreenMenu *Menu, alt_up_char_buffer_dev *char_buffer,
		const char *str) {
	if (Menu->track > TOTAL_MENU_OPTIONS) {
		printf("ERROR: Menu full!!!!\n");
		return;
	}

	Menu->track++;
	int tr = Menu->track;
	snprintf(Menu->block[Menu->track].str, CHAR_BUFF, str);
	Menu->block[Menu->track].strLen = strlen(str);

	int y_offset = LINE_GAP * Menu->track;
	alt_up_char_buffer_string(char_buffer, str, CHAR_X, (CHAR_Y + y_offset));

	//if (Menu->track == 0) {
	if (tr == 0) {
		Menu->cursor_pos++;
		int x2 = COLOR_X + (Menu->block[Menu->cursor_pos].strLen * CHAR_SIZE)
				+ MARGIN;
		int y2 = COLOR_Y + COLOR_HEIGHT;
		DrawBoxFPGA(COLOR_X, COLOR_Y, x2, y2, CURSOR_COLOR, WITHOUT_BORDERS);
	}
	Menu->track = tr;
}

void reDrawMenu(ScreenMenu *Menu) {
	int x1, y1, x2, y2;
	x1 = COLOR_X;
	y1 = COLOR_Y + (COLOR_HEIGHT * Menu->cursor_pos);
	x2 = x1 + (Menu->block[Menu->cursor_pos].strLen * CHAR_SIZE) + MARGIN;
	y2 = y1 + COLOR_HEIGHT;
	DrawBoxFPGA(x1, y1, x2, y2, SCREEN_BACKGROUND_COLOR, WITHOUT_BORDERS);

	if (Menu->movedUp == 0) { //moved down
		x2 = x1 + (Menu->block[++Menu->cursor_pos].strLen * CHAR_SIZE) + MARGIN;
		y1 = COLOR_Y + (COLOR_HEIGHT * Menu->cursor_pos);
		y2 = y1 + COLOR_HEIGHT;
	} else if (Menu->movedUp == 1) {
		x2 = x1 + (Menu->block[--Menu->cursor_pos].strLen * CHAR_SIZE) + MARGIN;
		y1 = COLOR_Y + (COLOR_HEIGHT * Menu->cursor_pos);
		y2 = y1 + COLOR_HEIGHT;
	}
	DrawBoxFPGA(x1, y1, x2, y2, CURSOR_COLOR, WITHOUT_BORDERS);
}

int DrawStartMenu(int restart) {
	ScreenMenu Menu;
	int ret;
	alt_up_char_buffer_dev *char_buffer;

	char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	InitializeScreenMenu(&Menu, char_buffer);

	if (restart)
		alt_up_char_buffer_string(char_buffer, "Game Over :(", 33, 10);
	AddMenuOptions(&Menu, char_buffer, "Start New Game");
	AddMenuOptions(&Menu, char_buffer, "Load Saved Game");
	AddMenuOptions(&Menu, char_buffer, "Exit");
	while (1) {
		if (IORD_32DIRECT(PUSH_BASE,0) == 0x06) {
			while (IORD_32DIRECT(PUSH_BASE,0) != 0xE)
				;
			alt_up_char_buffer_clear(char_buffer);
			ClearScreen(&Menu);
			ret = Menu.cursor_pos;
			break;
		}
		if (Menu.movedUp == -1)
			switch (IORD_32DIRECT(PUSH_BASE,0)) {
			case 0xC:
				if (Menu.cursor_pos < Menu.track)
					Menu.movedUp = 0; // moved down
				break;
			case 0xA:
				if (Menu.cursor_pos > 0)
					Menu.movedUp = 1;
				break;
			default:
				break;
			}

		if (IORD_32DIRECT(PUSH_BASE,0) == 0xE && Menu.movedUp != -1) {
			reDrawMenu(&Menu);
			Menu.movedUp = -1;
		}
	}
	return ret;
}

int DrawLevelMenu(){
		int ret, i = 0;
		ScreenMenu Menu;
		const char* game[] = { "LEVEL 1", "LEVEL 2", "LEVEL 3", "LEVEL 4", "LEVEL 5",
				"LEVEL 6", "LEVEL 7", "LEVEL 8", "LEVEL 9", "LEVEL 10" };
		alt_up_char_buffer_dev *char_buffer;

		char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer");
		alt_up_char_buffer_init(char_buffer);
		alt_up_char_buffer_clear(char_buffer);

		InitializeScreenMenu(&Menu, char_buffer);
		while (i < (TOTAL_MENU_OPTIONS - 1) && i < LEVELS)
			AddMenuOptions(&Menu, char_buffer, game[i++]);
			AddMenuOptions(&Menu, char_buffer, "BACK");

		while (1) {
			if (IORD_32DIRECT(PUSH_BASE,0) == 0x06) {
				while (IORD_32DIRECT(PUSH_BASE,0) != 0xE)
					;
				alt_up_char_buffer_clear(char_buffer);
				ClearScreen(&Menu);
				if(Menu.cursor_pos == Menu.track) ret = (TOTAL_MENU_OPTIONS - 1); // back button
				else ret = Menu.cursor_pos;
				break;
			}
			if (Menu.movedUp == -1)
				switch (IORD_32DIRECT(PUSH_BASE,0)) {
				case 0xC:
					if (Menu.cursor_pos < Menu.track)
						Menu.movedUp = 0; // moved down
					break;
				case 0xA:
					if (Menu.cursor_pos > 0)
						Menu.movedUp = 1;
					break;
				default:
					break;
				}

			if (IORD_32DIRECT(PUSH_BASE,0) == 0xE && Menu.movedUp != -1) {
				reDrawMenu(&Menu);
				Menu.movedUp = -1;
			}
		}
		return ret;
}

void LoadNewGame(RenderObjectStructure *renderObjectStructure,
		BlockObjectStructure * blockObjectStructure, int level) {
	int i, j;
	switch (level) {
	case 0:
		j=5;
		//for (j = 5; (j + DEFAULT_BLOCK_HEIGHT - 1) < 32; j += 2) {
			for (i = 1; (i + DEFAULT_BLOCK_WIDTH - 1) < 40; i += 2) {
				AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
						DEFAULT_BLOCK_HEIGHT, SingleHealth);
			}

		//}
		j = 6;
		for (i = 1; (i + DEFAULT_BLOCK_WIDTH - 1) < 40; i += 2) {
						AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
								DEFAULT_BLOCK_HEIGHT, DoubleHealth);
		}
		j = 7;
		for (i = 1; (i + DEFAULT_BLOCK_WIDTH - 1) < 40; i += 2) {
								AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
										DEFAULT_BLOCK_HEIGHT, TripleHealth);
		}
		j=10;
				//for (j = 5; (j + DEFAULT_BLOCK_HEIGHT - 1) < 32; j += 2) {
					for (i = 1; (i + DEFAULT_BLOCK_WIDTH - 1) < 40; i += 2) {
						AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
								DEFAULT_BLOCK_HEIGHT, TripleHealth);
					}

				//}
				j = 11;
				for (i = 1; (i + DEFAULT_BLOCK_WIDTH - 1) < 40; i += 2) {
								AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
										DEFAULT_BLOCK_HEIGHT, DoubleHealth);
				}
				j = 12;
				for (i = 1; (i + DEFAULT_BLOCK_WIDTH - 1) < 40; i += 2) {
										AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
												DEFAULT_BLOCK_HEIGHT, SingleHealth);
				}

		j = 15;
				for (i = 7; (i + DEFAULT_BLOCK_WIDTH - 1) < 17; i += 2) {
									AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
												DEFAULT_BLOCK_HEIGHT, TripleHealth);
								}
				for (i = 23; (i + DEFAULT_BLOCK_WIDTH - 1) < 35; i += 2) {
													AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
																DEFAULT_BLOCK_HEIGHT, TripleHealth);
												}
		j = 16;
				for (i = 11; (i + DEFAULT_BLOCK_WIDTH - 1) < 17; i += 2) {
													AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
																DEFAULT_BLOCK_HEIGHT, TripleHealth);
												}
				for (i = 23; (i + DEFAULT_BLOCK_WIDTH - 1) < 30; i += 2) {
																	AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
																				DEFAULT_BLOCK_HEIGHT, TripleHealth);
																}
		j = 17;
				for (i = 15; (i + DEFAULT_BLOCK_WIDTH - 1) < 17; i += 2) {
													AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
																DEFAULT_BLOCK_HEIGHT, TripleHealth);
				}
				for (i = 23; (i + DEFAULT_BLOCK_WIDTH - 1) < 25; i += 2) {
																	AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
																				DEFAULT_BLOCK_HEIGHT, TripleHealth);
								}
		break;
	case 1:
		for (j = 0; (j + DEFAULT_BLOCK_HEIGHT) < NUM_RENDER_OBJECTS_HEIGHT;
				j += 3 * DEFAULT_BLOCK_HEIGHT) {
			for (i = 0; (i + DEFAULT_BLOCK_WIDTH) < NUM_RENDER_OBJECTS_WIDTH;
					i += 3 * DEFAULT_BLOCK_WIDTH) {
				AddBlock(blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
						DEFAULT_BLOCK_HEIGHT, TripleHealth);
			}
		}
		break;
	default:
		break;
	}
}

int DrawSavedGameMenu() {
	int num_games = sdrd();
	int ret, i = 0;
	ScreenMenu Menu;
	const char* game[] = { "GAME 1", "GAME 2", "GAME 3", "GAME 4", "GAME 5",
			"GAME 6", "GAME 7", "GAME 8", "GAME 9", "GAME 10" };
	alt_up_char_buffer_dev *char_buffer;

	char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	InitializeScreenMenu(&Menu, char_buffer);
	while (i < (TOTAL_MENU_OPTIONS - 1) && i < num_games)
		AddMenuOptions(&Menu, char_buffer, game[i++]);
		AddMenuOptions(&Menu, char_buffer, "BACK");

	while (1) {
		if (IORD_32DIRECT(PUSH_BASE,0) == 0x06) {
			while (IORD_32DIRECT(PUSH_BASE,0) != 0xE)
				;
			alt_up_char_buffer_clear(char_buffer);
			ClearScreen(&Menu);
			if(Menu.cursor_pos == Menu.track) ret = (TOTAL_MENU_OPTIONS - 1); // back button
			else ret = Menu.cursor_pos;
			break;
		}
		if (Menu.movedUp == -1)
			switch (IORD_32DIRECT(PUSH_BASE,0)) {
			case 0xC:
				if (Menu.cursor_pos < Menu.track)
					Menu.movedUp = 0; // moved down
				break;
			case 0xA:
				if (Menu.cursor_pos > 0)
					Menu.movedUp = 1;
				break;
			default:
				break;
			}

		if (IORD_32DIRECT(PUSH_BASE,0) == 0xE && Menu.movedUp != -1) {
			reDrawMenu(&Menu);
			Menu.movedUp = -1;
		}
	}
	return ret;

}
int SaveGame(){
	int num_games = 5;//sdrd();
		int ret, i = 0;
		ScreenMenu Menu;
		const char* game[] = { "GAME 1", "GAME 2", "GAME 3", "GAME 4", "GAME 5",
				"GAME 6", "GAME 7", "GAME 8", "GAME 9", "GAME 10" };
		alt_up_char_buffer_dev *char_buffer;

		char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer");
		alt_up_char_buffer_init(char_buffer);
		alt_up_char_buffer_clear(char_buffer);

		InitializeScreenMenu(&Menu, char_buffer);
		while (i < (TOTAL_MENU_OPTIONS - 1) && i < num_games)
			AddMenuOptions(&Menu, char_buffer, game[i++]);
			AddMenuOptions(&Menu, char_buffer, "BACK");

		while (1) {
			if (IORD_32DIRECT(PUSH_BASE,0) == 0x06) {
				while (IORD_32DIRECT(PUSH_BASE,0) != 0xE)
					;
				alt_up_char_buffer_clear(char_buffer);
				ClearScreen(&Menu);
				if(Menu.cursor_pos == Menu.track) ret = (TOTAL_MENU_OPTIONS - 1); // back button
				else ret = Menu.cursor_pos;
				break;
			}
			if (Menu.movedUp == -1)
				switch (IORD_32DIRECT(PUSH_BASE,0)) {
				case 0xC:
					if (Menu.cursor_pos < Menu.track)
						Menu.movedUp = 0; // moved down
					break;
				case 0xA:
					if (Menu.cursor_pos > 0)
						Menu.movedUp = 1;
					break;
				default:
					break;
				}

			if (IORD_32DIRECT(PUSH_BASE,0) == 0xE && Menu.movedUp != -1) {
				reDrawMenu(&Menu);
				Menu.movedUp = -1;
			}
		}
		return ret;
}

int DrawPauseMenu(){
	ScreenMenu Menu;
		int ret;
		alt_up_char_buffer_dev *char_buffer;

		char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer");
		alt_up_char_buffer_init(char_buffer);
		alt_up_char_buffer_clear(char_buffer);

		InitializeScreenMenu(&Menu, char_buffer);

		AddMenuOptions(&Menu, char_buffer, "Resume");
		AddMenuOptions(&Menu, char_buffer, "Save Game");
		AddMenuOptions(&Menu, char_buffer, "Exit");
		while (1) {
			if (IORD_32DIRECT(PUSH_BASE,0) == 0x06) {
				while (IORD_32DIRECT(PUSH_BASE,0) != 0xE)
					;
				alt_up_char_buffer_clear(char_buffer);
				ClearScreen(&Menu);
				ret = Menu.cursor_pos;
				break;
			}
			if (Menu.movedUp == -1)
				switch (IORD_32DIRECT(PUSH_BASE,0)) {
				case 0xC:
					if (Menu.cursor_pos < Menu.track)
						Menu.movedUp = 0; // moved down
					break;
				case 0xA:
					if (Menu.cursor_pos > 0)
						Menu.movedUp = 1;
					break;
				default:
					break;
				}

			if (IORD_32DIRECT(PUSH_BASE,0) == 0xE && Menu.movedUp != -1) {
				reDrawMenu(&Menu);
				Menu.movedUp = -1;
			}
		}
		return ret;
}
/*
void SaveGame(){

}
*/
void ClearScreen(ScreenMenu *Menu) {
	int x1, y1, x2, y2;
	x1 = COLOR_X;
	y1 = COLOR_Y + (COLOR_HEIGHT * Menu->cursor_pos);
	x2 = x1 + (Menu->block[Menu->cursor_pos].strLen * CHAR_SIZE) + MARGIN;
	y2 = y1 + COLOR_HEIGHT;
	DrawBoxFPGA(x1, y1, x2, y2, SCREEN_BACKGROUND_COLOR, WITHOUT_BORDERS);
}
