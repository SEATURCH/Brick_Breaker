/*
 * ball.h
 *
 *  Created on: February 8th, 2015
 *      Author: Alan Larson
 */

#include "../include/set.h"
#include "../include/vga_graphics.h"
#include "../include/sound.h"
#include <math.h>
#define DEFAULT_BALL_POS_X				(SCREEN_WIDTH >> 1)
#define DEFAULT_BALL_POS_Y				(SCREEN_HEIGHT >> 1)
#define DEFAULT_BALL_FREQ_X				(5)
#define DEFAULT_BALL_FREQ_Y				(5)
#define DEFAULT_BALL_DIR_X				(1) // Start direction off going to up right
#define DEFUALT_BALL_DIR_Y				(-1)
#define DEFAULT_BALL_COLOR				0xFFFF
#define DEFAULT_PADDLE_ZERO_VEC			DEFAULT_PADDLE_WIDTH/4
#define BALL_SPEED_1					18
#define BALL_SPEED_2					15
#define BALL_SPEED_3					12
#define BALL_SPEED_4					9
#define BALL_SPEED_5					7
#define BALL_SPEED_6					5
#define BALL_SPEED_7					2
/*** MACROS ***/

// Returns 1 if ball is supposed to be moving in currentFrame
// Returns 0 if ball is not supposed to be moving in currentFrame
#define IS_MOVING(frequency, currentFrame) (((currentFrame % frequency)==0) ? (1) : (0));

#define CHECK_RIGHT_WALL(pos) 		((pos + (BALL_OBJECT_WIDTH)) >= (SCREEN_WIDTH))
#define CHECK_LEFT_WALL(pos)		(pos < 0)
#define CHECK_WALL_COLLISION_X(pos)	((CHECK_RIGHT_WALL(pos) || CHECK_LEFT_WALL(pos)) ? 1 : 0)

#define CHECK_TOP_WALL(pos)			(pos < 0)
#define CHECK_BOTTOM_WALL(pos)		((pos + BALL_OBJECT_HEIGHT) >= (SCREEN_HEIGHT ))
#define CHECK_WALL_COLLISION_Y(pos) ((CHECK_TOP_WALL(pos) || CHECK_BOTTOM_WALL(pos)) ? 1 : 0)


/*** FUNCTION DECLARATIONS ***/

void InitializeBall(Ball* ball);

// numPoints = 4
// x x x x
// Returns 1 if any pixels provided do not have the color of SCREEN_BACKGROUND_COLOR
// Returns 0 if all pixels checked have the color of SCREEN_BACKGROUND_COLOR
int CheckTouchedRenderObjectsY(Set *touchedRenderObjects,
		RenderObjectStructure *renderObjectStructure, int xStart, int yStart,
		int numPoints);


// numPoints = 4
// x
// x
// x
// x
// Returns 1 if any pixels provided do not have the color of SCREEN_BACKGROUND_COLOR
// Returns 0 if all pixels checked have the color of SCREEN_BACKGROUND_COLOR
int CheckTouchedRenderObjectsX(Set *touchedRenderObjects,
		RenderObjectStructure *renderObjectStructure, int xStart, int yStart,
		int numPoints);

// Moves ball and triggers collisions with blocks
int MoveBall(RenderObjectStructure *renderObjectStructure,
		BlockObjectStructure *blockObjectStructure, Paddle* paddle, Ball* ball,
		unsigned int currentFrame, MusicData* music_data);
