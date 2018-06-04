/*
  MobileCommunication.h - Library for communicating with a mobile device via bluetooth (using Arduino Mega 2560 Serial3)

  Initialize indicating vcc pin and enable pin (pin that connects to PIO11 on module).
  Use send(message) to send a String message to the mobile device.
  You can send a char (not array), int, bool or String value.

  -- Example of MobileCommunication library:
  #include "src/MobileCommunication/MobileCommunication.h"

  -- During setup:
  const int BT_ENABLE_PIN = 52;
  const int BT_VCC_PIN = 53;

  MobileCommunication mobile(BT_VCC_PIN,  BT_ENABLE_PIN);

  -- Send message:
  mobile.send("My message");
*/

#ifndef CARRYBOT_MOBILECOMMUNICATION_H
#define CARRYBOT_MOBILECOMMUNICATION_H

#include "Arduino.h"

class MobileCommunication
{
	public:
		MobileCommunication(const int btVccPin,  const int btEnablePin);
		void send(String message);
		void send(int message);
		void send(bool message);
		void send(char message);

	private:
		int _btVccPin;
		int _btEnablePin;
		void _start(void);
		void _clearBuffer(void);
		const static int CONFIG_BAUD_RATE = 38400;
		const static int COMM_BAUD_RATE = 9600;
		const static int MICRODELAY = 20;
		const char INIT[9] = {'A', 'T', '+', 'I', 'N', 'I', 'T', '\r', '\n'};
		const char ROLE[11] = {'A', 'T', '+', 'R', 'O', 'L', 'E', '=', '0', '\r', '\n'};
		const char CLASS[12] = {'A', 'T', '+', 'C', 'L', 'A', 'S', 'S', '=', '0', '\r', '\n'};
		const char NAME[23] = {'A', 'T', '+', 'N', 'A', 'M', 'E', '=', 'C', 'A', 'R', 'R', 'Y', 'B', 'O', 'T', '-', 'C', 'O', 'M', 'M', '\r', '\n'};
		const char BAUD_RATE[18] = {'A', 'T', '+', 'U', 'A', 'R', 'T', '=', '9', '6', '0', '0', ',', '1', ',', '2', '\r', '\n'};
		const char RESTORE[9] = {'A', 'T', '+', 'O', 'R', 'G', 'L', '\r', '\n'};

};
#endif //CARRYBOT_MOBILECOMMUNICATION_H
