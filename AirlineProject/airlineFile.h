#pragma once
#include "airline.h"

int		writeStringToFile(char* str, FILE* fp, const char* msg);
int		writeIntToFile(int val, FILE* fp, const char* msg);
int		 writeCharsToFile(char* str, int length, FILE* fp, const char* msg);
char*	readStringFromFile(FILE* fp, const char* msg);
int		 readIntFromFile(int* pVal, FILE* fp, const char* msg);