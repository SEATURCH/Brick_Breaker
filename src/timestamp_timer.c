/*
 * timestamp_timer.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 */

#include "../include/timestamp_timer.h"

//Starts timer and tracks how long it takes to go through 3 loops of hundreds
void exer2(){
	 int sum=0;
	 int i, j, k;
	 alt_u32 start_time;
	 alt_u32 end_time;

	 start_time = alt_timestamp();

	 if (alt_timestamp_start() < 0){
		 printf ("No timestamp device is available.\n");
	 }else{
		 //Keeping speed
		for (i=0; i<100; i++) {
		  for(j=0; j<100;j++){
			  for (k=0; k<100; k++) {
				  sum ++;
			  }
		  }
		}
		end_time = alt_timestamp();
		printf("time taken: %d clock ticks\n", (int)(end_time-start_time));
		printf("            %f seconds\n", (float)(end_time-start_time)/(float)alt_timestamp_freq());
		printf("Frequency: %f\n", (float)alt_timestamp_freq());
	}

}

void sleep(unsigned int seconds){
	alt_u32 start_time;
	alt_u32 end_time;
	float sec;
		if (alt_timestamp_start() < 0){
			printf("No timestamp device is available.\n");
		}else{
			start_time = alt_timestamp();
			end_time = alt_timestamp();
			sec = ((float)(end_time-start_time)/(float)alt_timestamp_freq()) * 200;
			while (sec < (float)(seconds)){
				end_time = alt_timestamp();
				sec = ((float)(end_time-start_time)/(float)alt_timestamp_freq()) * 200;
			}
		}
}

void wait_16ms(){
	alt_u32 start_time;
	alt_u32 end_time;
			if (alt_timestamp_start() < 0){
				printf("No timestamp device is available.\n");
			}else{
				start_time = alt_timestamp();
				while(alt_timestamp() < 800000){} // waste cpu cycle
			}
			end_time = alt_timestamp();
}