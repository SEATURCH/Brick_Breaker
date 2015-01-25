/*
 * vga_graphics.h
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */

#ifndef VGA_GRAPHICS_H_
#define VGA_GRAPHICS_H_

#include "../include/common.h"
#include "../include/memory_map.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

void DrawBoxFPGA(int x1, int y1, int x2, int y2, int color);
void draw_random_boxes_forever();
void draw_diagonal_line_with_character();

#endif /* VGA_GRAPHICS_H_ */
