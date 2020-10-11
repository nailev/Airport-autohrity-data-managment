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
#include <ctype.h>
#include <math.h>
#include "Airport.h"
#define MAXSTR 300
#define R 3950
#define IATA 3
#define DEVIDE 2

int compareByName(const Airport * a1, const  Airport * a2)//compae by the name of the airport
{
	int i,size = strlen(a1->iata);
	for ( i = 0; i < size; i++)
	{
		if (a1->iata[i] != a2->iata[i])
			return 0;
	}
	return 1;
}

int compareByCode(const Airport * a1, char * code)//checks if airport IATA code is the same as the airport
{
	int i, size = strlen(a1->iata);
	for (i = 0; i < size; i++)
	{
		if (a1->iata[i] != code[i])
			return 0;
	}
	return 1;
}

double distance(const Airport * a1, const Airport * a2)//calculating the distance between two airports
{
	double dLon, dLat, a, c, d, f;
	dLon = a1->lon - a2->lon;
	dLat = a1->lat - a2->lat;
	a = sin(dLat / DEVIDE)*sin(dLat / DEVIDE) + (double)cos(a1->lat) * 
		(double)cos(a2->lat) * (sin(dLon / DEVIDE)*sin(dLon / DEVIDE));
	f = (1-a);
	c = DEVIDE * (double)atan2((double)sqrt(a), (double)sqrt(f));
	d = R * c;
	return d;
}

int newAp(Airport * a)//making new airport - allocates and get from user
{
	char str[MAXSTR];
	fflush(stdin);
	fgets(str, IATA+1,stdin);
	while (!checkIATA(str))
	{
	printf("please enter IATA (must be 3 digit and uppercase)");
	fgets(str, IATA+1, stdin);
	} 
	//fflush(stdin);
	a->iata = (char*)malloc(sizeof(char)*(IATA+1));
	strcpy(a->iata, str);
	fgets(str, MAXSTR, stdin);
	printf("please enter name of airport");
	fgets(str, MAXSTR, stdin);
	deleteSpaces(str);
	printf("\n%s \n ", str);
	a->ap = (char*)malloc(sizeof(char)*(strlen(str) + 1));
	strcpy(a->ap, str);
	printf("please enter country\n");
	fgets(str, MAXSTR, stdin);
	a->country = (char*)malloc(sizeof(char)*(strlen(str) + 1));
	strcpy(a->country, str);
	getCoordinates(a);

	return 0;
}
int apFromFile(Airport * a, FILE * f)//name ,country , IATA ,lat , lon
{
	char str[MAXSTR];
	fgets(str, MAXSTR, f);
	fgets(str, MAXSTR, f);
	a->ap = (char*)malloc(sizeof(char)*(strlen(str)+1));
	strcpy(a->ap, str);
	fgets(str, MAXSTR, f);
	a->country = (char*)malloc(sizeof(char)*(strlen(str)+1));
	strcpy(a->country, str);
	fgets(str, IATA + 1, f);
	a->iata = (char*)malloc(sizeof(char)*(IATA + 1));
	strcpy(a->iata, str);
	fgets(str, MAXSTR, f);
	fscanf(f,"%lf", &a->lat);
	fscanf(f, "%lf", &a->lon);
	return 0;
}
void getCoordinates(Airport* a)// get coordinates from user and checks borders
{
	while (!((-90 <= a->lat && a->lat <= 90) && (-180 <= a->lon && a->lon <= 180)))
	{
		printf("please enter longitude (range: [-180,180])\n");
		scanf("%lf", &a->lon);
		printf("please enter latitude (range: [-90,90])\n");
		scanf("%lf", &a->lat);
		if(!((-90 <= a->lat && a->lat <= 90) && (-180 <= a->lon && a->lon <= 180)))
		printf("Try again\n");
	}
	
}

int checkIATA(char * code)//checks if code is OK
{
	int i,size=strlen(code);
	if (size != 3)
		return 0;
	for ( i = 0; i < size; i++)
	{
		if (!(code[i] >= 'A' && code[i] <= 'Z'))
			return 0;
	}
	return 1;
}

void deleteSpaces(char * str)//method that erase multiple spaces
{
	int  ip_ind = 0;
	while (*(str + ip_ind) != 0) {
		if (*(str + ip_ind++) == 32) {
			if (*(str + ip_ind) == 32) {
				int x = ip_ind;
				while (*(str + x + 1) != 0) {
					*(str + x) = *(str + 1 + x++);
				}
				*(str + x) = 0;
				--ip_ind;
			}
		}
	}
	makeUpper(str);
}
void freeAp(Airport * a)//free airport
{
	free(a->ap);
	free(a->country);
	free(a->iata);
	free(a);

}
int makeUpper(char * str)//method that convert first char to uppercase
{
	int i,n=strlen(str);
	for (i = 0; i < n; i++) {
		if (i == 0 && str[i] != ' ' || str[i] != ' ' && str[i - 1] == ' ') {
			if (str[i] >= 'a' && str[i] <= 'z') {
				str[i] = (char)(('A' - 'a') + str[i]);
			}
		}
		else if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] = (char)(str[i] + ('a' - 'A'));
		}
	}
	return 0;
}
void printAirport(void* airPort)
{
	Airport* a = *(Airport**)airPort;
	printf("\nName of airport: %s | Name of country :%s | IATA : %s | longitude %lf | latitude : %lf |\n"
		, a->ap, a->country, a->iata, a->lon, a->lat);
}
