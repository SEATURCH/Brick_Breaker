/*
 * sd_card.h
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */

#ifndef SD_CARD_H_
#define SD_CARD_H_

#include "../include/common.h"
#include "../include/memory_map.h"
#include "Altera_UP_SD_Card_Avalon_Interface.h"

//void prep(char* name);
alt_up_sd_card_dev* initialize_sd_card();
void sdwr(char* name, unsigned char* buffer);
int sdrd();
void readfile(int ind, unsigned char * buff);
void prep(char* name, int index);
#endif /* SD_CARD_H_ */
