/*
 * Set.h
 *
 *  Created on: 2015-02-05
 *      Author: Alan
 */

#ifndef SET_H_
#define SET_H_

#define SET_MAX_SIZE 	10

typedef struct Set
{
	unsigned int values[SET_MAX_SIZE];
	int numValues;
} Set;


int AddToSet(Set *set, int value);
int ClearSet(Set *set);



#endif /* SET_H_ */
