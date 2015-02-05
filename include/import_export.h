/*
 * ImportExport.c
 *
 *  Created on: Feb 4th, 2015
 *      Author: Alan L.
 */

#include "../include/vga_graphics.h"

int ExportBlockDataStructure(RenderObjectStructure *renderObjectStructure,
		BlockObjectStructure *blockObjectStructure, unsigned char *outputBuffer,
		unsigned int outputBufferLen);

int ImportBlockDataStructure(RenderObjectStructure *renderObjectStructure,
		BlockObjectStructure *blockObjectStructure, unsigned char *inputBuffer,
		unsigned int inputBufferLen);
