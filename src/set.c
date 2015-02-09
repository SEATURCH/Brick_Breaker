/*
 * Set.c
 *
 *  Created on: 2015-02-05
 *      Author: Alan
 */


#include "../include/set.h"


int ClearSet(Set *set)
{
	set->numValues = 0;
}


// Returns 0 if new value exists in set or set is full
// Returns 1 if successfully added to set
int AddToSet(Set *set, int value)
{
	int itemFound = 0;
	int setCursor = 0;

	for(setCursor = 0; setCursor < set->numValues; setCursor++)
	{
		if(set->values[setCursor] == value)
		{
			itemFound = 1;
			break;
		}
	}

	if(!itemFound)
	{
		if(set->numValues < SET_MAX_SIZE)
		{
			set->values[set->numValues++] = value;
			return 1; // Add success
		}
	}
	return 0; // Add failed
}
