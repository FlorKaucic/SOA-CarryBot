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

class Alarm
{
	public:
		Alarm(int speakerPin);
		void alert(void);
                void beep(void);
                void error(void);

	private:
		int _speakerPin;
		TunePlayer _tunePlayer;
		const int ALERT_SIZE = 11;
		const int ALERT_NOTES[] = {
                  NOTE_G2, 0, NOTE_D2, NOTE_G2,
                  0, NOTE_D2, NOTE_G2, NOTE_D2,
                  NOTE_G2, NOTE_B2, NOTE_D3
		};
		const int ALERT_DURATIONS[] = {
		  4, 4, 8, 4, 8, 8, 8, 8, 8, 8, 4
		};
};

#endif
