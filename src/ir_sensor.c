/*
 * ir_sensor.c
 *
 *  Created on: Jan 29, 2015
 *      Author: Himanshu
 */

#include "../include/ir_sensor.h"
#include <system.h>
#define bit_flipper_base	(volatile int *)	BIT_FLIPPER_0_BASE
#define TOP_BOUND 271
#define BOT_BOUND 0

int init = 0;

int paddleposition(char *spi_read){

	//	all registers to zero
	if(init == 0){
		 *(bit_flipper_base)	=	0x00000;
		 init++;
	 }
	//Arbitrary Command MOSI
	 char spi_command_string_tx[10] = "0000000000";
	 alt_avalon_spi_command(SPI_BASE,0, 1, spi_command_string_tx, 1, spi_read, 0);
	 int input = (VOLT_CONV*((6787.00 / ((unsigned char)*spi_read- 3.00) - 4.00 )- VOLT_OFF));

	 if(input<BOT_BOUND)
			 input = BOT_BOUND;
	 else if(input > TOP_BOUND)
		 	 input = TOP_BOUND;

	*(bit_flipper_base+1) = input;

	 return *(bit_flipper_base+1);
}
