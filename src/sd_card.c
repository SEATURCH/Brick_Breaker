/*
 * sd_card.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */

#include "../include/sd_card.h"
#define BUFFERLEN 500
const char* level_map[10] = { 0 };
int size = 0;
alt_up_sd_card_dev* initialize_sd_card(){
	return alt_up_sd_card_open_dev("/dev/SDCard");
}

//Writes a file named after parameter with contents inside buffer
void sdwr(char* name, unsigned char *buff){
	short int sd_fileh;
	//char buffer[512] = "WELCOME TO THE INTERFACE!!\r\n\0";
	 printf("SD Card Access Test\n");

	      if(alt_up_sd_card_is_Present())
	      {
	          if(alt_up_sd_card_is_FAT16())
	              printf("Card is FAT16\n");
	          else
	              printf("Card is not FAT16\n");

	          sd_fileh = alt_up_sd_card_fopen(name, true);

	          if (sd_fileh < 0)
	              printf("Problem creating file. Error %i", sd_fileh);
	          else
	          {
	              printf("SD Accessed Successfully, writing data...");
	              int index = 0;
	              while(index <BUFFERLEN) {
	                  alt_up_sd_card_write(sd_fileh, buff[index]);
                      index = index + 1;
	              }
	              printf("Done!\n");

	              printf("Closing File...");
	              alt_up_sd_card_fclose(sd_fileh);
	              printf("Done!\n");
	          }
	      }

}

void sdwr2(char* name, unsigned char *buff){
	short int sd_fileh;
	//char buffer[512] = "WELCOME TO THE INTERFACE!!\r\n\0";
	 printf("SD Card Access Test\n");

	      if(alt_up_sd_card_is_Present())
	      {
	          if(alt_up_sd_card_is_FAT16())
	              printf("Card is FAT16\n");
	          else
	              printf("Card is not FAT16\n");

	          sd_fileh = alt_up_sd_card_fopen(name, true);

	          if (sd_fileh < 0){
	              printf("Problem creating file. Error %i", sd_fileh);
	              sd_fileh = alt_up_sd_card_fopen(name, false);
	          }

	              printf("SD Accessed Successfully, writing data...");
	              int index = 0;
	              while(index <BUFFERLEN) {
	                  alt_up_sd_card_write(sd_fileh, buff[index]);
                      index = index + 1;
	              }
	              printf("Done!\n");

	              printf("Closing File...");
	              alt_up_sd_card_fclose(sd_fileh);
	              printf("Done!\n");

	      }

}

int sdrd(){
	int i, j=0;
	short int sd_fileh;
	char bfile[8];
		if( (alt_up_sd_card_is_Present()) ) {
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");
			} else {
			  printf("Unknown file system.\n");
			}
			sd_fileh = alt_up_sd_card_find_first( "", bfile);
			//printf(bfile);
			//printf("\n");
		 /*
		  } else if( (connected == 1) && (alt_up_sd_card_is_Present() == false)) {
			  printf("Card disconnected.\n");
			  connected = 0;
		 */
			while( (sd_fileh = alt_up_sd_card_find_next(bfile)) == 0 ) {
				for(i =0; ((bfile[i]) != '.'); i++){
					if( (bfile[i+1] =='.') && (bfile[i+2] == 'B')){
						//printf(bfile);
						level_map[j] =malloc(8);
						strcpy(level_map[j], bfile);
						j++;
						break;
					}
				}
			}
		  } else{
			  printf("SD CARD NOT CONNECTED");
			  printf("\n");
		  }

		return j;
}

void readfile(int ind, unsigned char * buff){
	short int sd_fileh;
	char byte;
	char *name =  level_map[ind];
	 if(alt_up_sd_card_is_Present())
		      {
		          sd_fileh = alt_up_sd_card_fopen(name, false);

		          if (sd_fileh < 0)
		              printf("Problem opening file. Error %i", sd_fileh);
		          else
		          {
		              int index = 0;
		              while (index <BUFFERLEN){
		            	  byte = alt_up_sd_card_read(sd_fileh);
						  buff[index] = byte;
						  index++;
		              }
		              printf("Done!\n");

		              printf("Closing File...");
		              alt_up_sd_card_fclose(sd_fileh);
		              printf("Done!\n");
		          }
		      }
		//printf("%s", buff);
}
