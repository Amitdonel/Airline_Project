#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"
#include "List.h"

#define FROM_FILE 1
#define FROM_USER 2

typedef struct
{
	LIST airportsList;
}AirportManager;

int		 initManager(AirportManager* pManager, const char* fileName);
int		 addAirport(AirportManager* pManager);
int		 initAirport(Airport* pPort, AirportManager* pManager);
Airport* findAirportByCode(const AirportManager* pManager, const char* code);
void	 printAirports(const AirportManager* pManager);
void	 freeManager(AirportManager* pManager);
int		 getAirportCount(const AirportManager* pManager);
int		 readManagerFromFile(const char* fileName, AirportManager* pManager);
int		 saveManagerToFile(const char* fileName, const AirportManager* pManager);
#endif 