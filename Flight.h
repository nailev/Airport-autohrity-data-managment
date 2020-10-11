/*
 *
 *
 *  Created on: 27 April 2020
 *      Author: Nadav Shlomo Shaoulian 
 *	
 */

#define _CRT_SECURE_NO_WARNINGS
#ifndef __Flight__
#define __Flight__
#include "Airport.h"
#include "Manager.h"
typedef struct
{
	int day;
	int month; //mph
	int year;
} Date;

typedef struct
{

	Airport* from;
	Airport* dest;
	int depTime;//number between 0-23
	Date d;
	int speed; //mph

} Flight;

double timeOfFlight(Flight* f);
int isFlight(Flight* f, char* s, char* d);
int howManyFlights(Flight ** fs, int numOfFlights);
int newFlight(Flight * a,Manager* m);
int checkDate(char* str);
void freeFlight(Flight * f);
void freeF(void* flight);
int newFlightFile(Flight * f, Manager* m, FILE* fp);
int sortBySpeed(void* a, void* b);
int sortByTime(void* a, void* b);
int sortByDate(void* a, void* b);
void departureFlights(Flight* flight, ...);
#endif