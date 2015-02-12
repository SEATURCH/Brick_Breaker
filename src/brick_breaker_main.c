/*
 * brick_breaker_main.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 *  Modified on: Jan 25, 2015
 *  	Author: Alan
 *  Modified on: Jan 28, 2015
 *	Author: Francis
 */

#include "../include/common.h"
#include "../include/memory_map.h"
#include "../include/sd_card.h"
#include "../include/vga_graphics.h"
#include "../include/character_lcd.h"
#include "../include/timestamp_timer.h"
#include "../include/import_export.h"
#include "../include/ball.h"

#define PERFORM_EXERCISES 0
#define PERFORM_EXPORTIMPORT 1

// Returns:
// 0 if not successful
// non-zero if successful
int InitializeComponents(alt_up_sd_card_dev* sd_dev,
		alt_up_character_lcd_dev * char_lcd_dev,
		alt_up_pixel_buffer_dma_dev* pixel_buffer,
		RenderObjectStructure *renderObjectStructure,
		BlockObjectStructure *blockObjectStructure, Paddle* paddle, Ball *ball) {
	int ret = 1;

	// Open SDCard port
	sd_dev = initialize_sd_card();
	if (sd_dev == NULL) {
		printf("Error: could not open SDCard device\n");
		ret = 0;
	} else
		printf("Opened character SDCard Device\n");

	// open the Character LCD port
	char_lcd_dev = open_and_init_character_lcd("/dev/character_lcd_0");

	if (char_lcd_dev == NULL) {
		printf("Error: could not open character LCD device\n");
		ret = 0;
	} else {
		printf("Opened character LCD device\n");
	}

	// Open VGA
	if (!InitializeVGA(pixel_buffer)) {
		ret = 0;
	}

	// Open render structure
	InitializeRenderObjectStructure(renderObjectStructure);
	InitializeBlockObjectStructure(blockObjectStructure, renderObjectStructure);
	InitializePaddle(paddle);
	InitializeBall(ball);

	return ret;
}

int main() {
	alt_up_sd_card_dev* sd_dev;
	alt_up_character_lcd_dev * char_lcd_dev;
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	RenderObjectStructure renderObjectStructure;
	BlockObjectStructure blockObjectStructure;
	Paddle paddle;
	Ball ball;
	int* spi_read;
	spi_read = (int*) malloc(sizeof(int));
	unsigned int currentFrame = 0;

	// Surpress the 'unused' warnings
	(void) sd_dev;
	(void) char_lcd_dev;
	(void) pixel_buffer;
	(void) renderObjectStructure;
	(void) blockObjectStructure;

	printf("Game Start!\n");

	if (!InitializeComponents(sd_dev, char_lcd_dev, pixel_buffer,
			&renderObjectStructure, &blockObjectStructure, &paddle, &ball)) {
		printf("Error with initialization\n");
		return 0;
	}
#if 1
	// SAMPLE SUBROUTINE: Checkerboard
	{
		int i, j;
		for (j = 0; (j + DEFAULT_BLOCK_HEIGHT) < NUM_RENDER_OBJECTS_HEIGHT;
				j += 3 * DEFAULT_BLOCK_HEIGHT) {
			for (i = 0;
					(i + DEFAULT_BLOCK_WIDTH) < NUM_RENDER_OBJECTS_WIDTH;
					i += 3 * DEFAULT_BLOCK_WIDTH) {
				AddBlock(&blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
						DEFAULT_BLOCK_HEIGHT, TripleHealth);
			}
		}
	}

	/*{
	 int i, j;
	 for (j = 5; (j + DEFAULT_BLOCK_HEIGHT - 1) < 10; j += 2) {
	 for (i = 10; (i + DEFAULT_BLOCK_WIDTH - 1) < 20; i += 3) {
	 AddBlock(&blockObjectStructure, i, j, DEFAULT_BLOCK_WIDTH,
	 DEFAULT_BLOCK_HEIGHT, SingleHealth);
	 }
	 }
	 }*/

	// Map the blocks that were just added
	MapBlockObjectStructureToRender(&blockObjectStructure,
			&renderObjectStructure);

	// Draw the set of blocks to screen
	// This only has to be done once
	DrawRenderObjectStructure(&renderObjectStructure);
#endif
#if 0
 // DisabledPERFORM_EXPORTIMPORT
	// ** PURELY TESTING OF IMPORT/EXPORT ** //
	// This function exports the current block structure, and then reads it back in
	{
#define BUFFERLEN 500
		unsigned char buffer[BUFFERLEN] = {0};
		unsigned char buffin[BUFFERLEN] = {0};
		// Export the block data structure
		ExportBlockDataStructure(&renderObjectStructure, &blockObjectStructure,
				buffer, BUFFERLEN);
		//sdwr("LVL1.bin", buffer);

		lvlselect(char_lcd_dev, buffin);
		// Import the just-exported block data structure
		// This resets the render and block object structures and
		// sets them to the previous set-up
		ImportBlockDataStructure(&renderObjectStructure, &blockObjectStructure,
				buffin, BUFFERLEN);

	}
#endif

	// MAIN LOOP GOES HERE
	while (1) {
		MovePaddle(&paddle,paddleposition(spi_read));
		MoveBall(&renderObjectStructure, &blockObjectStructure, &paddle, &ball,
				currentFrame);

		currentFrame++;

	}

#if PERFORM_EXERCISES == 1
	int press;
	/* Write "Welcome to" in the first row */
	alt_up_character_lcd_string(char_lcd_dev, "Exercise Number");
	/* Move Cursor */
	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);

	if(*switches == 0x01) {
		press = 0
		while (1) {
			*leds = *switches;;
			printf("Starting Exercise 5\n");
			alt_up_character_lcd_string(char_lcd_dev, "  5");
			// draw boxes forever until reset
			draw_random_boxes_forever();
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
			printf("Done!\n");
		} else {

			switch(*push) {
				case 0xC:
				press = 1;
				break;
				case 0xA:
				press = 2;
				break;
				case 0x6:
				press = 3;
				break;
				default:
				break;
			}

			if(*push == 0xE) {
				switch(press) {
					case 1 :
					press = 0;
					printf("Starting Exercise 2\n");
					alt_up_character_lcd_string(char_lcd_dev, "  2");
					exer2();
					alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
					printf("Done!\n");
					break;
					case 2 :
					press = 0;
					printf("Starting Exercise 3\n");
					alt_up_character_lcd_string(char_lcd_dev, "  3");
					sdrd();
					alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
					printf("Done!\n");
					break;
					case 3:
					press = 0;
					printf("Starting Exercise 4\n");
					alt_up_character_lcd_string(char_lcd_dev, "  4");
					draw_diagonal_line_with_character();
					alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
					printf("Done!\n");
					break;
					default:
					break;
				}
			}
		}
	}

#endif

	return 0;
}

