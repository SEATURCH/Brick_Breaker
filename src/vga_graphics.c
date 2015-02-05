/*
 * vga_graphics.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 *  Modified on: Jan 25, 2015
 *  	Author: Alan
 *  Modified on: Jan 28, 2015
 *	Author: Francis
 */

#include "../include/vga_graphics.h"
#include "../include/timestamp_timer.h"

// x - 9 bits
// y - 8 bits
void DrawBoxFPGA(int x1, int y1, int x2, int y2, int color) {
	IOWR_32DIRECT(DRAWER_BASE, 0, x1);
	// Set x1
	IOWR_32DIRECT(DRAWER_BASE, 4, y1);
	// Set y1
	IOWR_32DIRECT(DRAWER_BASE, 8, x2);
	// Set x2
	IOWR_32DIRECT(DRAWER_BASE, 12, y2);
	// Set y2
	IOWR_32DIRECT(DRAWER_BASE, 16, color);
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

	// Start drawing
	IOWR_32DIRECT(DRAWER_BASE, 20, 1);

	// wait until done
	while (IORD_32DIRECT(DRAWER_BASE, 20) == 0)
		;
}

void DrawFPGAPaddleObject(Paddle* paddle, int color) { //The color is added just so we can erase the paddle
	DrawBoxFPGA(paddle->x_pos, paddle->y_pos, paddle->x_pos + paddle->width,
			paddle->y_pos + paddle->height, color);
}
void DrawBallObjectMovement(BlockObjectStructure *blockObjectStructure,
		RenderObjectStructure *renderObjectStructure, Paddle *paddle) {
	int cursorY = 72;
	int cursorX = 72;
	int Xspeed = BALL_SPEED;
	int Yspeed = BALL_SPEED;
	int Xdir = 1;
	int Ydir = 1;
	int oldCursorX = cursorX;
	int oldCursorY = cursorY;
	BlockObject *blockObject;
	int L1, L2, R1, R2, offset, temp_moveX, temp_moveY, temp_index, temp1 = 0,
			temp2;
	int potential_L1_index, potential_L2_index, potential_R1_index,
			potential_R2_index, old_L1_index;

	int collision = 0; // assume
	int paddle_collision = 0;
	int position;
	int* spi_read;
	spi_read = (int*) malloc(1);
	*spi_read = 0;
	while (1) {
		sleep(1);
		L1 = (L2) = (R1) = (R2) = -1;
		// use SCREEN_HEIGHT_BLOCKAREA instead of BLOCK_AREA_LIMIT
		// because ball can come from below (outside block area) as well
		if (collision) {
			collision = 0;
		}
		if (paddle_collision) {
			paddle_collision = 0;
		}
		oldCursorX = cursorX;
		oldCursorY = cursorY;
		old_L1_index = ((cursorY / 8) * (40)) + (cursorX / 8); // ? need to modify - find x and y
		cursorX += (Xspeed * Xdir);
		cursorY += (Yspeed * Ydir);
		if (cursorY <= BLOCK_AREA_LIMIT && cursorY >= 0 && cursorX <= X_LIMIT
				&& cursorX >= 0) {
			//define new potential position

			potential_L1_index = ((cursorY / 8) * (40)) + (cursorX / 8);
			potential_L2_index = (((cursorY + BALL_OBJECT_HEIGHT) / 8) * (40))
					+ (cursorX / 8);
			potential_R1_index = ((cursorY / 8) * (40))
					+ ((cursorX + BALL_OBJECT_WIDTH) / 8);
			potential_R2_index = (((cursorY + BALL_OBJECT_HEIGHT) / 8) * (40))
					+ ((cursorX + BALL_OBJECT_WIDTH) / 8);
			// not dealing with x = 0 or y = 0 yet
			if (Xdir > 0 && Ydir > 0)
				offset = 41;
			if (Xdir < 0 && Ydir > 0) {
				offset = 39;
				potential_R1_index = potential_L1_index;
				temp_index = potential_R2_index;
				potential_R2_index = potential_L2_index;
				potential_L2_index = temp_index;
			}
			if (Xdir > 0 && Ydir < 0) {
				offset = -39;
				potential_R2_index = potential_L1_index;
				temp_index = potential_R1_index;
				potential_R1_index = potential_L2_index;
				potential_L2_index = temp_index;
			}
			if (Xdir < 0 && Ydir < 0) {
				offset = -41;
				potential_R2_index = potential_L1_index;
				temp_index = potential_R1_index;
				potential_R1_index = potential_L2_index;
				potential_L2_index = temp_index;
			}

			if (potential_R2_index
					>= 0&& potential_R2_index < NUM_RENDER_OBJECTS_TOTAL) {
				R2 = DetectCollision(potential_R2_index, oldCursorX, oldCursorY,
						offset, renderObjectStructure, blockObjectStructure);
			}

			if (potential_L2_index
					>= 0&& potential_L2_index < NUM_RENDER_OBJECTS_TOTAL) {
				L2 = DetectCollision(potential_L2_index, oldCursorX, oldCursorY,
						offset, renderObjectStructure, blockObjectStructure);
			}

			if (potential_R1_index
					>= 0&& potential_R1_index < NUM_RENDER_OBJECTS_TOTAL) {
				R1 = DetectCollision(potential_R1_index, oldCursorX, oldCursorY,
						offset, renderObjectStructure, blockObjectStructure);
			}

			if (L2 > -1) {
				blockObject = &blockObjectStructure->blockObjects[L2];
				temp_moveY = (blockObject->blockYStart * RENDER_OBJECT_HEIGHT)
						- (cursorY + BALL_OBJECT_HEIGHT);
				temp_moveX = temp_moveY;
			}
			if (R1 > -1) {
				blockObject = &blockObjectStructure->blockObjects[R1];
				temp_moveX = (blockObject->blockXStart * RENDER_OBJECT_WIDTH)
						- (cursorX + BALL_OBJECT_WIDTH);
				temp_moveY = temp_moveX;
			}
			if (R2 > -1) {
				blockObject = &blockObjectStructure->blockObjects[R1];
				temp_moveY = blockObject->blockYStart - BALL_OBJECT_HEIGHT;
				temp_moveX = blockObject->blockXStart - BALL_OBJECT_WIDTH;
			}
			if (R1 > -1 || L2 > -1 || R2 > -1) {
				collision = 1;
				cursorX += (temp_moveX * Xdir);
				cursorY += (temp_moveY * Ydir);
			}
			if (L2 > -1 && R1 > -1) {
				Ydir *= -1;
				Xdir *= -1;
			} else if (L2 > -1 || R2 > -1) {
				Ydir *= -1;
			} else if (R1 > -1 || R2 > -1) {
				Xdir *= -1;
			} else if (R2 > -1) {
				Ydir *= -1;
				Xdir *= -1;
			}
		}
		// position = paddleposition(spi_read);
		// MovePaddle(paddle, position); // use it with IR sensor
		if (*switches == 0x01 && temp1 <= X_LIMIT) {
			temp1 += 4;
		} else if (*switches == 0x00 && temp1 >= 4) {
			temp1 -= 4;
		}
		MovePaddle(paddle, temp1);
		if (cursorY == (INITIAL_PADDLE_Y_POS - BALL_OBJECT_HEIGHT)
				&& cursorX < (paddle->x_pos + DEFAULT_PADDLE_WIDTH)
				&& cursorX >= (paddle->x_pos - BALL_OBJECT_WIDTH)) {
			/*	temp1 = cursorX - paddle->x_pos;
			 temp2 = DEFAULT_PADDLE_WIDTH - BALL_OBJECT_WIDTH;
			 if(temp1 < 0){
			 Xdir = -1;
			 }else if(temp1 > temp2){
			 Xdir = 1;
			 }else{
			 Xdir = ((temp1*10)/temp2) - 1;
			 }*/
			Ydir *= -1;
			paddle_collision = 1;
		}
		if (!collision && !paddle_collision) {
			if (cursorY >= Y_LIMIT || cursorY <= 0) {
				Ydir *= -1;
			}
			if (cursorX >= X_LIMIT || cursorX <= 0) {
				Xdir *= -1;
			}
			// GOTO

		}
		DrawFPGABallObject(oldCursorX, oldCursorY, SCREEN_BACKGROUND_COLOR);
		DrawFPGABallObject(cursorX, cursorY, BALL_COLOUR);
		if (collision) {
			//DrawBoxFPGA(oldCursorX, oldCursorY, (oldCursorX + temp_moveX), (oldCursorY + temp_moveY), color);
			EraseBlockAndResetRenderObjects(L2, blockObjectStructure,
					renderObjectStructure);
			EraseBlockAndResetRenderObjects(R1, blockObjectStructure,
					renderObjectStructure);
			EraseBlockAndResetRenderObjects(R2, blockObjectStructure,
					renderObjectStructure);
		}
	}

	int DetectCollision(int index, int oldCursorX, int oldCursorY, int offset,
			RenderObjectStructure *renderObjectStructure,
			BlockObjectStructure *blockObjectStructure) {
		int blockCursor = renderObjectStructure->blockNumber[index];
		BlockObject *blockObject =
				&blockObjectStructure->blockObjects[blockCursor];
		int Xstart = blockObject->blockXStart * RENDER_OBJECT_WIDTH;
		int Ystart = blockObject->blockYStart * RENDER_OBJECT_HEIGHT;
		int ret = -1;
		while ((blockCursor != -1)
				&& (Xstart > oldCursorX || Ystart > oldCursorY)) { // ther's a block and condition met
			ret = blockCursor; // will collide with ball
			//collision = 1;
			index -= offset;
			if (index < 0 || index > NUM_RENDER_OBJECTS_TOTAL) {
				break;
			}
			blockCursor = renderObjectStructure->blockNumber[index];
			blockObject = &blockObjectStructure->blockObjects[blockCursor];
		}
		//collide with render object at L2
		index += 41; // true index - if want to use in future
		return ret;
	}

	void DrawFPGARenderObject(int renderObjectStartX, int renderObjectStartY,
			int color) {
		// Set the coordinates of the render object
		IOWR_32DIRECT(DRAWER_BASE, 0, renderObjectStartX);
		IOWR_32DIRECT(DRAWER_BASE, 4, renderObjectStartY);

		IOWR_32DIRECT(DRAWER_BASE, 8, renderObjectStartX + RENDER_OBJECT_WIDTH);
		IOWR_32DIRECT(DRAWER_BASE, 12,
				renderObjectStartY + RENDER_OBJECT_HEIGHT);

		// Set colour
		IOWR_32DIRECT(DRAWER_BASE, 16, color);

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

		for (cursorY = 0;
				cursorY < renderObjectStructure->numRenderObjectsHeight;
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
		pixel_buffer = alt_up_pixel_buffer_dma_open_dev(
				"/dev/pixel_buffer_dma");
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
		for (renderObjectCursor = 0;
				renderObjectCursor < NUM_RENDER_OBJECTS_TOTAL;
				renderObjectCursor++) {
			renderObjectStructure->color[renderObjectCursor] =
					SCREEN_BACKGROUND_COLOR; //renderObjectCursor * 800;
			renderObjectStructure->blockNumber[renderObjectCursor] = -1;
		}

		// Set the stride so that the block object structure can stride through multiple rows
		renderObjectStructure->numRenderObjectsWidth = NUM_RENDER_OBJECTS_WIDTH;
		renderObjectStructure->numRenderObjectsHeight =
				NUM_RENDER_OBJECTS_HEIGHT;

	}

	void InitializeBlockObjectStructure(
			BlockObjectStructure * blockObjectStructure,
			RenderObjectStructure *renderObjectStructure) {
		// Iterate through the entire possible scratchpad, making each block inactive
		int blockObjectCursor;
		for (blockObjectCursor = 0; blockObjectCursor < DEFAULT_MAX_BLOCKS;
				blockObjectCursor++) {
			blockObjectStructure->blockObjects[blockObjectCursor].isActive =
					FALSE;
		}
		blockObjectStructure->numBlocksSet = 0;
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
			int blockHeight, int blockType) {
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

	void EraseBlockAndResetRenderObjects(int blockCursor,
			BlockObjectStructure *blockObjectStructure,
			RenderObjectStructure *renderObjectStructure) {

		if (blockCursor > -1) {
			int index = -1;
			BlockObject *blockObject =
					&blockObjectStructure->blockObjects[blockCursor];
			if (blockObject->isActive == FALSE)
				return;
			int renderObjectWidth = blockObject->blockWidthRenderObjects;
			int renderObjectHeight = blockObject->blockHeightRenderObjects;
			int blockXStart = blockObject->blockXStart;
			int blockYStart = blockObject->blockYStart;
			int xCursor, yCursor, x1, x2, y1, y2;
			blockObject->isActive = FALSE;
			x1 = blockXStart * RENDER_OBJECT_WIDTH;
			y1 = blockYStart * RENDER_OBJECT_HEIGHT;
			x2 = x1 + (renderObjectWidth * RENDER_OBJECT_WIDTH);
			y2 = y1 + (renderObjectHeight * RENDER_OBJECT_HEIGHT);

			DrawBoxFPGA(x1, y1, x2, y2, SCREEN_BACKGROUND_COLOR);

			for (yCursor = 0; yCursor < renderObjectHeight; yCursor++) {
				int renderObjectCursor = (yCursor + blockYStart)
						* renderObjectStructure->numRenderObjectsWidth;
				for (xCursor = 0; xCursor < renderObjectWidth; xCursor++) {
					index = renderObjectCursor + (xCursor + blockXStart);
					renderObjectStructure->color[index] =
							SCREEN_BACKGROUND_COLOR;
					renderObjectStructure->blockNumber[index] = -1;
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

// Test func
	void SetRandomColors(RenderObjectStructure *renderObjectStructure) {
		// Set the color of all render objects to a pseudo-random color based off the cursor
		int renderObjectCursor;
		for (renderObjectCursor = 0;
				renderObjectCursor < NUM_RENDER_OBJECTS_TOTAL;
				renderObjectCursor++) {
			renderObjectStructure->color[renderObjectCursor] =
					renderObjectCursor * 800;
		}
	}

	void SetBlack(RenderObjectStructure *renderObjectStructure) {
		// Set the color of all render objects to a pseudo-random color based off the cursor
		int renderObjectCursor;
		for (renderObjectCursor = 0;
				renderObjectCursor < NUM_RENDER_OBJECTS_TOTAL;
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
		pixel_buffer = alt_up_pixel_buffer_dma_open_dev(
				"/dev/pixel_buffer_dma");

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
				DrawBoxFPGA(x1, y1, x2, y2, color);
			} else {
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x1, y1, x2, y2,
						color, 0);
			}
		}
	}

	void draw_diagonal_line_with_character() {
		alt_up_pixel_buffer_dma_dev* pixel_buffer;
		alt_up_char_buffer_dev *char_buffer;
		pixel_buffer = alt_up_pixel_buffer_dma_open_dev(
				"/dev/pixel_buffer_dma");
		char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer");
		alt_up_char_buffer_init(char_buffer);
		alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
				pixel_buff_base);
		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
			;
		alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 0, 320, 240, 0xFFFF,
				0);
		alt_up_char_buffer_string(char_buffer, "EECE 381", 40, 30);
	}
