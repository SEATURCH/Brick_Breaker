/*
 * ImportExport.c
 *
 *  Created on: Feb 4th, 2015
 *      Author: Alan L.
 */
#include "../include/import_export.h"

// **********Protocol Structure**********
// |N A B| |v0 w0 x0 y0 z0| |v1 w1 x1 y1 z1| ... |vn wn xn yn zn|

// Each letter {A, B, C, v, w, x, y, z} represents 1 byte
// First digit represents block number
// Second digit represents byte number
// N: Number of blocks
// n: N - 1
// W: Render Object Width [pixels]
// H: Render Object Height [pixels]

// v_: X Position [render objects]
// w_: Y Position [render objects]
// x_: Bits (7..4): Block Width [render objects]
//	   Bits (3..0): Block Height [render objects]
// y_: Color(15..8)
// z_: Color(7..0)

// In Bytes
#define EXPORT_HEADER_LEN 		(3)
#define EXPORT_BLOCKINFO_LEN	(5)
#define EXPORT_MINIMUM_LEN		(EXPORT_HEADER_LEN + EXPORT_BLOCKINFO_LEN) // 1 block
// **Color**
// *EXPORT*
// input: word
// output: byte[0]
#define GET8BIT_LOWER(input) ((input&0x00FF))
// input: word
// output: byte[1]
#define GET8BIT_UPPER(input) (GET8BIT_LOWER((input&0xFF00)>>8))

// *IMPORT*
// input: byte[1], byte[0]
// output: word
#define COMBINE_BYTES_INTO_WORD(upper, lower) (((upper<<0x8)&0xFF00)|(lower&0x00FF))

// **Width/Height**
// *EXPORT*
// input: upper 4 bits, lower 4 bits
// output: byte
#define COMBINE_2BYTES_INTO_BYTE(upper, lower) (((upper<<4)&0xF0)|(lower&0x0F))

// *IMPORT*
// input: byte
// output: lower 4 bits
#define GET4BIT_LOWER(input) (input&0x0F)
// input: byte
// output: upper 4 bits
#define GET4BIT_UPPER(input) (GET4BIT_LOWER((input&0xF0)>>0x4))

// Serializes current block object structure, based off numBlocksSet
// Returns 0 on failure
// Input: blockObjectStructure
// Output: serialized blockObjectStructure, contained in outputBuffer
int ExportBlockDataStructure(RenderObjectStructure *renderObjectStructure,
		BlockObjectStructure *blockObjectStructure, unsigned char *outputBuffer,
		unsigned int outputBufferLen) {
	unsigned char blockCursor = 0;
	unsigned char bufferCursor = 0;
	// Check for invalid input
	if (outputBufferLen < EXPORT_MINIMUM_LEN) {
		return 0;
	}
	if (!blockObjectStructure->numBlocksSet) {
		return 0;
	}
	if (outputBufferLen
			< ((blockObjectStructure->numBlocksSet * EXPORT_BLOCKINFO_LEN)
					+ EXPORT_HEADER_LEN)) {
		return 0;
	}

	// Set Header Bytes
	// [0] : Number of Blocks
	// [1] : Render Object Width (not used)
	// [2] : Render Object Height (not used)
	outputBuffer[bufferCursor++] = blockObjectStructure->numBlocksSet;
	outputBuffer[bufferCursor++] = RENDER_OBJECT_WIDTH;
	outputBuffer[bufferCursor++] = RENDER_OBJECT_HEIGHT;

	// Loop through every input block
	for (blockCursor = 0; blockCursor < blockObjectStructure->numBlocksSet;
			blockCursor++) {
		// X
		outputBuffer[bufferCursor++] =
				blockObjectStructure->blockObjects[blockCursor].blockXStart;
		// Y
		outputBuffer[bufferCursor++] =
				blockObjectStructure->blockObjects[blockCursor].blockYStart;
		// (height, width)
		outputBuffer[bufferCursor++] =
				COMBINE_2BYTES_INTO_BYTE(blockObjectStructure->blockObjects[blockCursor].blockHeightRenderObjects,
						blockObjectStructure->blockObjects[blockCursor].blockWidthRenderObjects);
		outputBuffer[bufferCursor++] =
				GET8BIT_UPPER(blockObjectStructure->blockObjects[blockCursor].blockType);
		outputBuffer[bufferCursor++] =
				GET8BIT_LOWER(blockObjectStructure->blockObjects[blockCursor].blockType);
	}

	return 1;
}

BlockType determineBlockType( unsigned short inputColor){
	switch (inputColor){
	case (int)SingleHealth:
		return SingleHealth;
	case (int)DoubleHealth:
		return DoubleHealth;
	case (int)TripleHealth:
			return TripleHealth;
	case (int)Unbreakable:
				return Unbreakable;
	}
	return Invisible;
}

int ImportBlockDataStructure(RenderObjectStructure *renderObjectStructure,
		BlockObjectStructure *blockObjectStructure, unsigned char *inputBuffer,
		unsigned int inputBufferLen) {
	unsigned char numBlocks = 0;
	unsigned char blockCursor = 0;
	unsigned int bufferCursor = 0;
	unsigned char renderObjectWidth; // Not used
	unsigned char renderObjectHeight; // Not used

	if (inputBufferLen < EXPORT_MINIMUM_LEN) {
		return 0;
	}

	// Reset Render Object Structure
	InitializeRenderObjectStructure(renderObjectStructure);
	// Reset Block Object Structure
	InitializeBlockObjectStructure(blockObjectStructure, renderObjectStructure);

	// Retrieve header info;
	numBlocks = inputBuffer[bufferCursor++];
	renderObjectWidth = inputBuffer[bufferCursor++];
	renderObjectHeight = inputBuffer[bufferCursor++];

	// Loop through input buffer
	for (blockCursor = 0; blockCursor < numBlocks; blockCursor++) {
		unsigned char posX, posY, width, height;
		short color;
		BlockType color2;
		posX = inputBuffer[bufferCursor++];
		posY = inputBuffer[bufferCursor++];
		width = GET4BIT_LOWER(inputBuffer[bufferCursor]); // Don't increment, height is in the same index
		height = GET4BIT_UPPER(inputBuffer[bufferCursor++]);
		color =
				COMBINE_BYTES_INTO_WORD(inputBuffer[bufferCursor], inputBuffer[bufferCursor + 1]);
		color2 = determineBlockType(color);
		bufferCursor++;
		bufferCursor++;

		AddBlock(blockObjectStructure, posX, posY, width, height, color2);

	}

	// Map the blocks that were just added
	MapBlockObjectStructureToRender(blockObjectStructure,
			renderObjectStructure);

	// Draw the set of blocks to screen
	DrawRenderObjectStructure(renderObjectStructure);

	return 1;

}

