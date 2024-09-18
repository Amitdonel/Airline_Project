#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"

typedef enum { eNotSorted, eSrcCode, eDestCode, eDate, eNofsortOpt } eSortOpt;
static const char* sortOptStr[eNofsortOpt] = { "None", "Source Code", "Destination Code", "Date" };

typedef struct
{
	char* name;
	int			flightCount;
	Flight**	flightArr;
	int			planeCount;
	Plane*		planeArr;
	eSortOpt 	sortOpt;
}Airline;

void	initAirline(Airline* pComp);
int		addFlight(Airline* pComp, const AirportManager* pManager);
int		addPlane(Airline* pComp);
Plane*  FindAPlane(Airline* pComp);
void	printCompany(const Airline* pComp);
void	doPrintFlightsWithPlaneType(const Airline* pComp);
void	freeCompany(Airline* pComp);

//file functions
int	    initAirlineFromFile(Airline* pComp, AirportManager* pManager, const char* fileName);
int		readAirlineFromFile(Airline* pComp, AirportManager* pManager, const char* fileName);
int     saveAirlineToFile(const Airline* pComp, const char* fileName);

//sort functions
void	sortFlights(Airline* pComp);
void	findFlight(const Airline* pComp);
eSortOpt showSortMenu();


#endif

