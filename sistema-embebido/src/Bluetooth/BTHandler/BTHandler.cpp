#include "BTHandler.h"



/**

    Parametros:
	int* 		mem_fail	- Output: NULL o MEM_NOT_ALLOCATED si falla al pedir memoria
	const int 	baudrate 	- El baudrate a utilizar para comunicarse con el modulo
	const int  	max_med 	- Maximo de mediciones, valor que emplea el modulo en INQM.
	const int  	timeout 	- Entre 1 y 48. IMPORTANTE: El HC-05 hace val*1.28.

*/
BTHandler::BTHandler(int* mem_fail, const int baudrate, const int max_med, const int timeout){
	
	Serial2.begin(baudrate);
	this->baudrate;
    this->inqm_max_mediciones = max_med;
	this->inqm_timeout = timeout;
	this->mediciones = new t_medicion[max_med];
	
	if ( !this->mediciones )
		*mem_fail = MEM_NOT_ALLOCATED;
	else
		*mem_fail = NULL;
	
}



/**

	Retorno
	int		-	Entero, valor de iqnm_max_mediciones

*/
int	BTHandler::get_inqm_max_mediciones(){ return this->inqm_max_mediciones; }


/**

    Retorno
	int		-	Entero, 

*/
int	BTHandler::set_inqm_max_mediciones(const int inqm_max_med){ 
	
	this->module_status = MODULE_SET_PENDING;
	delete[] this->mediciones;
	this->inqm_max_mediciones = inqm_max_med;
	this->mediciones = new t_medicion[inqm_max_med];
	
	if ( !this->mediciones ){
		return MEM_NOT_ALLOCATED;
	}
	return SUCCESS;
	
}




/**

	Retorno
	int		-	Entero que representa el timeout en AT+INQM

*/
int BTHandler::get_inqm_timeout(){ return this->inqm_timeout; }



/**

    Parametros:
        int   timeout - Entero (medido en seg) entre 1 y 48 que se usa en AT+INQM. 
						Tener en cuenta que el Modulo hace val*1.28.

*/
void BTHandler::set_inqm_timeout(const int timeout){ this->inqm_timeout = timeout; }



/**

    Parametros:
        int   timeout - Timeout (en ms) a utilizar. Si es <= 0 se usa el timeout por defecto
		
	Retorno:
		int	-	1 	si el modulo responde OK, 
				0 	si responde otra cosa.

*/
int BTHandler::at_ready(const int timeout){
	
	String	reply = "";
	
	prepare_serial_comm(timeout); // flush y settimeout
	
	Serial2.write("AT\r\n"); //envio AT
	
	delay( COMM_DELAY ); //genero un breve delay
	
	while( Serial2.available() > 0 ) // hay algo para leer
		reply += Serial2.read();
	
	
	if ( reply.length() > 0 ){
		
		if( reply.equals("OK\r\n") )
			return SUCCESS; // respondio OK\r\n
		
	}
	
	return FAILURE; // si no respondio nada, o respondio otra cosa.
}



/**

    Parametros:
        int   timeout - Timeout (en ms) a utilizar. Si es <= 0 se usa el timeout por defecto
		
	Retorno:
		int	-	1 	si el modulo responde OK, 
				0 	si responde otra cosa.

*/
int	BTHandler::at_init(const int timeout){ // AT+INIT
	
	String reply = "";
	int	aux;
	
	prepare_serial_comm(timeout); // flush y settimeout
	
	Serial2.write("AT+INIT\r\n"); //envio AT
	
	delay( COMM_DELAY ); //genero un breve delay
	
	while( Serial2.available() > 0 ) // hay algo para leer
		reply += Serial2.read();
	
	
	if ( reply.length() > 0 ){
		
		if ( reply.equals("OK\r\n") ) // Si responde OK, no use INIT antes
			return SUCCESS;
		else{
				// estructura de error -> ERROR:(error_code)
			if ( reply.length() > 5 ) //la palabra error son 5 char :)
				if ( reply.substring(0,5).equalsIgnoreCase("error") ){
					
					aux = reply.substring(7,9).toInt();
					
					if ( aux == 17 ) // ERROR:(17) -> SPP lib has been repeated initialization. 
						return SUCCESS; // ya estaba inicializado
					
				}
				
				
			}

	}
	
	return FAILURE; // si no respondio nada, o responde algo inesperado
	
}




/**

    Parametros:
		int		role	- Role a asignar a AT+ROLE (1: Maestro, 0: Esclavo)
        int		timeout - Timeout (en ms) a utilizar. Si es <= 0 se usa el timeout por defecto
		
	Retorno:
		int	-	1 	si el modulo responde OK, 
				0 	si responde otra cosa.

*/
int	BTHandler::at_role(const int role, const int timeout){ // AT+ROLE

	String	reply = "";
	String	msg = "AT+ROLE=";
	char	msg_buffer[15];
	
	prepare_serial_comm(timeout); // flush y settimeout
	
	msg += (role + "\r\n");
	// write no acepta String, tengo que pasarlo a un array de char
	
	msg.toCharArray(msg_buffer, msg.length());
	
	Serial2.write( msg_buffer ); //envio AT+ROLE=%d\r\n, %d el role del parametro
	
	delay( COMM_DELAY ); //genero un breve delay
	
	while( Serial2.available() > 0 ) // hay algo para leer
		reply += Serial2.read();
	
	if ( reply.length() > 0 ){
		
		if( reply.equals("OK\r\n") )
			return SUCCESS; // respondio OK\r\n, role setteado.
		
	}
	
	return FAILURE; // si no respondio nada, o respondio otra cosa.

}



/**

    Parametros:
        int   timeout - Timeout (en ms) a utilizar. Si es <= 0 se usa el timeout por defecto
		
	Retorno:
		int	-	1 	si el modulo responde OK, 
				0 	si responde otra cosa.

*/
int	BTHandler::at_inqm(const int timeout){ // AT+INQM

	String	reply = "";
	String	msg = "AT+INQM=";
	char	msg_buffer[15];
	
	prepare_serial_comm(timeout); // flush y settimeout
	
	msg += ("1,"+ this->inqm_max_mediciones);
	msg += ("," + this->inqm_timeout);
	msg += "\r\n";
	
	// write no acepta String, tengo que pasarlo a un array de char
	
	msg.toCharArray(msg_buffer, msg.length());
	
	Serial2.write( msg_buffer ); //envio "AT+INQM=1,%d,%d\r\n" 
	
	delay( COMM_DELAY ); //genero un breve delay
	
	while( Serial2.available() > 0 ) // hay algo para leer
		reply += Serial2.read();
	
	
	if ( reply.length() > 0 ){
		
		if( reply.equals("OK\r\n") )
			return SUCCESS; // respondio OK\r\n, role setteado.
		
	}
	
	return FAILURE; // si no respondio nada, o respondio otra cosa.
	
	
}



/*
	Retorno:
	int -	El valor de module_status

*/
int BTHandler::get_module_status(){	return this->module_status; }


/**

	Retorno:
		int	-	AT_GOES_WRONG	si no logra usar el comando AT
				AT_READY 		si logra usar AT pero falla AT+INIT
				AT_INITIALIZED 	si logra usar AT+INIT pero falla AT+ROLE
				AT_ROLE_SET		si logra usar AT+ROLE pero falla AT+INQM
				AT_INQM_SET		si logra usar AT+INQM

*/
int	BTHandler::set_module(){ // Prepara el modulo para realizar mediciones.
	
	
	if ( at_ready(0) != 1 ){ 	// se debe estar en modo AT
		this->module_status = AT_GOES_WRONG;
		return AT_GOES_WRONG;
	}
	this->module_status = AT_READY;
	if ( at_init(0) != 1 ) 		// se debe cargar SPP lib
		return AT_READY;
	
	this->module_status = AT_INITIALIZED;
	if ( at_role(1, 0) != 1 )	// se debe estar en role Maestro (1)
		return AT_INITIALIZED;
	
	this->module_status = AT_ROLE_SET;
	if ( at_inqm(0) != 1 ) 		// se deben settear los parametros para INQ
		return AT_ROLE_SET;
	
	this->module_status = AT_INQM_SET;
	
	return AT_INQM_SET;
	
}


/*

	Retorno:
	int	- 	>=0						->	Nº de mediciones parseadas.
			-1 (EMPTY_MSG)			-> 	El Mensaje estaba vacio
			-2 (MEM_NOT_ALLOCATED)	->	this->mediciones == null o no se pudo pedir memoria
			-3 (CORRUPTED_MSG)		->	El mensaje estaba incompleto.
			-4 (MODULE_NOT_SET)		->  si this->module_status != AT_INQM_SET

*/
int BTHandler::at_inq(){
	
	String	reply = "";
	char*	msg = NULL;
	int		comm_timeout = ((int)(this->inqm_timeout*1.28)),
			rtn;
	
	if ( this->module_status != AT_INQM_SET )
		return MODULE_NOT_SET;
	
	prepare_serial_comm( comm_timeout ); // flush y settimeout
	
	Serial2.write( "AT+INQ\r\n" ); //envio "AT+INQ\r\n"
	
	delay( COMM_DELAY ); //genero un breve delay
	
	while( Serial2.available() > 0 ) // hay algo para leer
		reply += Serial2.read();
	
	rtn = EMPTY_MSG;
	
	if ( reply.length() > 0 ){
		
		msg = new char[ reply.length() + 1 ];
		
		if ( !msg ) // ocurrio un error al pedir memoria.
			return MEM_NOT_ALLOCATED;
		
		reply.toCharArray( msg, reply.length() );		
		
		rtn = BTParser::parse( msg , this->mediciones );		
		
	}
	
	if ( msg != NULL )
		delete[] msg;
	
	return rtn; // no puede no responder nada, habria que reiniciar el modulo.
	
}



// Retorna >= 0 si existe en el array, -1 si no existe, y -2 si no hay array de mediciones
/*int BTHandler::find_address(const char* address){ //recibe la direccion a buscar

    int i=0;
    if ( this->mediciones == NULL  )
        return -2; // no hay array de mediciones

    while( i < this->inqm_max_mediciones && !strcmp(address, this->mediciones[i].address ) )
        i++;
    return i;

}*/

int BTHandler::check_timeout(const int comm_timeout){
	return comm_timeout <= 0 ? COMM_TIMEOUT : comm_timeout;
}

void BTHandler::prepare_serial_comm(const int comm_timeout){
	
	Serial2.setTimeout( check_timeout(comm_timeout) ); // defino un timeout para la comunicacion
	
}

BTHandler::~BTHandler(){ delete[] mediciones; }


