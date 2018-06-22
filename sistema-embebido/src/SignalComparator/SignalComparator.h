/*
  SignalComparator.cpp - Library for comparing signal level to the previous reading


*/

#ifndef SISTEMA_EMBEBIDO_SIGNALCOMPARATOR_H
#define SISTEMA_EMBEBIDO_SIGNALCOMPARATOR_H

#include "Arduino.h"
#include "../Bluetooth/BTHandler/BTHandler.h"
#include "../KalmanFilter/Kalman.h"

enum RelativeDistance {
	NEARER_TO_TARGET,
	FURTHER_AWAY_FROM_TARGET,
	ON_TARGET,
	ERROR
};

const int SMALL_DELAY = 200;

class SignalComparator
{
	public:
		SignalComparator(HardwareSerial * serial, char * address);
		SignalComparator(HardwareSerial * serial, char * address, int numberOfReadings, int readingTimeout);
		RelativeDistance getRelativeDistance(void);
		void setFilterParameters(double process_noise, double sensor_noise, double estimated_error);
		int setInitialReading(void);
		void init(void);

	private:
		const int DEFAULT_NUMBER_OF_READINGS = 5;
		const int DEFAULT_READING_TIMEOUT = 48;
		const int DEFAULT_THRESHOLD = -3;
		const int INITIAL_NOISE_LEVEL = 0;
		const int MAXIMUM_NOISE_ON_TARGET = -50;
		BTHandler * _bluetooth;
		Kalman * _filter;
		char * _address;
		int _previousNoiseLevel;
		int _readingTimeout;
		int _numberOfReadings;
		int _memoryFailureError;
		int _getFilteredNoiseLevel(void);
};


#endif //SISTEMA_EMBEBIDO_SIGNALCOMPARATOR_H
