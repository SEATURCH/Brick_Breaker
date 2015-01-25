/*
 * brick_breaker_main.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */

#include "../include/common.h"
#include "../include/memory_map.h"
#include "../include/sd_card.h"
#include "../include/vga_graphics.h"
#include "../include/character_lcd.h"
#include "../include/timestamp_timer.h"

int main()
{
  int press;
  alt_up_character_lcd_dev * char_lcd_dev;
  alt_up_sd_card_dev* sd_dev;
  printf("Hello from Nios II!\n");

  // Open SDCard port
   sd_dev = initialize_sd_card();
   if (sd_dev == NULL){
   	 printf ("Error: could not open SDCard device\n");
   	 return 0;
   }else
   	  printf ("Opened character SDCard Device\n");

   // open the Character LCD port
   char_lcd_dev = open_and_init_character_lcd ("/dev/character_lcd_0");

   if ( char_lcd_dev == NULL){
 	 printf ("Error: could not open character LCD device\n");
 	 return 0;
   }
   else
 	  printf ("Opened character LCD device\n");

   /* Write "Welcome to" in the first row */
   alt_up_character_lcd_string(char_lcd_dev, "Exercise Number");
   /* Move Cursor */
   alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);

   while (1) {
 	  *leds = *switches;

 	  if(*switches == 0x01){
 		 press = 0;
 		 printf("Starting Exercise 5\n");
 		 alt_up_character_lcd_string(char_lcd_dev, "  5");
 		 // draw boxes forever until reset
 		draw_random_boxes_forever();
 		 alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
 		 printf("Done!\n");
 	  }else {

		  switch(*push){
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

		 if(*push == 0xE){
			 switch(press){
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
  return 0;
}

