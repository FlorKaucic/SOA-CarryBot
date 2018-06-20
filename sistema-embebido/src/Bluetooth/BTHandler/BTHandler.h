#ifndef BTHANDLER_H
#define BTHANDLER_H

#include <string.h>
#include "Arduino.h"
#include "../BTParser/BTParser.h"

using namespace std;

typedef enum {
	SHORT_DELAY = 30,
	DEFAULT_DELAY = 100,
	LONG_DELAY = 1000,
	EXTRA_LONG_DELAY = 1500
} e_delays; // ms

typedef enum {
	AT_GOES_WRONG = -1,
	MODULE_SET_PENDING = 0,
	AT_READY = 1,
	AT_INITIALIZED = 2,
	AT_ROLE_SET = 3,
	AT_INQM_SET = 4
} e_hc05_status;

typedef enum {
	FAILURE = 0,
	SUCCESS = 1, // TODO: reemplazar por la convencion en C (success = 0, failure/error != 0)
	MEM_NOT_ALLOCATED = -2,
	MODULE_NOT_SET = -4,
	ADDRESS_NOT_FOUND = 0,
	OBJECT_NOT_FOUND = -1,
	EMPTY_MSG = -5
} e_generic_returns;

typedef enum {
	MASTER = 1,
	SLAVE = 0
} e_roles;

class BTHandler {
	public:
		BTHandler(HardwareSerial *);
		int get_average_reading_for_address(const char *);
		int get_inqm_max_mediciones();
		int get_inqm_timeout();
		int get_module_status();
		int set_inqm_max_mediciones(const int);
		void set_inqm_timeout(const int);
		int set_module(); // at_ready + at_init + at_role + at_inqm
		int at_ready();
		int at_init();
		int at_role(e_roles);
		int at_inqm();
		int at_inq();

	private:
		HardwareSerial * serial;
		t_medicion * mediciones;
		int inqm_max_mediciones;
		int inqm_timeout; // Max: 48s, El modulo usa timeout*1.28s para obtener el timeout real
		int module_status;
};

#endif // BTHANDLER_H
