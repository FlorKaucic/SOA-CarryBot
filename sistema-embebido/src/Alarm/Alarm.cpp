/*
  Alarm.cpp - Library that handles alarm behaviour

  Refer to Alarm.h for method signature and explanation
*/

#include "Alarm.h"

Alarm::Alarm(int speakerPin) {
	_speakerPin = speakerPin;
	_tunePlayer = new TunePlayer(_speakerPin);
}

void Alarm::arrival() {
	_tunePlayer->setTune(ARRIVAL_NOTES, ARRIVAL_DURATIONS, ARRIVAL_SIZE);
	_tunePlayer->playTune();
}

void Alarm::error() {
	_tunePlayer->setTune(ERROR_NOTES, ERROR_DURATIONS, ERROR_SIZE);
	_tunePlayer->playTune();
}

void Alarm::alert() {
	_tunePlayer->setTune(ALERT_NOTES, ALERT_DURATIONS, ALERT_SIZE);
	_tunePlayer->playTune();
}

void Alarm::connected() {
	_tunePlayer->setTune(CONNECTED_NOTES, CONNECTED_DURATIONS, CONNECTED_SIZE);
	_tunePlayer->playTune();
}

void Alarm::disconnected() {
	_tunePlayer->setTune(DISCONNECTED_NOTES, DISCONNECTED_DURATIONS, DISCONNECTED_SIZE);
	_tunePlayer->playTune();
}
