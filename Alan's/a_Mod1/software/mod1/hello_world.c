#include "alt_types.h"
#include "sys/alt_stdio.h"
#include "io.h"
#include "system.h"
#include "sys/alt_cache.h"

#include "altera_avalon_spi.h"
#include "altera_avalon_spi_regs.h"

#include "sys/alt_irq.h"

int main()
{
  alt_printf("Hello from Nios II!\n");

  int return_code;
  //Arbitrary Command MOSI
  char spi_command_string_tx[10] = "$HELLOABC*";

  //Data buffer and initialization
  int* spi_read ;
  spi_read = (int*) malloc(1);
  *spi_read = 0;

while(1){
  return_code = alt_avalon_spi_command(SPI_BASE,0,
                                1, spi_command_string_tx,
                                1, spi_read,
                                0);

 printf("%d", *spi_read );
 printf("\n");

  /*Data stored in spi_read
    Data range is 125 at 5.04V
				  41 at 1.65V
	ranging down to 0
  */
  }

  return 0;

}
