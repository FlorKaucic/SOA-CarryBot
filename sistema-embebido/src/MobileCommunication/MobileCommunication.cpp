/*
  MobileCommunication.h - Library for communicating with a mobile device via bluetooth (using Arduino Mega 2560 Serial3)

  Refer to MobileCommunication.h for method signature and explanation
*/

#include "MobileCommunication.h"

MobileCommunication::MobileCommunication(const int btVccPin, const int btEnablePin) {
	this->_btVccPin = btVccPin;
	this->_btEnablePin = btEnablePin;

	pinMode(this->_btVccPin, OUTPUT);
	pinMode(this->_btEnablePin, OUTPUT);

	this->_start();
}

void MobileCommunication::_start() {
	digitalWrite(this->_btVccPin, LOW);
	delay(MICRODELAY);
	digitalWrite(this->_btEnablePin, HIGH);
	digitalWrite(this->_btVccPin, HIGH);
	delay(MICRODELAY);
	digitalWrite(this->_btEnablePin, LOW);

	Serial3.begin(CONFIG_BAUD_RATE);
	Serial3.write(RESTORE);
	this->_clearBuffer();
	Serial3.write(NAME);
	this->_clearBuffer();
	Serial3.write(CLASS);
	this->_clearBuffer();
	Serial3.write(ROLE);
	this->_clearBuffer();
	Serial3.write(BAUD_RATE);
	this->_clearBuffer();
	Serial3.write(INIT);
	this->_clearBuffer();

	digitalWrite(this->_btVccPin, LOW);
	delay(MICRODELAY);
	digitalWrite(this->_btVccPin, HIGH);
	delay(MICRODELAY);

	Serial3.begin(COMM_BAUD_RATE);
}

void MobileCommunication::_clearBuffer() {
	while(Serial3.available()) {
		Serial3.read();
	}
}

void MobileCommunication::send(String message){
	Serial3.println(message);
}

void MobileCommunication::send(int message){
	Serial3.println(message);
}

void MobileCommunication::send(bool message){
	Serial3.println(message);
}

void MobileCommunication::send(char message){
	Serial3.println(message);
}