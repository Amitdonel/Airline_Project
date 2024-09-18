#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "Airport.h"
#include "General.h"
#include "list.h"

int	initManager(AirportManager* pManager, const char* fileName)
{
	if (!L_init(&pManager->airportsList))
		return 0;

	if (!readManagerFromFile(fileName, pManager))
	{
		freeManager(pManager);
		return FROM_USER;
	}
	return FROM_FILE;
}

int	addAirport(AirportManager* pManager)
{
	Airport* pPort = (Airport*)calloc(1, sizeof(Airport));
	if (!pPort)
		return 0;

	if (!initAirport(pPort, pManager))
	{
		freeManager(pManager);
		free(pPort);
		return 0;
	}

	if (!&pManager->airportsList)
	{
		freeManager(pManager);
		return 0;
	}
	return 1;
}

int  initAirport(Airport* pPort, AirportManager* pManager)
{
	while (1)
	{
		getAirportCode(pPort->code);
		if (L_insert(&pManager->airportsList.head, pPort, compareAirportByCode))
			break;

		printf("This code already in use - enter a different code\n");
	}
	return initAirportNoCode(pPort);
}

Airport* findAirportByCode(const AirportManager* pManager, const char* code)
{
	Airport toSearch;
	strcpy(toSearch.code, code);
	NODE* pNode = L_find(pManager->airportsList.head.next, &toSearch, compareAirportByCode);
	if (pNode)
		return pNode->key;
	return NULL;
}

int getAirportCount(const AirportManager* pManager) {
	int count = 0;
	if (!pManager->airportsList.head.next)
		return count;

	NODE* pNode = pManager->airportsList.head.next;
	while (pNode != NULL) {
		count++;
		pNode = pNode->next;
	}
	return count;
}

void	printAirports(const AirportManager* pManager)
{
	int count = getAirportCount(pManager);
	printf("there are %d airports\n", count);
	L_print(&pManager->airportsList, printAirport);
}

int saveManagerToFile(const char* fileName, const AirportManager* pManager)
{
	FILE* fp = fopen(fileName, "w");
	if (!fp)
		return 0;

	int count = getAirportCount(pManager);
	fprintf(fp, "%d\n", count);

	NODE* pNode = pManager->airportsList.head.next;
	while (pNode != NULL) {
		writeAirportToTextFile(fp, pNode->key);
		pNode = pNode->next;
	}
	fclose(fp);
	return 1;
}

int		readManagerFromFile(const char* fileName, AirportManager* pManager)
{
	FILE* fp = fopen(fileName, "r");
	if (!fp)
		return 0;

	//read number of airports
	int count;
	fscanf(fp, "%d\n", &count);

	Airport* pPort = (Airport*)malloc(sizeof(Airport));
	if (!pPort)
	{
		fclose(fp);
		return 0;
	}

	while (readAirportFromTextFile(fp, pPort))
	{
		if (!L_insert(&pManager->airportsList.head, pPort, compareAirportByCode))
		{
			free(pPort);
			fclose(fp);
			return 0;
		}
		pPort = (Airport*)malloc(sizeof(Airport));
	}
	free(pPort);
	fclose(fp);
	return 1;
}


void	freeManager(AirportManager* pManager)
{
	L_free(&pManager->airportsList, free);
}
