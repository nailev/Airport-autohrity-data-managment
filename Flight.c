/*
 *
 *
 *  Created on: 27 April 2020
 *      Author: Nadav Shlomo Shaoulian
 *	
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Flight.h"
#include "Airport.h"
#include "Airline.h"


#define MAXSTR 300
#define IATA 3

double timeOfFlight(Flight * f)// duration of flight = distance\speed
{
	double dis, dur;
	dis = distance(f->from, f->dest);
	dur = dis / f->speed;
	return dur;
}

int isFlight(Flight * f, char * s, char * d)//checks if right flight
{
	Airport *sPtr, *dPtr;
	sPtr = f->from;
	dPtr = f->dest;
	if (compareByCode(sPtr, s) && compareByCode(dPtr, d))
		return 1;
	return 0;
}

int howManyFlights(Flight ** fs, int numOfFlights)//checks how many flights from A to B there is
{
	char s[IATA+1], d[IATA+1];
	int i = 0, sum=0;
	printf("please enter code of airport (source)");
	scanf("%s", &s);
	printf("please enter code of airport (destintion)");
	scanf("%s", &d);
	for ( i = 0; i < numOfFlights; i++)
	{
		sum += isFlight(fs[i], s, d);
	}
	printf("\nThere are %d flight between airport %s and airport %s \n", sum,s,d);
	return 0;
}
int newFlight(Flight * f, Manager* m)//allocating new flight
{
	int departure;
	fflush(stdin);
	printf("please enter time of flight (must be between 0-23)");
	scanf("%d", &departure);
	while (!(departure >= 0 && departure <= 23))
	{
		printf("please enter time of flight (must be between 0-23)");
		scanf("%d", &departure);
	} 
	f->depTime = departure;
	char str[MAXSTR];
	printf("please enter code of airport (source)");
	scanf("%s",str);
	f->from = findAirport(m, str);
	printf("please enter code of airport (destintion)");
	scanf("%s", str);
	f->dest = findAirport(m, str);
	printf("please enter date (dd/mm/yyyy) \n");
	scanf("%s", str);
	while (!checkDate(str))
	{
		printf("please enter date (dd/mm/yyyy) \n");
		scanf("%s", str);
	}
	sscanf(str, "%d/%d/%d", &f->d.day, &f->d.month, &f->d.year);
	printf("please enter speed (mph)");
	scanf("%d", &f->speed);
	return 0;
}
int newFlightFile(Flight * f, Manager* m,FILE* fp)//allocating new flight from file
{
	char str[IATA + 1];
	fread(str, sizeof(char), IATA+1, fp);
	f->from = findAirport(m, str);
	fread(str, sizeof(char), IATA + 1, fp);
	f->dest = findAirport(m, str);
	fread(&f->depTime, sizeof(int), 1, fp);
	fread(&f->d.day, sizeof(int), 1, fp);
	fread(&f->d.month, sizeof(int), 1, fp);
	fread(&f->d.year, sizeof(int), 1, fp);
	fread(&f->speed, sizeof(int), 1, fp);
	
	return 0;
}
void freeFlight(Flight * f)//free flight allocation
{
	free(f);
}
int checkDate(char* str)// method that checks if date is valid 
{
	int dd, mm, yy;
	sscanf(str,"%d/%d/%d", &dd, &mm, &yy);
	//check year
	if (yy >= 1900 && yy <= 9999)
	{//check month
		if (mm >= 1 && mm <= 12)
		{//check days
			if ((dd >= 1 && dd <= 31) && (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12))
				return 1;
			else if ((dd >= 1 && dd <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11))
				return 1;
			else if ((dd >= 1 && dd <= 28) && (mm == 2))
				return 1;
			else if (dd == 29 && mm == 2 && (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0)))
				return 1;
			else
				return 0;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	return 1;
}
void freeF(void* flight)
{
	Flight* f = *(Flight**)flight;
	freeFlight(f);
}
int sortBySpeed(const void* a, const void* b)
{
	Flight* f1 = *(Flight**)a;
	Flight* f2 = *(Flight**)b;
	return f1->speed - f2->speed;
}
int sortByTime(const void* a,const void* b)
{
	Flight* f1 = *(Flight**)a;
	Flight* f2 = *(Flight**)b;
	return f1->depTime - f2->depTime;
}

int sortByDate(const void* a, const void* b)
{
	Flight* f1 = *(Flight**)a;
	Flight* f2 = *(Flight**)b;
	if (f1->d.year == f2->d.year && f1->d.month == f2->d.month)
		return f1->d.day - f2->d.day;
	if (f1->d.year == f2->d.year)
		return f1->d.month - f2->d.month;
	return f1->d.year - f2->d.year;
}

void departureFlights(Flight* flight, ...)
{
	va_list allFlights;
	Flight* currentFlight;
	va_start(allFlights, flight);
	currentFlight = flight;
	printf("\nDeparture board Flights :\n");
	while (currentFlight != NULL)
	{
		printf("From %s To %s ,date: %d/%d/%d | departure time :%d | avarage speed : %d |\n"
			, currentFlight->from->iata, currentFlight->dest->iata, currentFlight->d.day,
			currentFlight->d.month, currentFlight->d.year
			, currentFlight->depTime, currentFlight->speed);
			currentFlight = va_arg(allFlights, Flight *);
	}
			printf("\n");
			va_end(allFlights);
	
}