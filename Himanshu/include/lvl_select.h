#include "../include/sd_card.h"
#include "../include/character_lcd.h"


#define GO_UP	 		3
#define GO_DOWN 		2
#define DONE	 		1

int lvlamount = 0;
const char* level[] = { "LVL 1", "LVL 2", "LVL 3", "LVL 4", "LVL 5", "LVL 6", "LVL 7", "LVL 8", "LVL 9", "LVL 10"  };

void lvlselect(alt_up_character_lcd_dev * char_lcd_dev, unsigned char* buffin);
