/*
 *
 *
 *  Created on: 27 April 2020
 *      Author: Nadav Shlomo Shaoulian
 *	I
 */

#define _CRT_SECURE_NO_WARNINGS
#ifndef __Manager__
#define __Manager__
#include "Airport.h"
typedef struct
{

	Airport** aps; 
	int numOfAps; 

} Manager;

int addAirport(Manager* m);
Airport* findAirport(Manager* m ,char* code);
Manager* initMan(int num, FILE* f);
int printManagaerStats(Manager* m);
void freeManager(Manager* m);
#endif
