/*
  Alarm.cpp - Library that handles alarm behaviour
  
  Initialization: 
  Alarm myAlarm(pin); 

  
  Use alert() to make an alert when the device reaches its destiny
  
  Use beep() to make a notification beep sound. 
  * Intended to be use to alert changes or when there is a new connection via bluetooth.
  
  Use error() to make an error noise.
  * Intended to be one sound only, and be repeated until the obstacle in front is removed. 
*/

#ifndef Alarm_h
#define Alarm_h

#include "Arduino.h"
#include "../Speaker/TunePlayer.h"
#include "../Speaker/Notes.h"

class Alarm
{
	public:
		Alarm(int speakerPin);
		void arrival(void);
		void error(void);
		void alert(void);
		void connected(void);
		void disconnected(void);

	private:
		int _speakerPin;
		TunePlayer * _tunePlayer;
		const int ARRIVAL_SIZE = 5;
		const int ARRIVAL_NOTES[5] = {NOTE_F4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4};
		const int ARRIVAL_DURATIONS[5] = {4, 8, 8, 4, 4};
		const int ERROR_SIZE = 2;
		const int ERROR_NOTES[2] = {NOTE_E6, NOTE_E6};
		const int ERROR_DURATIONS[2] = {2, 1};
		const int ALERT_SIZE = 2;
		const int ALERT_NOTES[2] = {NOTE_E5, NOTE_A4};
		const int ALERT_DURATIONS[2] = {4, 4};
		const int CONNECTED_SIZE = 3;
		const int CONNECTED_NOTES[3] = {NOTE_A5, NOTE_C6, NOTE_E6};
		const int CONNECTED_DURATIONS[3] = {16, 16, 16};
		const int DISCONNECTED_SIZE = 3;
		const int DISCONNECTED_NOTES[3] = {NOTE_E6, NOTE_C6, NOTE_A5};
		const int DISCONNECTED_DURATIONS[3] = {16, 16, 16};
};

#endif
