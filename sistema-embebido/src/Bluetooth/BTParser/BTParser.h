#ifndef BTPARSER_H
#define BTPARSER_H

#include <string.h>
#include "Arduino.h"
#include <ArduinoSTL.h>

using namespace std;

const int EQUAL = 0;

typedef struct {
  char    address[20];
  int     dec_rssi;
} t_medicion;

typedef enum {
	NO_ARRAY_ALLOC  = -2,
	CORRUPTED_MSG   = -3
} e_parsing_err;

class BTParser
{
  public:
    static int parse(char* , t_medicion*);
};

#endif // BTPARSER_H
