#include "../include/sd_card.h"
#include "../include/character_lcd.h"

#define LVLAMOUNT 		2
#define GO_UP	 		3
#define GO_DOWN 		2
#define DONE	 		1

const char* level[] = { "LVL 1", "LVL 2" };
const char* level_map[] = { "aAl.bin", "aAl.bin" };


void lvlselect(alt_up_character_lcd_dev * char_lcd_dev, unsigned char* buffin);
