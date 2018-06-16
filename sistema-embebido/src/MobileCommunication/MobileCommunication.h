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
		void start(void);
		void send(String message);
		void send(int message);
		void send(bool message);
		void send(char message);
		const static int CONFIG_BAUD_RATE = 38400;
		const static int COMM_BAUD_RATE = 9600;

	private:
		int _btVccPin;
		int _btEnablePin;
		void _clearBuffer(void);
		const static int SMALL_DELAY = 200;
		const String INIT = "AT+INIT\r\n";
		const String ROLE = "AT+ROLE=0\r\n";
		const String CLASS = "AT+CLASS=0\r\n";
		const String BAUD_RATE = "AT+UART=9600,1,2\r\n";
		const String RESTORE = "AT+ORGL\r\n";
		const String NAME = "AT+NAME=PEPEBOT\r\n";

};
#endif //CARRYBOT_MOBILECOMMUNICATION_H
