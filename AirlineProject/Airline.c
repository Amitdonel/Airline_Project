#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Airport.h"
#include "General.h"
#include "airlineFile.h"

void	initAirline(Airline* pComp)
{
	//printf("-----------  Init Airline\n");
	pComp->name = getStrExactName("Enter Airline name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	pComp->planeArr = NULL;
	pComp->planeCount = 0;
}

int initAirlineFromFile(Airline* pComp, AirportManager* pManager, const char* fileName)
{
	if (!readAirlineFromFile(pComp, pManager, fileName))
		return 0;
	return 1;
}

int	addFlight(Airline* pComp, const AirportManager* pManager)
{
	if (getAirportCount(pManager) < 2)
	{
		printf("There are not enough airport to set a flight\n");
		return 0;
	}
	if (pComp->planeCount == 0)
	{
		printf("There is no plane in company\n");
		return 0;
	}

	Flight* pFlight = (Flight*)calloc(1, sizeof(Flight));
	if (!pFlight)
		return 0;

	Plane* thePlane = FindAPlane(pComp);
	printAirports(pManager);
	initFlight(pFlight, thePlane, pManager);

	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		free(pFlight);
		return 0;
	}
	pComp->flightArr[pComp->flightCount] = pFlight;
	pComp->flightCount++;
	return 1;
}

int		addPlane(Airline* pComp)
{
	pComp->planeArr = (Plane*)realloc(pComp->planeArr, (pComp->planeCount + 1) * sizeof(Plane));
	if (!pComp->planeArr)
		return 0;
	initPlane(&pComp->planeArr[pComp->planeCount], pComp->planeArr, pComp->planeCount);
	pComp->planeCount++;
	return 1;
}

Plane* FindAPlane(Airline* pComp)
{
	printf("Choose a plane from list, type its serial Number\n");
	generalArrayFunction((void*)pComp->planeArr, pComp->planeCount, sizeof(Plane), printPlane);
	int sn;
	Plane* temp = NULL;
	do {
		scanf("%d", &sn);
		temp = findPlaneBySN(pComp->planeArr, pComp->planeCount, sn);
		if (!temp)
			printf("No plane with that serial number! Try again!\n");
	} while (temp == NULL);

	return temp;
}

void printCompany(const Airline* pComp)
{
	printf("Airline %s\n", pComp->name);
	printf("\n -------- Has %d planes\n", pComp->planeCount);
	generalArrayFunction((void*)pComp->planeArr, pComp->planeCount, sizeof(Plane), printPlane);
	printf("\n\n -------- Has %d flights\n", pComp->flightCount);
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight*), printFlightVoid);
}


void	doPrintFlightsWithPlaneType(const Airline* pComp)
{
	ePlaneType type = getPlaneType();
	int count = 0;
	printf("Flights with plane type %s:\n", GetPlaneTypeStr(type));
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneTypeInFlight(pComp->flightArr[i], type))
		{
			printFlight(pComp->flightArr[i]);
			count++;
		}
	}
	if (count == 0)
		printf("Sorry - could not find a flight with plane type %s:\n", GetPlaneTypeStr(type));
	printf("\n");
}

eSortOpt showSortMenu()
{
	int option;
	printf("Please choose a sort option:\n");
	do {
		for (int i = 1; i < eNofsortOpt; i++)
			printf("%d - %s\n", i, sortOptStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option > eNofsortOpt);

	return (eSortOpt)option;
}

void sortFlights(Airline* pComp)
{
	pComp->sortOpt = showSortMenu();
	int(*compare)(const void*, const void*) = NULL;

	switch (pComp->sortOpt)
	{
	case eSrcCode:
		compare = compareFlightBySrcCode;
		break;
	case eDestCode:
		compare = compareFlightByDestCode;
		break;
	case eDate:
		compare = compareFlightByDate;
		break;
	}
	if (compare)
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
}

void findFlight(const Airline* pComp) {
	int(*compare)(const void*, const void*) = NULL;
	Flight f = { 0 };
	Flight* pF = &f;

	switch (pComp->sortOpt)
	{
	case eSrcCode:
		printf("Source code:");
		getAirportCode(f.sourceCode);
		compare = compareFlightBySrcCode;
		break;
	case eDestCode:
		printf("Destination code:");
		getAirportCode(f.destCode);
		compare = compareFlightByDestCode;
		break;
	case eDate:
		getCorrectDate(&f.date);
		compare = compareFlightByDate;
		break;
	}
	if (compare)
	{
		Flight** pFlight = (Flight**)bsearch(&pF, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
		if (pFlight)
			printFlight(*pFlight);
		else
			printf("Flight not found\n");
	}
	else {
		printf("Search cannot be performed, Array not sorted..\n");
	}
}

int     saveAirlineToFile(const Airline* pComp, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp) {
		printf("Error open copmpany file to write\n");
		return 0;
	}
	if (!writeStringToFile(pComp->name, fp, "Error write comapny name\n"))
		return 0;

	if (!writeIntToFile(pComp->flightCount, fp, "Error write flight count\n"))
		return 0;

	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (!writeFlightToFile(fp, pComp->flightArr[i]))
			return 0;
	}
	if (!writeIntToFile(pComp->planeCount, fp, "Error write plane count\n"))
		return 0;
	for (int i = 0; i < pComp->planeCount; i++)
	{
		if (!writePlaneToFile(fp, &pComp->planeArr[i]))
			return 0;
	}
	fclose(fp);
	return 1;
}

int readAirlineFromFile(Airline* pComp, const AirportManager* pManager, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}

	pComp->flightArr = NULL;

	pComp->name = readStringFromFile(fp, "Error reading company name\n");
	if (!pComp->name)
	{
		fclose(fp);
		return 0;
	}

	if (!readFlightsFromFile(pComp, fp, pManager))
	{
		fclose(fp);
		return 0;
	}

	if (!readPlanesFromFile(pComp, fp))
	{
		fclose(fp);
		return 0;
	}

	fclose(fp);
	return 1;
}

int readFlightsFromFile(Airline* pComp, FILE* fp, const AirportManager* pManager)
{
	if (!readIntFromFile(&pComp->flightCount, fp, "Error reading flights count name\n"))
		return 0;

	if (pComp->flightCount > 0)
	{
		pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
		if (!pComp->flightArr)
		{
			printf("Allocation error\n");
			return 0;
		}
	}
	else
		pComp->flightArr = NULL;

	for (int i = 0; i < pComp->flightCount; i++)
	{
		pComp->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
		if (!pComp->flightArr[i])
		{
			printf("Allocation error\n");
			return 0;
		}
		if (!readFlightFromFile(fp, pComp->flightArr[i], pManager))
			return 0;
	}

	return 1;
}

int readPlanesFromFile(Airline* pComp, FILE* fp)
{
	if (!readIntFromFile(&pComp->planeCount, fp, "Error reading planes count name\n"))
		return 0;

	if (pComp->planeCount > 0)
	{
		pComp->planeArr = (Plane*)malloc(pComp->planeCount * sizeof(Plane));
		if (!pComp->planeArr)
		{
			printf("Allocation error\n");
			return 0;
		}
	}
	else
		pComp->planeArr = NULL;

	for (int i = 0; i < pComp->planeCount; i++)
	{
		if (!readPlaneFromFile(fp, &pComp->planeArr[i]))
			return 0;
	}

	return 1;
}

void	freeCompany(Airline* pComp)
{
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight*), freeFlightVoid);
	free(pComp->name);
}


