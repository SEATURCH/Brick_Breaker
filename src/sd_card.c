/*
 * sd_card.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */

#include "../include/sd_card.h"

alt_up_sd_card_dev* initialize_sd_card(){
	return alt_up_sd_card_open_dev("/dev/SDCard");
}

//Writes a file named after parameter with contents inside buffer
void sdwr(char* name){
	short int sd_fileh;
	char buffer[512] = "WELCOME TO THE INTERFACE!!\r\n\0";
	sd_fileh = alt_up_sd_card_fopen(name, true);

	if (sd_fileh < 0)
	  printf("Problem creating file. Error %i", sd_fileh);
	else
	{
	  printf("SD Accessed Successfully, writing data...");
	  int index = 0;
	  while (buffer[index] != '\0')
	  {
		  alt_up_sd_card_write(sd_fileh, buffer[index]);
		  index = index + 1;
	  }
	  printf("Closing File...");
	  alt_up_sd_card_fclose(sd_fileh);
	  printf("Done!\n");
	}

}

void sdrd(){
	short int sd_fileh;
	char* bfile;
		if( (alt_up_sd_card_is_Present()) ) {
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");
			} else {
			  printf("Unknown file system.\n");
			}
			sd_fileh = alt_up_sd_card_find_first( "", bfile);
			printf(bfile);
			printf("\n");
		 /*
		  } else if( (connected == 1) && (alt_up_sd_card_is_Present() == false)) {
			  printf("Card disconnected.\n");
			  connected = 0;
		 */
		  } else{
			  printf("SD CARD NOT CONNECTED");
			  printf("\n");
		  }
		while( (sd_fileh = alt_up_sd_card_find_next(bfile)) == 0 ) {
			 printf(bfile);
			 printf("\n");
		  }
}
