#ifndef BTPARSER_H
#define BTPARSER_H


#include <string.h>
#include <stdio.h>

using namespace std;

typedef struct {

    char    address[20];
    char    hex_rssi[10];
    int     dec_rssi;

} t_medicion;

typedef enum {
		
		EMPTY_MSG		= -1,
		NO_ARRAY_ALLOC	= -2,
		CORRUPTED_MSG	= -3
		

} e_parsing_err;

class BTParser
{
    public:

        static int parse(char* , t_medicion* );

    private:

        // Removido todo lo private

};

#endif // BTPARSER_H
