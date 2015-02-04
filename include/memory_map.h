/*
 * memory_map.h
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */

#ifndef MEMORY_MAP_H_
#define MEMORY_MAP_H_

#define switches 		(volatile char *) SWITCHES_BASE
#define leds 			(char *) LEDS_BASE
#define push 			(char *) PUSH_BASE
#define pixel_buff_base	(volatile int) PIXEL_BUFFER_BASE
#define DRAWER_BASE		(volatile int) PIXEL_DRAWER_BASE


#endif /* MEMORY_MAP_H_ */
