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

alt_up_sd_card_dev* initialize_sd_card();
void sdwr(char* name);
void sdrd();

#endif /* SD_CARD_H_ */
