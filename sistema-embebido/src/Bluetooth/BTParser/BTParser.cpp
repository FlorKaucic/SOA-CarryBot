#include "BTParser.h"

/*

    Algoritmo para procesar Lecturas:
        Por cada "lectura":
        1. Busco el siguiente ":" y guardo su dir+1 en indI.
        2. Desde el indI busco el siguiente "," y guardo su dir en indF
        3. Piso la "," de indF con "\0"
        4. Uso strcpy desde indI hacia el lugar donde vaya a alojar el ID.

        5. Busco la siguiente "," desde indF y le asigno su dir+1 a indI.
        6. Busco el siguiente "\r", guardo su dir en indF.
        7. Piso el "\r" de indF con "\0"
        8. Uso strcpy desde indI hacia el lugar donde vaya a alojar la intensidad (RSSI)

*/
/**

    Parametros:
        char*   msg - String que contiene el mensaje a parsear.
        t_medicion*  mediciones - Dir del array donde alojar las mediciones parseadas.

    Retorno:
        int -   < 0 para errores (ver enum "e_parsing_err")
            -   0 si msg es null, o si no hubo mediciones.
            -   > 0 si se obtuvieron N mediciones.

*/
int BTParser::parse(char * message, t_medicion * readings) {
	vector<char *> lines;
	int i = 0;

	char * parsedLine = strtok(message, "\r\n");

	while(parsedLine) {
		lines.push_back(parsedLine);
		parsedLine = strtok(NULL, "\r\n");
	}
	
	for(char * line : lines) {
		if(strcmp(line, "OK") != EQUAL) {
			if (sscanf(line, "+INQ:%[^,],%*[^,],%x\n", readings[i].address, &readings[i].dec_rssi) != 2) {
				return CORRUPTED_MSG;
			}
			i++;
		}
	}

	return i;
}
