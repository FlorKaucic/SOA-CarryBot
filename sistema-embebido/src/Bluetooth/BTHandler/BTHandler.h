#ifndef BTHANDLER_H
#define BTHANDLER_H

#include <string.h>
#include "arduino.h"
#include "../BTParser/BTParser.h"


using namespace std;

typedef enum{
	
			COMM_TIMEOUT 	= 30, 	// ms
			COMM_DELAY		= 30	// ms
			
} e_def_values;

typedef enum{
			AT_GOES_WRONG		= -1,
			MODULE_SET_PENDING	= 0,
			AT_READY 			= 1,
			AT_INITIALIZED 		= 2,
			AT_ROLE_SET			= 3,
			AT_INQM_SET			= 4			
			
} e_hc05_status;

typedef enum{
			
			FAILURE				= 0,
			SUCCESS				= 1,
			MEM_NOT_ALLOCATED 	= -2,
			MODULE_NOT_SET		= -4
			
} e_generic_return;


class BTHandler
{
    public:
		
        BTHandler(int* , const int, const int , const int );
		
		int		get_inqm_max_mediciones();
		int		set_inqm_max_mediciones(const int); // CUIDADO AL USARLO
		int 	get_inqm_timeout();
		void 	set_inqm_timeout(const int );
		
		int 	at_ready(const int ); // AT
		int		at_init(const int ); // AT+INIT
		int		at_role(const int , const int); // AT+ROLE
		int		at_inqm(const int ); // AT+INQM
		
		int		get_module_status(); 
		int		set_module(); // Prepara el modulo para realizar mediciones.
        int 	at_inq(); //AT+INQ
		
		//int 	find_address(const char* );
        
		virtual ~BTHandler();
		
		
    private:
		
		static int	check_timeout(const int timeout);
		static void prepare_serial_comm(const int timeout);
		
		t_medicion*   	mediciones;
		
		int				baudrate;
		int				role;
		int      		inqm_max_mediciones;
		int				inqm_timeout; //NOTA: MAX 48s, El modulo usa timeout*1.28s para obtener el timeout real
		
		int				module_status;
        

};

#endif // BTHANDLER_H
