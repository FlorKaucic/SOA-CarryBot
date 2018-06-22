#include "BTHandler.h"

/**

  Parametros:
	HardwareSerial * ser 	- El serial a utilizar para comunicarse con el modulo

*/
BTHandler::BTHandler(HardwareSerial * ser) {
	this->serial = ser;
}

/**

	Retorno
	int - Entero, valor de iqnm_max_mediciones

*/
int BTHandler::get_inqm_max_mediciones() {
	return this->inqm_max_mediciones;
}

/**

	Parametros
	int - Entero,maximo de mediciones a hacer en cada AT+INQ

*/
int BTHandler::set_inqm_max_mediciones(const int inqm_max_med) {
	this->module_status = MODULE_SET_PENDING;
	this->inqm_max_mediciones = inqm_max_med;
	this->mediciones = new t_medicion[inqm_max_med];

	if (!this->mediciones) {
		return MEM_NOT_ALLOCATED;
	}
	return SUCCESS;
}

/**

	Retorno
	int	-	Entero que representa el timeout en AT+INQM

*/
int BTHandler::get_inqm_timeout() {
	return this->inqm_timeout;
}

/**

	Parametros:
  int   timeout - Entero (medido en seg) entre 1 y 48 que se usa en AT+INQM.
  								Tener en cuenta que el Modulo hace val*1.28.
*/
void BTHandler::set_inqm_timeout(const int timeout) {
	this->inqm_timeout = timeout;
}

/**

	Retorno:
	int	-	1 	si el modulo responde OK,
				0 	si responde otra cosa.

*/
int BTHandler::at_ready() {
	String reply;

	serial->print("AT\r\n"); //envio AT

	delay(SHORT_DELAY); //genero un breve delay

	if (serial->available()) { // hay algo para leer
		reply = serial->readString();
	}

	if (reply.length()) {
		if (reply.equals("OK\r\n")) {
			return SUCCESS; // respondio OK\r\n
		}
	}

	return FAILURE; // si no respondio nada, o respondio otra cosa.
}

/**

	Retorno:
	int	-	1 	si el modulo responde OK,
				0 	si responde otra cosa.

*/
int BTHandler::at_init() { // AT+INIT
	String reply = "";
	int aux;
	serial->print("AT+INIT\r\n"); //envio AT+INIT
	delay(DEFAULT_DELAY); //genero un breve delay
	if (serial->available()) // hay algo para leer
		reply = serial->readString();

	if (reply.length()) {
		if (reply.equals("OK\r\n")) { // Si responde OK, no use INIT antes
			return SUCCESS;
		}
		if (reply.equals("ERROR:(17)\r\n")) {
			return SUCCESS;
		}
	}

	return FAILURE; // si no respondio nada, o responde algo inesperado
}

/**

	Parametros:
	int		role	- Role a asignar a AT+ROLE (1: Maestro, 0: Esclavo)
		
	Retorno:
	int	-	1 	si el modulo responde OK,
		   	0 	si responde otra cosa.

*/
int BTHandler::at_role(e_roles role) { // AT+ROLE
	String command = role == e_roles::MASTER ? "AT+ROLE=1\r\n" : "AT+ROLE=0\r\n";
	String response = "";

	serial->print(command);

	delay(DEFAULT_DELAY);

	if (serial->available()) {
		response = serial->readString();
	}

	if (response.length() && response.equals("OK\r\n")) {
			return SUCCESS;
	}

	return FAILURE;
}

/**

	Retorno:
	int	-	1 	si el modulo responde OK,
	      0 	si responde otra cosa.

*/
int BTHandler::at_inqm() {
	String reply = "";
	char buffer[21];


	String msg = "AT+INQM=";
	msg += "1,";
	itoa(this->inqm_max_mediciones, buffer, 10);
	msg += buffer;
	msg += ",";
	itoa(this->inqm_timeout, buffer, 10);
	msg += buffer;
	msg += "\r\n";

	serial->print(msg);

	delay(DEFAULT_DELAY);

	while (serial->available()) {
		reply += serial->readString();
	}

	if (reply.length() && reply.equals("OK\r\n")) {
		return SUCCESS;
	}

	return FAILURE;
}

/**

	Retorno:
	int -	El valor de module_status

*/
int BTHandler::get_module_status() {
	return this->module_status;
}

/**

	Retorno:
	int	-	AT_GOES_WRONG		si no logra usar el comando AT
				AT_READY 				si logra usar AT pero falla AT+INIT
				AT_INITIALIZED 	si logra usar AT+INIT pero falla AT+ROLE
				AT_ROLE_SET			si logra usar AT+ROLE pero falla AT+INQM
				AT_INQM_SET			si logra usar AT+INQM

*/
int BTHandler::set_module() { // Prepara el modulo para realizar mediciones.
	if (at_ready() != SUCCESS) {  // se debe estar en modo AT
		this->module_status = AT_GOES_WRONG;
		return AT_GOES_WRONG;
	}

	this->module_status = AT_READY;
	if (at_init() != SUCCESS) { // se debe cargar SPP lib
		return AT_READY;
	}

	this->module_status = AT_INITIALIZED;
	if (at_role(e_roles::MASTER) != SUCCESS) { // se debe estar en role Maestro (1)
		return AT_INITIALIZED;
	}

	this->module_status = AT_ROLE_SET;
	if (at_inqm() != SUCCESS) {   // se deben settear los parametros para INQ
		return AT_ROLE_SET;
	}

	this->module_status = AT_INQM_SET;

	return AT_INQM_SET;
}

/**

	Retorno:
	int	- >=0								->	Nº de mediciones parseadas.
				-5 (EMPTY_MSG)				-> 	El Mensaje estaba vacio
				-3 (CORRUPTED_MSG)			->	El mensaje estaba incompleto.

*/
int BTHandler::at_inq() {
	String response = "";
	char * message = NULL;
	int result;

	serial->print("AT+INQ\r\n"); //envio "AT+INQ\r\n"
	delay(LONG_DELAY);

	String endOfString;
	while(!endOfString.equals("OK\r\n")) {
		response += serial->readString();
		endOfString = response.substring(response.length()-4, response.length());
	}

	result = EMPTY_MSG;

	if (response.length()) {
		message = new char[response.length() + 1];
		response.toCharArray(message, response.length());
		result = BTParser::parse(message, mediciones);
	}

	return result;
}

/**
 * Receives an address and returns the average of the values obtained by inquiring RSSI values
 * @param address - pointer to a char array with the address to look for (formatted as NAP:UAP:LAP)
 * @return an integer for the average reading
 */
int BTHandler::get_average_reading_for_address(const char * address) {
	if (!this->mediciones) {
		return OBJECT_NOT_FOUND;
	}

	int number_of_reading_values = 0;
	int average_noise_level = 0;
	int i = 0;

	while(i < inqm_max_mediciones) {
		Serial.print("i:");
		Serial.println(i);
		Serial.print("address:");
		Serial.println(mediciones[i].address);
		Serial.print("rssi:");
		Serial.println(mediciones[i].dec_rssi);
		if(!strcmp(address, mediciones[i].address)) {
			number_of_reading_values++;
			if(number_of_reading_values > 1) {
				average_noise_level = (average_noise_level + mediciones[i].dec_rssi) / 2;
			} else {
				average_noise_level = mediciones[i].dec_rssi;
			}
		}
		i++;
		Serial.print("avr:");
		Serial.print(average_noise_level);
	}

	if(!number_of_reading_values) {
		return ADDRESS_NOT_FOUND;
	}

	return average_noise_level;
}
