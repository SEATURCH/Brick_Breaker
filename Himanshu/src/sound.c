/*
 * sound.c
 *
 *  Created on: February 10th, 2015
 *      Author: Francis
 */


#include "../include/sound.h"

/*
 * Setup for the audio video config core.
 */
void av_config_setup() {
	alt_up_av_config_dev * av_config = alt_up_av_config_open_dev(AV_CONFIG);
	while (!alt_up_av_config_read_ready(av_config)) {
	}
}


/*
 * Function to check that the given .wav file exists, and parses the header
 * to make sure that it is a .wav file, is 16-bit, and is at 32kHz. It also
 * checks to see how large the data section size is and stores this in
 * the MusicData struct passed to it.
 */
int read_wav_file(char* file_name, MusicData* music_data){
	char* wav_file;
	int i;
	int file_found, file_frequency, bits_per_sample, data_section_size, sample;
	short int file_handler;
	char string_validator[5];
	//The "RIFF" and "WAVE" strings in the file do not have a null character
	string_validator[4] = '\0';
	if( (alt_up_sd_card_is_Present()) ) {
		printf("Card connected.\n");

		if (alt_up_sd_card_is_FAT16()) {
			printf("FAT16 file system detected.\n");
		} else {
		  printf("Unknown file system.\n");
		  return 0;
		}

		//Check to see if there are any files, and then further check if there is a file with the proper name
		if (alt_up_sd_card_find_first(WAV_FILE_DIRECTORY, wav_file) == 0) {
			//Try to find a filename matching the given argument
			if (strcmp(wav_file,file_name)) {
				file_found = 0;
				while(alt_up_sd_card_find_next(wav_file) == 0) {
					if (!strcmp(wav_file,file_name)) {
						file_found = 1;
						break;
					}
				}
				if(file_found == 0) {
					return READ_WAV_FILE_ERROR; //failed to find file
				}
			}

			/*
			 * CODE TO PARSE WAV FILE
			*/

			//Open the file
			file_handler = alt_up_sd_card_fopen(wav_file,0); //0 makes it so we do not make the file if it does not exist
			if (file_handler == FILE_HANDLER_ERROR_1) {
				printf("File has been located, but could not be opened.\n");
				return READ_WAV_FILE_ERROR;
			} else if (file_handler == FILE_HANDLER_ERROR_2) {
				printf("File is already open.\n");
				return READ_WAV_FILE_ERROR;
			}
			/* Code to read the header of the .wav file */
			//Should read "RIFF"
			for(i=0;i<4;i++) {
				string_validator[i] = alt_up_sd_card_read(file_handler);
			}
			if(strcmp(string_validator,"RIFF")) {
				printf("File does not contain RIFF in header (Not a .wav file)\n");
				printf("%c%c%c%c\n",string_validator[0],string_validator[1],string_validator[2],string_validator[3]);
				if(!alt_up_sd_card_fclose(file_handler)) {
					printf("Failed to close the file\n");
				}
				return READ_WAV_FILE_ERROR;
			}

			//Skip next 4 bytes, that describe size of file (not needed for our purposes)
			if(!skip_bytes_in_file(file_handler,4)){
				return READ_WAV_FILE_ERROR;
			}

			//Should read "WAVE"
			for(i=0;i<4;i++) {
				string_validator[i] = alt_up_sd_card_read(file_handler);
			}
			if(strcmp(string_validator,"WAVE")) {
				printf("File does not contain WAVE in header (Not a .wav file)\n");
				if(!alt_up_sd_card_fclose(file_handler)) {
					printf("Failed to close the file\n");
				}
				return READ_WAV_FILE_ERROR;
			}

			//Skip next 12 bytes, we only play mono .wav files and its PCM
			if(!skip_bytes_in_file(file_handler,12)) {
				return READ_WAV_FILE_ERROR;
			}

			//Next 4 bytes determine the frequency of the music, it must match 32kHz or we reject it
			if((file_frequency = read_multibyte_number_in_file(file_handler,4)) == READ_MULTIBYTE_ERROR) {
				return READ_WAV_FILE_ERROR;
			}
			if(file_frequency != 32000) {
				printf("File frequency of .wav file is not 32000!\n");
				return READ_WAV_FILE_ERROR;
			}

			//Skip another 6 unneeded bytes in the header
			if(!skip_bytes_in_file(file_handler,6)) {
				return READ_WAV_FILE_ERROR;
			}

			//Next 2 bytes determine the bits per sample, which must be 16 as per our specifications.
			if((bits_per_sample = read_multibyte_number_in_file(file_handler,2)) == READ_MULTIBYTE_ERROR) {
				return READ_WAV_FILE_ERROR;
			}
			if(bits_per_sample != 16) {
				printf("Bits per sample is not 16! It is %d\n",bits_per_sample);
			}

			//Should read "data"
			for(i=0;i<4;i++) {
				string_validator[i] = alt_up_sd_card_read(file_handler);
			}
			if(strcmp(string_validator,"data")) {
				printf("File does not contain 'data' in header (Not a .wav file)\n");
				if(!alt_up_sd_card_fclose(file_handler)) {
					printf("Failed to close the file\n");
				}
				return READ_WAV_FILE_ERROR;
			}

			//Next 4 bytes are the file size, excluding the first 44 bytes(the header)
			if((data_section_size = read_multibyte_number_in_file(file_handler,4)) == READ_MULTIBYTE_ERROR) {
				return READ_WAV_FILE_ERROR;
			}

			//Parsing the data -- there are data_section_size number of bytes, and we read two bytes at once
			//to get an amplitude value for a sample. Note, the current code expects to read ALL the data
			//at once. As such, if the file is too big we do not read at all.

			if(data_section_size/2 <= MAX_SAMPLES_SIZE) {
				for(i=0; i<data_section_size/2; i++) {
					 if((sample = read_multibyte_number_in_file(file_handler,2)) == READ_MULTIBYTE_ERROR) {
						 return READ_WAV_FILE_ERROR;
					 }
					 music_data->samples[i] = (unsigned int) sample;
				}
			} else {
				printf("File is too large!\n");
				if(!alt_up_sd_card_fclose(file_handler)) {
					printf("Failed to close the file\n");
					return READ_WAV_FILE_ERROR;
				}
				return READ_WAV_FILE_ERROR;
			}
			music_data->final_position = data_section_size/2;
			music_data->current_position = data_section_size/2;
			//Close the file handler
			if(!alt_up_sd_card_fclose(file_handler)) {
				printf("Failed to close the file\n");
				return READ_WAV_FILE_ERROR;
			}
			/*
			 * END CODE TO PARSE WAV FILE
			 */

		} else {
			printf("No file found\n");
			return READ_WAV_FILE_ERROR;
		}

	  } else{
		  printf("SD CARD NOT CONNECTED\n");
		  return READ_WAV_FILE_ERROR;
	  }
	return READ_WAV_FILE_SUCCESS;
}

/*
 * Function to skip a given number of bytes to the given file.
 * The function returns 0 on failure and 1 on success, as well as printing out
 * the corresponding error messages. When there is an error, it also tries
 * to close the files.
 */
int skip_bytes_in_file(short int file_handler, int bytes_to_skip) {
	int i;
	short int byte_read;
	for(i=0;i<bytes_to_skip;i++) {
		byte_read = alt_up_sd_card_read(file_handler);
		if(byte_read == FILE_READ_ERROR_1) {
			printf("File handler invalid, while trying to skip bytes. \n");
			if(!alt_up_sd_card_fclose(file_handler)) {
				printf("Failed to close the file\n");
			}
			return 0;
		} else if (byte_read == FILE_READ_ERROR_2) {
			printf("Could not read from SD card while skipping unneeded bytes in file. \n");
			if(!alt_up_sd_card_fclose(file_handler)) {
				printf("Failed to close the file\n");
			}
			return 0;
		}
	}
	return 1;
}

/*
	Function to read a variable number of bytes in the given file.
	It returns these bytes as an integer given that the .wav file is in
	little endian. There are error checks, and if an error occurs it attempts
	to close the file. This should not return a negative value, so the
	error return value is -1.
*/
int read_multibyte_number_in_file(short int file_handler, int bytes) {
	short byte_read;
	int i;
	int multibyte_number = 0;
	if(bytes>4) {
		printf("Too many bytes being read at once, there is a maximum of 4\n");
		return READ_MULTIBYTE_ERROR;
	}
	for(i=0;i<bytes;i++) {
		byte_read = alt_up_sd_card_read(file_handler);
		if(byte_read == FILE_READ_ERROR_1) {
			printf("File Handler invalid, while trying to read the freq.\n");
			if(!alt_up_sd_card_fclose(file_handler)) {
				printf("Failed to close the file\n");
			}
			return READ_MULTIBYTE_ERROR;
		} else if (byte_read == FILE_READ_ERROR_2) {
			printf("Could not read from SD Card, while trying to read the freq.\n");
			if(!alt_up_sd_card_fclose(file_handler)) {
				printf("Failed to close the file\n");
			}
			return READ_MULTIBYTE_ERROR;
		}
		/*
		* Since .wav files are little endian, the LSB are stored first. Thus, we OR
		* our final integer with the bytes we read, bit shifted. Make sure to
		* cast the byte, so that integer promotion doesn't change the results
		*/
		multibyte_number = multibyte_number | ((unsigned short) byte_read << 8*i);
	}
	return multibyte_number;
}


/*
 * Fills the buffer that the sound channels copy from, with new data
 * from our MusicData struct.
 */
void fill_sound_buffer(unsigned int buf[], MusicData* music_data) {
	int i, maximum;
	int currentPosition = music_data->current_position;
	int finalPosition = music_data->final_position;
	if(currentPosition + MAX_BUFFER_SIZE < finalPosition) {
		maximum = MAX_BUFFER_SIZE;
	} else {
		maximum = finalPosition-currentPosition;
	}
	for(i=0;i<maximum;i++) {
		buf[i] = music_data->samples[i+currentPosition];
	}
	music_data->current_position = currentPosition+i;
	//If we have reached the end of the file, fill the rest of the buffer with 0's to stop sound.
	for(;i<MAX_BUFFER_SIZE;i++) {
		buf[i] = 0;
	}
}
/*
 * Checks to see if the current music piece is done,
 * this is done through a comparison between the current position
 * in the music and the final position of the music. If it is not
 * done, and if the audio fifo needs filling, write the next
 * samples of music into the buffer, and fill the audio fifos
 * with the buffer.
 */

void fill_audio_channels (unsigned int buf[], MusicData* music_data, alt_up_audio_dev* audio_handler){
	if (music_data->current_position < music_data->final_position) {
		if ((alt_up_audio_write_fifo_space(audio_handler,ALT_UP_AUDIO_LEFT) >= MAX_BUFFER_SIZE) && (alt_up_audio_write_fifo_space(audio_handler,ALT_UP_AUDIO_RIGHT)>= MAX_BUFFER_SIZE)) {
			fill_sound_buffer(buf,music_data);
			alt_up_audio_write_fifo(audio_handler, buf, MAX_BUFFER_SIZE, ALT_UP_AUDIO_LEFT);
			alt_up_audio_write_fifo(audio_handler, buf, MAX_BUFFER_SIZE, ALT_UP_AUDIO_RIGHT);
		}
	}
}

/*
 * Function to restart the current position of the given MusicData struct.
 * Call this function to play a given sound.
 */
void play_sound (MusicData* music_data) {
	music_data->current_position = 0;
}
