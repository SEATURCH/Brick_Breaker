/*
 * brick_breaker_main.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 *  Modified on: Jan 25, 2015
 *  	Author: Alan
 */

#include "../include/common.h"
#include "../include/memory_map.h"
#include "../include/sd_card.h"
#include "../include/vga_graphics.h"
#include "../include/character_lcd.h"
#include "../include/timestamp_timer.h"

#define PERFORM_EXERCISES 0

// Returns:
// 0 if not successful
// non-zero if successful
int InitializeComponents(alt_up_sd_card_dev* sd_dev,
		alt_up_character_lcd_dev * char_lcd_dev,
		alt_up_pixel_buffer_dma_dev* pixel_buffer,
		RenderObjectStructure *renderObjectStructure,
		BlockObjectStructure *blockObjectStructure) {
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

	return ret;
}

int main() {
	alt_up_sd_card_dev* sd_dev;
	alt_up_character_lcd_dev * char_lcd_dev;
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	RenderObjectStructure renderObjectStructure;
	BlockObjectStructure blockObjectStructure;

	// Surpress the 'unused' warnings
	(void) sd_dev;
	(void) char_lcd_dev;
	(void) pixel_buffer;
	(void) renderObjectStructure;
	(void) blockObjectStructure;

	printf("Game Start!\n");

	if (!InitializeComponents(sd_dev, char_lcd_dev, pixel_buffer,
			&renderObjectStructure, &blockObjectStructure)) {
		printf("Error with initialization\n");
		return 0;
	}

	// This sets the render object's to pseudo-random colors.
	// Use this to test that display is working correctly.
	{
		//SetRandomColors(&renderObjectStructure);
		SetBlack(&renderObjectStructure);
	}

	// GENERATE BLOCK STRUCTURE HERE
	// Additional AddBlock functions can be added in the future
	//  i.e. add power-up block, or add a smaller sized block, etc

	// SAMPLE SUBROUTINE: Checkerboard
/*	{
		int i, j;
		for (j = 0; (j + DEFAULT_BLOCK_HEIGHT - 1) < NUM_RENDER_OBJECTS_HEIGHT;
				j += 3) {
			for (i = 0;
					(i + DEFAULT_BLOCK_WIDTH - 1) < NUM_RENDER_OBJECTS_WIDTH;
					i += 4) {
				AddBlock(&blockObjectStructure, i, j);
			}
		}
	}*/

	{
			int i, j;
			for (j = 5; (j + DEFAULT_BLOCK_HEIGHT - 1) < 10;
					j ++) {
				for (i = 10;
						(i + DEFAULT_BLOCK_WIDTH - 1) < 20;
						i ++) {
					AddBlock(&blockObjectStructure, i, j);
				}
			}
		}

	// Map the blocks that were just added
	MapBlockObjectStructureToRender(&blockObjectStructure,
			&renderObjectStructure);

	// Draw the set of blocks to screen
	// This only has to be done once
	DrawRenderObjectStructure(&renderObjectStructure);

	// MAIN LOOP GOES HERE

	{
		// One loop iteration: 1 frame


		// Calculate ball movement

		// Erase last ball position

		// Draw new ball position

		// The size of the ball is up to you, but it should be <= the size of a render object (RENDER_OBJECT_WIDTH)
		// Use CPU function to draw square ball:
		//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x1, y1, x2, y2,
		//		color, 0);

		DrawBallObjectMovement(&renderObjectStructure); // still hardcoded
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

