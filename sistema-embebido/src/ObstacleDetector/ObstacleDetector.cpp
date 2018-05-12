/*
  ObstacleDetector.cpp - Library for simplifying obstacle detection
  
  Refer to ObstacleDetector.h for method signature and explanation
*/

#include "Arduino.h"
#include "ObstacleDetector.h"

ObstacleDetector::ObstacleDetector(int triggerPin, int echoPin) {
  _init(triggerPin, echoPin, DEFAULT_TIMEOUT);
}

ObstacleDetector::ObstacleDetector(int triggerPin, int echoPin, long timeout) {
  _init(triggerPin, echoPin, timeout);
}

void ObstacleDetector::_init(int triggerPin, int echoPin, long timeout) {
  _triggerPin = triggerPin;
  _echoPin = echoPin;
  _timeout = timeout;
  
  pinMode(_triggerPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

bool ObstacleDetector::isRoadBlocked() {
  long distanceInCm = _getDistance();

  return distanceInCm != 0;
}

long ObstacleDetector::_getDistance() {
  long durationInMicroseconds;
  
  digitalWrite(_triggerPin, LOW);
  delayMicroseconds(DEFAULT_REGULAR_DELAY);
  digitalWrite(_triggerPin, HIGH);
  delayMicroseconds(DEFAULT_PULSE_LENGTH);
  digitalWrite(_triggerPin, LOW);

  durationInMicroseconds = pulseIn(_echoPin, HIGH, _timeout);

  return _convertToCentimeters(durationInMicroseconds);
}

long ObstacleDetector::_convertToCentimeters(long microseconds) {
  return microseconds / MICROSECONDS_PER_CENTIMETER / NUMBER_OF_TIMES_TRAVELLING_THE_DISTANCE;
}

