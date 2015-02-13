/*
 * brick_breaker_main.c
 *
 *  Created on: Jan 24, 2015
 *      Author: Himanshu
 *  Modified on: Jan 25, 2015
 *  	Author: Alan
 *  Modified on: Jan 28, 2015
 *	Author: Francis
 */

#include "../include/common.h"
#include "../include/memory_map.h"
#include "../include/sd_card.h"
#include "../include/sound.h"
#include "../include/vga_graphics.h"
#include "../include/character_lcd.h"
#include "../include/timestamp_timer.h"
#include "../include/import_export.h"
#include "../include/ball.h"

#define PERFORM_EXERCISES 0
#define PERFORM_EXPORTIMPORT 1

// Returns:
// 0 if not successful
// non-zero if successful
int InitializeComponents(alt_up_sd_card_dev* sd_dev,
		alt_up_character_lcd_dev * char_lcd_dev,
		alt_up_pixel_buffer_dma_dev* pixel_buffer,
		RenderObjectStructure *renderObjectStructure,
		BlockObjectStructure *blockObjectStructure, Paddle* paddle, Ball *ball,
		MusicData* blockHitSound) {
	int ret = 1;

	// Open SDCard port
	sd_dev = initialize_sd_card();
	if (sd_dev == NULL) {
		printf("Error: could not open SDCard device\n");
		ret = 0;
	} else
		printf("Opened character SDCard Device\n");

	// open the Character LCD port
	char_lcd_dev = open_and_init_character_lcd("/dev/character_lcd_0");

	if (char_lcd_dev == NULL) {
		printf("Error: could not open character LCD device\n");
		ret = 0;
	} else {
		printf("Opened character LCD device\n");
	}

	// Open VGA
	if (!InitializeVGA(pixel_buffer)) {
		ret = 0;
	}

	// Open render structure
	InitializeRenderObjectStructure(renderObjectStructure);
	InitializeBlockObjectStructure(blockObjectStructure, renderObjectStructure);
	InitializePaddle(paddle);
	InitializeBall(ball);

	if (read_wav_file(BLOCK_HIT_SOUND_FILENAME,
			blockHitSound) == READ_WAV_FILE_ERROR) {
		printf("Could not read the wav file for block hit sound properly\n");
		ret = 0;
	}

	return ret;
}

/*
 * ISR Routine for reading the sound, given a context of ISRparams.
 */
static void sound_buffer_isr(void* context, alt_u32 id) {
	MusicData* blockHitSound;
	unsigned int *sound_buf;
	alt_up_audio_dev* audio_handler;

	//Clear the TO bit, aka reset the counter
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

	blockHitSound = ((ISRparams*) context)->music;
	sound_buf = ((ISRparams*) context)->sound_buf;
	audio_handler = ((ISRparams*) context)->audio_handler;
	fill_audio_channels(sound_buf, blockHitSound, audio_handler);

}

/*
 * ISR initialization, which sets up the timer to be continuous at a specific rate.
 * ISRparams is a parameter, because we initialize it in the stack of the main() function. Another method would be to malloc it
 * inside this function.
 */
void InitializeISR(MusicData* music_data, unsigned int sound_buf[],
		alt_up_audio_dev* audio_handler, ISRparams* isr_params) {
	(isr_params)->music = music_data;
	(isr_params)->sound_buf = sound_buf;
	(isr_params)->audio_handler = audio_handler;

	//Make the timer trip an interrupt when it counts to 0, set it to continuous, clear the TO bit
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

	//0x000101D0 = 66000, which is ~ 500 interrupts per second w/ the 3.3Mhz processor, plenty for the audio
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x01D0);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0001);
	alt_irq_register(TIMER_IRQ, isr_params, sound_buffer_isr);

	//Start the ISR
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);
}

#define CLOCKSPEED 						(50000000)
#define MODIFIER						(1450)
#define MIN_TOTAL_CYCLES_PER_FRAME 		((CLOCKSPEED) / (MODIFIER))

int main() {
	alt_up_sd_card_dev* sd_dev;
	alt_up_character_lcd_dev * char_lcd_dev;
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_audio_dev* audio_handler;
	RenderObjectStructure renderObjectStructure;
	BlockObjectStructure blockObjectStructure;
	alt_up_char_buffer_dev *char_buffer;
	MusicData blockHitSound;
	ISRparams isr_params;
	Paddle paddle;

	Ball ball;
	// Holds all the balls
	BallObjectStructure ballObjectStructure;

 	char* game[] = { "G1.bin", "G2.bin", "G3.bin", "G4.bin", "G5.bin",
					"G6.bin", "G7.bin", "G8.bin", "G9.bin", "G10.bin" };


	char spi_read;
	//spi_read = (int*) malloc(sizeof(int));

	//Main Loop Variables
	unsigned char switchnow = 0;
	unsigned char switchlast = 0;
	int ballAlive = MAX_BALLS;
	int numBallAlive = 1;
	int ballCount = 0;
	unsigned int currentFrame = 0;


	unsigned int sound_buf[MAX_BUFFER_SIZE];
	 alt_u32 start_time;
	 alt_u32 end_time;
	 const alt_u32 total_wait_time = MIN_TOTAL_CYCLES_PER_FRAME;
	// Surpress the 'unused' warnings
	(void) sd_dev;
	(void) char_lcd_dev;
	(void) audio_handler;
	(void) pixel_buffer;
	(void) renderObjectStructure;
	(void) blockObjectStructure;

	printf("Game Start!\n");

	int restart = 0, num, level = 0, loadNewGame = -1, loadSavedGame = -1,
			savedgame = -1, backtomain = -1, game_index = -1;
	while (1) {
		InitializeBallObjectStructure(&ballObjectStructure);
		ballAlive = MAX_BALLS;
		ballCount = 0;
		restart = 0, num, level = 0, loadNewGame = -1, loadSavedGame = -1, savedgame =
				-1, backtomain = -1;
		if (!InitializeComponents(sd_dev, char_lcd_dev, pixel_buffer,
				&renderObjectStructure, &blockObjectStructure, &paddle, &ball,
				&blockHitSound)) {
			printf("Error with initialization\n");
			return 0;
		}

		DrawRenderObjectStructure(&renderObjectStructure);
		
		 // Open video audio config
		 av_config_setup();
		 // Open Audio port
		 audio_handler = alt_up_audio_open_dev(AUDIO_NAME);
		 if (audio_handler == NULL) {
		 printf("Audio core could not be opened. \n");
		 }


		 InitializeISR(&blockHitSound,sound_buf,audio_handler,&isr_params);
		while (loadSavedGame == -1 && loadNewGame == -1 && backtomain == -1) {
			num = DrawStartMenu(restart);
			switch (num) {
			case 0:
				level = DrawLevelMenu();
				if (level == (TOTAL_MENU_OPTIONS - 1))
					loadNewGame = -1;
				else {
					LoadNewGame(&renderObjectStructure, &blockObjectStructure,
							level);
					loadNewGame = 1;
				}
				break;
			case 1:
				loadSavedGame = DrawSavedGameMenu();
				if (loadSavedGame == (TOTAL_MENU_OPTIONS - 1))
					loadSavedGame = -1;
				break;
			default:
				backtomain = 1;
				break;
			}
		}
		//DrawRenderObjectStructure(&renderObjectStructure);
		if (loadSavedGame != -1) {
			unsigned char buffin[BUFFERLEN] = { 0 };
			readfile(loadSavedGame, buffin);
			ImportBlockDataStructure(&renderObjectStructure,
					&blockObjectStructure, buffin, BUFFERLEN);
		} else if (loadNewGame == 1) {
			// Map the blocks that were just added
			MapBlockObjectStructureToRender(&blockObjectStructure,
					&renderObjectStructure);

			// Draw the set of blocks to screen
			// This only has to be done once
			//DrawRenderObjectStructure(&renderObjectStructure);
		} else if (backtomain == 1) {
			continue;
		} else if (loadNewGame == -1)
			return 0;
#if 0
		// DisabledPERFORM_EXPORTIMPORT
		// ** PURELY TESTING OF IMPORT/EXPORT ** //
		// This function exports the current block structure, and then reads it back in
		{
#define BUFFERLEN 500
			unsigned char buffer[BUFFERLEN] = {0};
			unsigned char buffin[BUFFERLEN] = {0};
			// Export the block data structure
			ExportBlockDataStructure(&renderObjectStructure, &blockObjectStructure,
					buffer, BUFFERLEN);
			//sdwr("LVL1.bin", buffer);

			lvlselect(char_lcd_dev, buffin);
			// Import the just-exported block data structure
			// This resets the render and block object structures and
			// sets them to the previous set-up
			ImportBlockDataStructure(&renderObjectStructure, &blockObjectStructure,
					buffin, BUFFERLEN);

		}
#endif
		savedgame = -1;
		// MAIN LOOP GOES HERE

		char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer");
		alt_up_char_buffer_init(char_buffer);
		alt_up_char_buffer_clear(char_buffer);
		alt_up_char_buffer_string(char_buffer, "KEY3: PAUSE", 65, 59);
		while ((numBallAlive != 0) && (ballAlive > 0)) {
				alt_u32 time_used;
		 		start_time = alt_timestamp();		// Frame rate cap		
				int ballCursor, numBalls;
			if (IORD_32DIRECT(PUSH_BASE,0) == 0x06) { //paused
				while (IORD_32DIRECT(PUSH_BASE,0) != 0xE)
					;
				unsigned char buffer[BUFFERLEN] = { 0 };
				ExportBlockDataStructure(&renderObjectStructure,
						&blockObjectStructure, buffer, BUFFERLEN);
				//sdwr2("LVL1.bin", buffer);
				num = -1;
				num = DrawPauseMenu();
				switch (num) {
				case 0:
					ImportBlockDataStructure(&renderObjectStructure,
							&blockObjectStructure, buffer, BUFFERLEN);
					break;
				case 1:
					game_index = SaveGame();
					sdwr2(game[game_index], buffer);
					savedgame = 1;
					//backtomain = 1;
					//ballAlive = 0;
					break;
				default:

					backtomain = 1;
					ballAlive = 0;
					break;
//							return 0;
				}
			}
			if(backtomain == 1) break;
			//int position = paddleposition(&spi_read);
			// printf("%d\n", position);
			MovePaddle(&paddle, paddleposition(&spi_read));

		switchnow = IORD_32DIRECT(SWITCHES_BASE, 0);
		if ((switchnow != switchlast) && (ballCount < MAX_BALLS)) {
			switchlast = switchnow;
			ballCount++;
			AddBall(&ballObjectStructure);
		}

		numBalls = ballObjectStructure.numBallsSet;
		for (ballCursor = 0; ballCursor < numBalls; ballCursor++) {
			if (ballObjectStructure.balls[ballCursor].isActive == TRUE) {
				int dead = MoveBall(&renderObjectStructure, &blockObjectStructure, &paddle,
						&ballObjectStructure.balls[ballCursor], currentFrame,
						&blockHitSound);
				if(dead == 0){
					ballAlive--;
					ballObjectStructure.balls[ballCursor].isActive = FALSE;
					EraseBall( &ballObjectStructure.balls[ballCursor]);
				}
			}
		}

		end_time = alt_timestamp();

		if(start_time >= end_time)
		{
			time_used = (0xFFFF - start_time) + end_time;
		}
		else
		{
			time_used = end_time - start_time ;
		}

		if(time_used < total_wait_time) {
			// Wait remaining cycles
			for(; time_used<total_wait_time; ++time_used);
		}



			currentFrame++;

		}
		if (savedgame != 1 || backtomain != 1)
			restart = 1;
		if (backtomain == 1)
			continue;
	}

	//Game Over print available options

#if PERFORM_EXERCISES == 1
	int press;
	/* Write "Welcome to" in the first row */
	alt_up_character_lcd_string(char_lcd_dev, "Exercise Number");
	/* Move Cursor */
	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);

	if(*switches == 0x01) {
		press = 0
		while (1) {
			*leds = *switches;;
			printf("Starting Exercise 5\n");
			alt_up_character_lcd_string(char_lcd_dev, "  5");
			// draw boxes forever until reset
			draw_random_boxes_forever();
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
			printf("Done!\n");
		} else {

			switch(*push) {
				case 0xC:
				press = 1;
				break;
				case 0xA:
				press = 2;
				break;
				case 0x6:
				press = 3;
				break;
				default:
				break;
			}

			if(*push == 0xE) {
				switch(press) {
					case 1 :
					press = 0;
					printf("Starting Exercise 2\n");
					alt_up_character_lcd_string(char_lcd_dev, "  2");
					exer2();
					alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
					printf("Done!\n");
					break;
					case 2 :
					press = 0;
					printf("Starting Exercise 3\n");
					alt_up_character_lcd_string(char_lcd_dev, "  3");
					sdrd();
					alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
					printf("Done!\n");
					break;
					case 3:
					press = 0;
					printf("Starting Exercise 4\n");
					alt_up_character_lcd_string(char_lcd_dev, "  4");
					draw_diagonal_line_with_character();
					alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
					printf("Done!\n");
					break;
					default:
					break;
				}
			}
		}
	}

#endif

	return 0;
}

