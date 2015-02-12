
#include "../include/lvl_select.h"



void lvlselect(alt_up_character_lcd_dev * char_lcd_dev, unsigned char* buffin){
	int press, index=0;
	lvlamount = sdrd();

	char_lcd_dev = open_and_init_character_lcd("/dev/character_lcd_0");
	alt_up_character_lcd_string(char_lcd_dev, "Level Select");
	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
	alt_up_character_lcd_string(char_lcd_dev, level[index]);

	while(1){
	switch(*push) {
		case 0xC:
		press = DONE;
		break;
		case 0xA:
		press = GO_DOWN;
		break;
		case 0x6:
		press = GO_UP;
		break;
		default:
		break;
	}

	if(*push == 0xE) {
		switch(press) {
		case DONE :
			printf("Starting Game!\n");
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 0);
			alt_up_character_lcd_string(char_lcd_dev, "Starting Game! ");
			break;
		case GO_DOWN :
			press = 0;
			if( (index=index-1) < 0 )
					index = lvlamount-1;
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
			alt_up_character_lcd_string(char_lcd_dev, level[index]);
			break;
		case GO_UP:
			press = 0;
			index = (index+1)%lvlamount;
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
			alt_up_character_lcd_string(char_lcd_dev, level[index]);
			break;
		default:
			break;
		}
	}
	if(press == DONE)
		break;
	}
	//printf("%s", level_map[index]);
	readfile( index, buffin);

}
