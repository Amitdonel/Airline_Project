#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"
#include "General.h"


void	initFlight(Flight* pFlight, Plane* thePlane, const AirportManager* pManager)
{
	Airport* pPortOr = setAiportToFlight(pManager, "Enter code of origin airport:");
	strcpy(pFlight->sourceCode, pPortOr->code);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Enter code of destination airport:");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport\n");
	} while (same);
	strcpy(pFlight->destCode, pPortDes->code);
	pFlight->flightPlane = *thePlane;
	getCorrectDate(&pFlight->date);
}

int		isFlightFromSourceAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->sourceCode, code) == 0)
		return 1;

	return 0;
}


int		isFlightToDestAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->destCode, code) == 0)
		return 1;

	return 0;

}

int		isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type)
{
	return (pFlight->flightPlane.type == type);
}

void printFlightVoid(const void* pFlight)
{
	const Flight* pF = *(const Flight**)pFlight;
	printFlight(pF);
}

void	printFlight(const Flight* pFlight)
{
	printf("Flight From %s To %s\t", pFlight->sourceCode, pFlight->destCode);
	printDate(&pFlight->date);
	printf("\t");
	printPlane(&pFlight->flightPlane);
}

Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s\t", msg);
		getAirportCode(code);
		port = findAirportByCode(pManager, code);
		if (port == NULL)
			printf("No airport with this code - try again\n");
	} while (port == NULL);

	return port;
}

//compare functions
int compareFlightBySrcCode(const void* flight1, const void* flight2)
{
	const Flight* f1 = *(const Flight**)flight1;
	const Flight* f2 = *(const Flight**)flight2;
	return strcmp(f1->sourceCode, f2->sourceCode);
}

int compareFlightByDestCode(const void* flight1, const void* flight2)
{
	const Flight* f1 = *(const Flight**)flight1;
	const Flight* f2 = *(const Flight**)flight2;
	return strcmp(f1->destCode, f2->destCode);
}

int compareFlightByDate(const void* flight1, const void* flight2)
{
	const Flight* f1 = *(const Flight**)flight1;
	const Flight* f2 = *(const Flight**)flight2;

	if (f1->date.year != f2->date.year)
		return f1->date.year - f2->date.year;
	if (f1->date.month != f2->date.month)
		return f1->date.month - f2->date.month;
	return f1->date.day - f2->date.day;
}

//file functions
int readFlightFromFile(FILE* fp, Flight* pF, const AirportManager* pManager)
{
	if (fread(pF, sizeof(Flight), 1, fp) != 1)
	{
		printf("Error read flight\n");
		return 0;
	}
	return 1;
}

int writeFlightToFile(FILE* fp, const Flight* pF )
{
	if (fwrite(pF, sizeof(Flight), 1, fp) != 1)
	{
		printf("Error write flight\n");
		return 0;
	}
	return 1;
}

void freeFlightVoid(void* pFlight)
{
	freeFlight(*(Flight**)pFlight);
}
void freeFlight(Flight* pFlight)
{
	free(pFlight);
}




