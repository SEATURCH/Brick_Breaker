#include "alt_types.h"
#include "sys/alt_stdio.h"
#include "io.h"
#include "system.h"
#include "sys/alt_cache.h"

#include "altera_avalon_spi.h"
#include "altera_avalon_spi_regs.h"

#include "sys/alt_irq.h"

#define VOLT_CONV 2.406
#define VOLT_OFF 97
//Data stored in spi_read. Data range is 125 at 5.04V, 41 at 1.65V, ranging down to 0
//Takes a pointer that will then hold the returned value
//Sensor ranges at Data range is 71 at 3.13V, 21 at 1.09V
int paddleposition(int *spi_read){
	  //Arbitrary Command MOSI
	 char spi_command_string_tx[10] = "$HELLOABC*";
	 alt_avalon_spi_command(SPI_BASE,0, 1, spi_command_string_tx, 1, spi_read, 0);
	 return VOLT_CONV*((6787.00 / (*spi_read - 3.00)) - 4.00 - VOLT_OFF);
}

int main()
{
  //Data buffer and initialization
  int position, reading;
  int* spi_read ;
  spi_read = (int*) malloc(1);
  *spi_read = 0;



while(1){
	 position = paddleposition(spi_read);
	 reading = *spi_read;
	 printf("Reading: %d  			Position %d\n", reading, position );
  }


return 0;

}
