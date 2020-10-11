/*
 *
 *
 *  Created on: 27 April 2020
 *      Author: Nadav Shlomo Shaoulian 
 *	
 */

#define _CRT_SECURE_NO_WARNINGS
#ifndef __Airline__
#define __Airline__
#include "Flight.h"
typedef enum { NO, SPEED, HOUR , DATE  } SORT;
typedef struct
{
	char* name;
	int numOfPlanes;
	int numOfFlights;
	Flight** flights;
	SORT sort;

}Airline;


Airline* initAirline(Manager *m);
int addFlightToAirline(Airline* a, Manager *m, FILE * fp);
void printAirlineStats(Airline *a);
void freeAirline(Airline * comp);
void freeAll(Manager* m, Airline* comp);
void genericArr(void* arr, int size, int typeSize, void(*fptr)(void*));
void sortTheFlights(Airline * comp);
Flight* bsearchFlight(Airline * comp);
void printToFile(Airline* a,Manager* m,char* nameOfFile,char* type);

#endif