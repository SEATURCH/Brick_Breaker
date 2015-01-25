/*
 * character_lcd.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */

#include "../include/character_lcd.h"

alt_up_character_lcd_dev * open_and_init_character_lcd(){
	alt_up_character_lcd_dev * char_lcd_dev;

	// open the Character LCD port
	char_lcd_dev = alt_up_character_lcd_open_dev ("/dev/character_lcd_0");

	if ( char_lcd_dev == NULL)
		return NULL;
	   else {
		   /* Initialize the character display */
		   alt_up_character_lcd_init (char_lcd_dev);
	   }
	return char_lcd_dev;
}

