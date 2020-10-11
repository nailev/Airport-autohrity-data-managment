/*
 *
 *
 *  Created on: 27 April 2020
 *      Author: Nadav Shlomo Shaoulian 
 *	
 */

#define _CRT_SECURE_NO_WARNINGS
#ifndef __Airport__
#define __Airport__
typedef struct 
{
	char* ap;
	char* country;
	char* iata;
	double lat;
	double lon;

} Airport;

int compareByName( Airport* a1,  Airport* a2);
int compareByCode(const Airport* a1, char* code);
double distance(const Airport* a1, const Airport* a2);
int newAp(Airport * a);
int apFromFile(Airport * a,FILE * f);
int checkIATA(char* code);
int makeUpper(char* str);
void getCoordinates(Airport* a);
void deleteSpaces(char * str);
void freeAp(Airport * a);
void printAirport(void* airPort);
#endif