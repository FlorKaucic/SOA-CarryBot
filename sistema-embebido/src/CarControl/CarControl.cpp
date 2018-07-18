/*
  CarControl.h - Library for controlling two DC motors

  Refer to CarControl.h for method signature and explanation
*/

#include "CarControl.h"

CarControl::CarControl(const int leftMotorClockwisePin,
                       const int leftMotorCounterClockwisePin,
                       const int rightMotorClockwisePin,
                       const int rightMotorCounterClockwisePin) {
	this->_leftMotorClockwisePin = leftMotorClockwisePin;
	this->_leftMotorCounterClockwisePin = leftMotorCounterClockwisePin;
	this->_rightMotorClockwisePin = rightMotorClockwisePin;
	this->_rightMotorCounterClockwisePin = rightMotorCounterClockwisePin;
	this->_millisecondsForTurning = DEFAULT_MILLISECONDS_FOR_TURNING;

	pinMode(this->_leftMotorClockwisePin, OUTPUT);
	pinMode(this->_leftMotorCounterClockwisePin, OUTPUT);
	pinMode(this->_rightMotorClockwisePin, OUTPUT);
	pinMode(this->_rightMotorCounterClockwisePin, OUTPUT);
}

void CarControl::setMillisecondsForTurning(const int time){
	this->_millisecondsForTurning = time;
}

void CarControl::goForward(void) {
	digitalWrite(this->_leftMotorCounterClockwisePin, HIGH);
	digitalWrite(this->_rightMotorClockwisePin, HIGH);
}

void CarControl::goBackwards(void) {
	digitalWrite(this->_leftMotorClockwisePin, HIGH);
	digitalWrite(this->_rightMotorCounterClockwisePin, HIGH);
}

void CarControl::turnLeft(void) {
	this->_goLeft();
	delay(this->_millisecondsForTurning);
	this->stop();
}

void CarControl::turnRight(void) {
	this->_goRight();
	delay(this->_millisecondsForTurning);
	this->stop();
}

void CarControl::stop(void) {
	digitalWrite(this->_leftMotorClockwisePin, LOW);
	digitalWrite(this->_leftMotorCounterClockwisePin, LOW);
	digitalWrite(this->_rightMotorClockwisePin, LOW);
	digitalWrite(this->_rightMotorCounterClockwisePin, LOW);
}

void CarControl::_goRight(void) {
	digitalWrite(this->_leftMotorClockwisePin, HIGH);
	digitalWrite(this->_rightMotorClockwisePin, HIGH);
}

void CarControl::_goLeft(void) {
	digitalWrite(this->_leftMotorCounterClockwisePin, HIGH);
	digitalWrite(this->_rightMotorCounterClockwisePin, HIGH);
}
