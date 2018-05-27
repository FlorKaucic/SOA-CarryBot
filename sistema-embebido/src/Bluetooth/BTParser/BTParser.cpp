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
int BTParser::parse(char* msg, t_medicion* mediciones){

    char* indI= msg, *indF = msg;
    int i=0;

    if ( !msg || !*msg ) 
        return EMPTY_MSG;

    if ( !mediciones )
        return NO_ARRAY_ALLOC;

    // comienzo con el puntero posicionado en el primer +
    while ( indI && *indI ){ //mientras no sea NULL ni \0

        /*

            la siguiente medicion debe de comenzar luego del \0
            ademas lo muevo aca, antes estaba abajo de todo pero si por alguna razon
            indF fuera NULL, indI seria NULL+1 y no podria atrapar el valor nulo en el while

        */
        indI = indF+1;


        /// INICIO BUSQUEDA DIRECCION/ADDRESS ///
        indI = strchr(indI, ':');
        // si por alguna razon no encuentra ":", pude haber llegado al final o haber un error
        if ( !indI ){
            // hubo un error, o no quedan mas "mediciones" en el mensaje
            if ( !strchr(indF, '\r') || !strchr(indF, '\n') )
                return CORRUPTED_MSG; // Hubo un error
            // si no hubo un error, dejo que salga del ciclo por indI == NULL, con normalidad
        }
        else{
            // tomo el sig de ":", no sumo antes porque si strchr retorna null, null+1 es incomparable contra NULL
            indI = indI+1;
            indF = strchr(indI, ',');

            // si por alguna razon no encuentra ",", hubo algun problema en el mensaje
            if ( !indF )
                return CORRUPTED_MSG;

            *indF = '\0';
            indF = indF+1; // sino al buscar el RSSI arranco en un \0 y eso rompe.

            if ( !mediciones )
                return NO_ARRAY_ALLOC;  // No hay array de mediciones

            strcpy((mediciones+i)->address , indI);

            /// FIN BUSQUEDA DIRECCION/ADDRESS ///

            /// INICIO BUSQUEDA  RSSI ///

            indI = strchr(indF, ',')+1; // el inicio del valor de RSSI es pos de la "," +1

            // si por alguna razon no encuentra ",", hubo algun problema en el mensaje
            if ( !indI )
                return CORRUPTED_MSG;

            indF = strchr(indF, '\r');

            // si por alguna razon no encuentra "\r", hubo algun problema en el mensaje
            if ( !indF )
                return CORRUPTED_MSG;

            *indF = '\0';


            if ( !mediciones )
                return NO_ARRAY_ALLOC;  // No hay array de mediciones

            strcpy((mediciones+i)->hex_rssi , indI);
			
			// paso el hexa a decimal y lo almaceno
			sscanf((mediciones+i)->hex_rssi, "%x", &((mediciones+i)->dec_rssi));
			
            i++;
            /// FIN BUSQUEDA  RSSI ///

        }

    }

    return i;

}
