/*
 * vga_graphics.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */


#include "../include/vga_graphics.h"

#define SCREEN_WIDTH		320
#define SCREEN_HEIGHT		240



// x - 9 bits
// y - 8 bits
void DrawBoxFPGA(int x1, int y1, int x2, int y2, int color)
{
	 IOWR_32DIRECT(DRAWER_BASE,0,x1); // Set x1
	 IOWR_32DIRECT(DRAWER_BASE,4,y1); // Set y1
	 IOWR_32DIRECT(DRAWER_BASE,8,x2); // Set x2
	 IOWR_32DIRECT(DRAWER_BASE,12,y2); // Set y2
	 IOWR_32DIRECT(DRAWER_BASE,16,color);  // Set colour
	 IOWR_32DIRECT(DRAWER_BASE,20,1);  // Start drawing
	 while(IORD_32DIRECT(DRAWER_BASE,20)==0); // wait until done
}

void draw_random_boxes_forever(){
	srand(time(NULL));


	  // VGA
	  alt_up_pixel_buffer_dma_dev* pixel_buffer;
	  //alt_up_char_buffer_dev *char_buffer;


	  // Use the name of your pixel buffer DMA core
	  pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");

	  // Set the background buffer address – Although we don’t use the background,
	  // they only provide a function to change the background buffer address, so
	  // we must set that, and then swap it to the foreground.
	  alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buff_base);

	  // Swap background and foreground buffers
	  alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

	  // Wait for the swap to complete
	  while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	  // Clear the screen
	  alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);


		int hw = 1;
			unsigned int* seed = (unsigned int*)0x666;

			int x1, y1;
			int x2, y2;

			int color;

			for(;;)
			{
				// Generate Random Values
				x1 = 0x1FF & rand_r(seed);
				x2 = 0x1FF & rand_r(seed);
			    y1 = 0xFF & rand_r(seed);
				y2 = 0xFF & rand_r(seed);
				color = 0xFFFF & rand_r(seed);


				// Check within bounds
				if(x1 >= SCREEN_WIDTH)
					x1 -= SCREEN_WIDTH;
				if(x2 >= SCREEN_WIDTH)
					x2 -= SCREEN_WIDTH;
				if(y1 >= SCREEN_HEIGHT)
					y1 -= SCREEN_HEIGHT;
				if(y2 >= SCREEN_HEIGHT)
					y2 -= SCREEN_HEIGHT;


				// Draw random values

				if (hw) {
					DrawBoxFPGA(x1, y1, x2, y2, color);
				}
				else {
					 alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x1,y1,x2,y2,color,0);
				}
			}
}

void draw_diagonal_line_with_character(){
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev *char_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer");
	alt_up_char_buffer_init(char_buffer);
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buff_base);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 0, 320, 240, 0xFFFF, 0);
	alt_up_char_buffer_string(char_buffer, "EECE 381", 40, 30);
}
