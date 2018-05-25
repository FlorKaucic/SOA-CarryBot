/*
  Alarm.cpp - Library that handles alarm behaviour

  Refer to Alarm.h for method signature and explanation
*/

#include "Arduino.h"
#include "TunePlayer.h"
#include "src/Speaker/TunePlayer.h"
#include "src/Speaker/Notes.h"

Alarm::Alarm(int speakerPin) {
	_speakerPin = speakerPin;
	_tunePlayer = new TunePlayer(_speakerPin);
}

void Alarm::alert() {
	_tunePlayer.setTune(ALERT_NOTES, ALERT_TIMES, ALERT_SIZE);
	_tunePlayer.playTune();
}
