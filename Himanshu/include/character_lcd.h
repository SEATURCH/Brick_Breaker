/*
 * character_lcd.h
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */

#ifndef CHARACTER_LCD_H_
#define CHARACTER_LCD_H_

#include "../include/common.h"
#include "../include/memory_map.h"
#include "altera_up_avalon_character_lcd.h"

alt_up_character_lcd_dev * open_and_init_character_lcd();
#endif /* CHARACTER_LCD_H_ */
