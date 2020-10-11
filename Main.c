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

#include "Airport.h"
#include "Flight.h"
#include "Manager.h"
#include "Airline.h"

int main()
{
	Manager *m ;
	Airline * comp;
	Flight *f1 = NULL,*f2=NULL;
	int numOfAirports,option,stop = 0;
	FILE *f;
	f = fopen("airport_authority.txt", "r");
	fscanf(f,"%d", &numOfAirports);
	m = initMan(numOfAirports,f);
	comp = initAirline(m);
	fclose(f);
		do
		{
			printf("\n\n");
			printf("Please choose one of the following options\n");
			printf("1 - Add flight to airline company\n");
			printf("2 - Add airport to airport manager \n");
			printf("3 - Print all airline stats\n");
			printf("4 - Print all airport managers stats\n");
			printf("5 - Print how many flight in 2 airports\n");
			printf("6 - Free Flights\n");
			printf("7 - Sort the airports \n");
			printf("8 - Binary searching flight \n");
			printf("9 - Departure flights \n");
			printf("10 - Free and Quit\n");
			scanf("%d", &option);
			switch (option)
			{
			case 1:
				addFlightToAirline(comp,m,NULL);
				printToFile(comp,m, "company.bin", "b");
				break;
			case 2:
				addAirport(m);
				printToFile(comp,m, "airport_authority.txt","text");
				break;
			case 3:
				printAirlineStats(comp);
				break;
			case 4:
				genericArr((m->aps), m->numOfAps, sizeof(Airport*), printAirport);
				break;
			case 5:
				howManyFlights(comp->flights,comp->numOfFlights);
				break;
			case 6:
				genericArr(comp->flights,comp->numOfFlights , sizeof(Flight*), freeF);
				comp->numOfFlights = 0;
				break;
			case 7:
				sortTheFlights(comp);
				break;
			case 8:
				f1= bsearchFlight(comp);//getting pointer to flight from bsearch
				f2 = bsearchFlight(comp);//the second one for the variadic function
				break;
			case 9:
				departureFlights(f1,f2,NULL);
				break;
			case 10:
				freeAll(m, comp);
				printf("Bye bye\n");
				stop = 1;
				break;
			default:
				printf("Wrong option\n");
				break;
			}
		} while (!stop);
	}
	
