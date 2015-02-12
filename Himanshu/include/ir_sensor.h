/*
 * ir_sensor.h
 *
 *  Created on: Jan 29, 2015
 *      Author: Himanshu
 */

#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_

#include "sys/alt_cache.h"

#include "altera_avalon_spi.h"
#include "altera_avalon_spi_regs.h"

#include "sys/alt_irq.h"


#define VOLT_CONV 1.39
#define VOLT_OFF 90
//Data stored in spi_read. Data range is 125 at 5.04V, 41 at 1.65V, ranging down to 0
//Takes a pointer that will then hold the returned value
//Sensor ranges at Data range is 71 at 3.13V, 21 at 1.09V
int paddleposition(char *spi_read);


#endif /* IR_SENSOR_H_ */
