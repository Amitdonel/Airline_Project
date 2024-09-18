#ifndef __FLIGHT__
#define __FLIGHT__

#include "AirportManager.h"
#include "Date.h"
#include "Plane.h"


typedef struct
{
	char	sourceCode[IATA_LENGTH + 1];
	char	destCode[IATA_LENGTH + 1];
	Plane	flightPlane;
	Date	date;
}Flight;

void		initFlight(Flight* pFlight, Plane* thePlane,const AirportManager* pManager);
int			isFlightFromSourceAirport(const Flight* pFlight, const char* code);
int			isFlightToDestAirport(const Flight* pFlight, const char* code);
void		printFlight(const Flight* pFlight);
void		printFlightVoid(const void* pFlight);
Airport*	setAiportToFlight(const AirportManager* pManager, const char* msg);
int			isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type);
int			compareFlightBySrcCode(const void* flight1, const void* flight2);
int			compareFlightByDestCode(const void* flight1, const void* flight2);
int			compareFlightByDate(const void* flight1, const void* flight2);
int			readFlightFromFile(FILE* fp, Flight* pFlight, AirportManager* pManager);
void		freeFlightVoid(void* pFlight);
void		freeFlight(Flight* pFlight);

#endif
