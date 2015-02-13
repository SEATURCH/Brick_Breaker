/*
 * sound.h
 *
 *  Created on: 2015-02-10
 *      Author: Francis
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"

#define AV_CONFIG "/dev/audio_video_config"
#define AUDIO_NAME "/dev/audio"
#define MAX_BUFFER_SIZE 64
#define MAX_SAMPLES_SIZE 20000
#define WAV_FILE_DIRECTORY ""
#define FILE_READ_ERROR_1 -1
#define FILE_READ_ERROR_2 -2
#define FILE_HANDLER_ERROR_1 -1
#define FILE_HANDLER_ERROR_2 -2
#define READ_MULTIBYTE_ERROR -1
#define BYTES_TO_DATA_SECTION 44
#define BLOCK_HIT_SOUND_FILENAME "S14.WAV"
#define PREP_WAV_FILE_ERROR -1
#define READ_WAV_FILE_ERROR 0
#define READ_WAV_FILE_SUCCESS 1


typedef struct MusicData {
	unsigned int samples[MAX_SAMPLES_SIZE];
	int current_position;
	int final_position;
} MusicData;

typedef struct ISRparams {
	MusicData* music;
	unsigned int *sound_buf;
	alt_up_audio_dev *audio_handler;
} ISRparams;

void av_config_setup(void);
int read_wav_file(char* file_name, MusicData* music_data);
int skip_bytes_in_file(short int file_handler, int bytes_to_skip);
int read_multibyte_number_in_file(short int file_handler, int bytes);
void fill_sound_buffer(unsigned int buf[],MusicData* music_data);
void fill_audio_channels (unsigned int buf[], MusicData* music_data,alt_up_audio_dev* audio_handler);
void play_sound (MusicData* music_data);


#endif /* SOUND_H_ */
