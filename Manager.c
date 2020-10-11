/*
 *
 *
 *  Created on: 27 April 2020
 *      Author: Nadav Shlomo Shaoulian 
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Manager.h"
#include "Airport.h"
#define IATA 4
#define MAXSTR 300

int addAirport(Manager* m)// adding airport to manager , at first checks if exist by code IATA
{
	char str[IATA] = {0};

	printf("please enter IATA code :");
	fgets(str, IATA, stdin);
	while (!checkIATA(str))
	{
		printf("code IATA must be 3 digit and uppercase");
		fgets(str, IATA, stdin);
	}
	if (findAirport(m, str) == NULL)
	{
		m->aps = (Airport **)realloc(m->aps, (m->numOfAps + 1) * sizeof(Airport*));
		if (!m)
			return 0;
		m->aps[m->numOfAps] = (Airport *)malloc(sizeof(Airport));
		newAp(m->aps[m->numOfAps]);
		m->numOfAps++;
		return 1;
	}
	else
	{
		printf("There is allready one with IATA :%s", str);
			return 0;
	}
		
}

Airport* findAirport(Manager* m ,char * code)//find airport by IATA code
{
	int i;
	for ( i = 0; i < m->numOfAps ; i++)
	{
		if (compareByCode(m->aps[i],code)==1)
			return m->aps[i];
	}
	return NULL;
}

Manager* initMan(int num ,FILE* f)//initiate new manager
{
	Manager* m;
	int i;
	m = (Manager *)malloc(sizeof(Manager));
	if (!m)
		return NULL;
	m->numOfAps = num;
	m->aps = (Airport **)malloc(sizeof(Airport*)*num);
	if (!m->aps)
		return NULL;
	for ( i = 0; i < num; i++)
	{
		m->aps[i] = (Airport *)malloc(sizeof(Airport));
		apFromFile(m->aps[i],f);
	}

	return m;
}

int printManagaerStats(Manager * m)//printing stats of manager
{
	
	int i;
	for ( i = 0; i < m->numOfAps; i++)
	{
		printf("\nAirport number %d\n", i + 1);
		printf("Name of airport: %s | Name of country :%s | IATA : %s | longitude %lf | latitude : %lf |\n"
			, m->aps[i]->ap, m->aps[i]->country, m->aps[i]->iata, m->aps[i]->lon, m->aps[i]->lat);
	}
	return 0;
}

void freeManager(Manager * m)//free manager
{
	int i;
	for (i = 0; i < m->numOfAps; i++)
	{
		freeAp(m->aps[i]);
	}
	free(m);


}
