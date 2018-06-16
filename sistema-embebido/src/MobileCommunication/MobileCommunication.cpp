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
}

void MobileCommunication::start() {
	digitalWrite(this->_btVccPin, LOW);
	delay(SMALL_DELAY);
	digitalWrite(this->_btEnablePin, HIGH);
	delay(SMALL_DELAY);
	digitalWrite(this->_btVccPin, HIGH);

	delay(1000);

	Serial3.print(RESTORE);
	this->_clearBuffer();
	Serial3.print(NAME);
	this->_clearBuffer();
	Serial3.print(CLASS);
	this->_clearBuffer();
	Serial3.print(ROLE);
	this->_clearBuffer();
	Serial3.print(BAUD_RATE);
	this->_clearBuffer();
	Serial3.print(INIT);
	this->_clearBuffer();

	delay(700);

	digitalWrite(this->_btEnablePin, LOW);
	digitalWrite(this->_btVccPin, LOW);
	delay(SMALL_DELAY);
	digitalWrite(this->_btVccPin, HIGH);
	delay(SMALL_DELAY);
}

void MobileCommunication::_clearBuffer() {
	delay(250);
	if(Serial3.available()) {
		Serial.print((String)Serial3.readString());
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
