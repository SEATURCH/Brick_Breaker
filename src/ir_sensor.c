/*
 * ir_sensor.c
 *
 *  Created on: Jan 29, 2015
 *      Author: Himanshu
 */

#include "../include/ir_sensor.h"

int paddleposition(int *spi_read){
	  //Arbitrary Command MOSI
	 char spi_command_string_tx[10] = "$HELLOABC*";
	 alt_avalon_spi_command(SPI_BASE,0, 1, spi_command_string_tx, 1, spi_read, 0);
	 return VOLT_CONV*((6787.00 / (*spi_read - 3.00)) - 4.00 - VOLT_OFF);
}
