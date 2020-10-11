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

#include "Airline.h"
#include "Flight.h"
#include "Manager.h"
#include "Airport.h"
#define MAXSTR 300
#define HOUR 60
#define IATA 3


Airline * initAirline(Manager *m)//initiate airline company
{
	Airline * a;
	FILE* fp;
	int sizeOfName,i,numOfFl;
	fp = fopen("company.bin", "rb");
	if (fp == NULL)
		return NULL;
	char str[MAXSTR];
	a = (Airline*)malloc(sizeof(Airline));
	if (!a)
		return NULL;
	a->numOfFlights = 0;
	fread(&a->numOfPlanes, sizeof(int), 1, fp);
	fread(&sizeOfName, sizeof(int), 1, fp);
	a->name = (char*)malloc(sizeof(char)*(sizeOfName+1 ));
	fread(str, sizeof(char), sizeOfName+1, fp);
	strcpy(a->name, str);
	fread(&numOfFl, sizeof(int), 1, fp);
	fread(&a->sort, sizeof(int), 1, fp);
	a->flights = NULL;
	for ( i = 0; i < numOfFl; i++)
	{
		addFlightToAirline(a, m, fp);
	}
	fclose(fp);
	return a;
}
int addFlightToAirline(Airline* a,Manager *m,FILE * fp)//adding flight to airline
{
	a->flights = (Flight**)realloc(a->flights, sizeof(Flight*)*(a->numOfFlights + 1));
	if (!a->flights)
		return 0;
	a->flights[a->numOfFlights] = (Flight*)malloc(sizeof(Flight));
	if (!a->flights[a->numOfFlights])
		return 0;
	if (fp == NULL)
		newFlight(a->flights[a->numOfFlights], m);
	else
		newFlightFile(a->flights[a->numOfFlights], m, fp);
	a->numOfFlights++;
	a->sort = 0;
	return 1;
}
void printToFile(Airline * a, Manager * m, char * nameOfFile, char * type)//writing to file by type text/binary
{
	FILE * fp;
	int i, size;
	if (type == "text")
	{
		fp = fopen(nameOfFile, "w");
		fprintf(fp, "%d\n", m->numOfAps);
		for (i = 0; i < m->numOfAps; i++)
		{
			fprintf(fp, "%s%s%s\n%lf %lf\n", m->aps[i]->ap, m->aps[i]->country, m->aps[i]->iata,
				m->aps[i]->lat, m->aps[i]->lon);
		}
	}
	else
	{
		char str[MAXSTR];
		strcpy(str, a->name);
		size = strlen(a->name);
		fp = fopen(nameOfFile, "wb");
		fwrite(&a->numOfPlanes, sizeof(int), 1, fp);
		fwrite(&size, sizeof(int), 1, fp);
		fwrite(str, sizeof(char), size+1, fp);
		fwrite(&a->numOfFlights, sizeof(int), 1, fp);
		fwrite(&a->sort, sizeof(int), 1, fp);
		for (i = 0; i < a->numOfFlights; i++)
		{
			strcpy(str, a->flights[i]->from->iata);
			fwrite(str, sizeof(char), IATA+1, fp);
			strcpy(str, a->flights[i]->dest->iata);
			fwrite(str, sizeof(char), IATA+1, fp);
			fwrite(&a->flights[i]->depTime, sizeof(int), 1, fp);
			fwrite(&a->flights[i]->d.day, sizeof(int), 1, fp);
			fwrite(&a->flights[i]->d.month, sizeof(int), 1, fp);
			fwrite(&a->flights[i]->d.year, sizeof(int), 1, fp);
			fwrite(&a->flights[i]->speed, sizeof(int), 1, fp);
		}
	}
	fclose(fp);
}
void printAirlineStats(Airline * a)// print airline stats
{
	int i, minutes, hours;
	double dist, duration;

	for (i = 0; i < a->numOfFlights; i++)
	{
		Flight* fPtr = a->flights[i];
		printf("\nFlight number %d\n", i + 1);
		printf("From %s To %s ,date: %d/%d/%d | departure time :%d | avarage speed : %d |\n"
			, fPtr->from->iata, fPtr->dest->iata, fPtr->d.day, fPtr->d.month, fPtr->d.year
			, fPtr->depTime, fPtr->speed);
		dist = distance(fPtr->from, fPtr->dest);
		duration = timeOfFlight(fPtr);
		hours = (int)duration;
		minutes = (int)(duration*HOUR)%HOUR;
		printf("Duration : %lf | Distance : %lf\n", duration, dist);
		printf("Duration of flight hours : %d and minutes : %d\n\n", hours, minutes);
	}
}

void freeAirline(Airline * comp)//free airline company
{
	int i;
	for ( i = 0; i < comp->numOfFlights; i++)
	{
		freeFlight(comp->flights[i]);
	}
	free(comp->name);
	free(comp);
}

void freeAll(Manager* m, Airline* comp)//free all 
{
	freeManager(m);
	freeAirline(comp);
}

void genericArr(void* arr, int size, int typeSize, void(*fptr)(void*))//genric function for array
{
	int i;
	for (i = 0; i < size; i++)
	{
		fptr((char*)arr + (i * typeSize));
	}
}

void sortTheFlights(Airline *comp)//sorting flight 
{
	int sortOp;
	printf("Please enter choice sort : (1 by speed , 2 by time , else by date");
	scanf("%d", &sortOp);
	if (sortOp == 1)//by speed
	{
		qsort(comp->flights, comp->numOfFlights, sizeof(Flight*), sortBySpeed);
		comp->sort = 1;
	}
	else if (sortOp == 2)//by time of departure
	{
		qsort(comp->flights, comp->numOfFlights, sizeof(Flight*), sortByTime);
		comp->sort = 2;
	}
	else//by date
	{
		qsort(comp->flights, comp->numOfFlights, sizeof(Flight*), sortByDate);
		comp->sort = 3;
	}
}

Flight* bsearchFlight(Airline * comp)//sending pointer to currect flight
{
	int numComp;
	if (comp->sort == 0)
	{
		printf("The Flight array is not sorted , please sort it (number 7 choice)\n");
		return NULL;
	}
	Flight * f = malloc(sizeof(Flight));
	Flight ** fPtr = NULL;
	if (comp->sort == 1)
	{
		printf("Please enter speed of flight\n");
		scanf("%d", &f->speed);
		fPtr = (Flight**)bsearch(&f, comp->flights, comp->numOfFlights, sizeof(Flight*), sortBySpeed);
		free(f);
		return *fPtr;
	}else if (comp->sort == 2)
	{
		printf("Please enter time of flight\n");
		scanf("%d", &f->depTime);
		fPtr = (Flight**)bsearch(&f, comp->flights, comp->numOfFlights, sizeof(Flight*), sortByTime);
		free(f);
		return *fPtr;
	}
	else
	{
		printf("\nPlease enter date dd/mm/yyyy\n");
		scanf("%d/%d/%d", &f->d.day, &f->d.month, &f->d.year);
		fPtr = *(Flight**)bsearch(&f, comp->flights, comp->numOfFlights, sizeof(Flight*), sortByDate);
		free(f);
		return fPtr;
	}
}





