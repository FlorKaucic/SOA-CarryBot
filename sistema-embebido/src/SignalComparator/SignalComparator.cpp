/*
  SignalComparator.cpp - Library for comparing signal level to the previous reading

  Refer to SignalComparator.h for method signature and explanation
*/

#include "SignalComparator.h"

SignalComparator::SignalComparator(HardwareSerial *serial, char *address) {
	this->_numberOfReadings = DEFAULT_NUMBER_OF_READINGS;
	this->_readingTimeout = DEFAULT_READING_TIMEOUT;
	this->_address = address;
	this->_bluetooth = new BTHandler(serial);
};

SignalComparator::SignalComparator(HardwareSerial *serial, char *address, int numberOfReadings, int readingTimeout) {
	this->_address = address;
	this->_numberOfReadings = numberOfReadings;
	this->_readingTimeout = readingTimeout;
	this->_bluetooth = new BTHandler(serial);
};

void SignalComparator::setFilterParameters(double process_noise, double sensor_noise, double estimated_error) {
	this->_bluetooth->at_inq();
	this->_filter = new Kalman(process_noise, sensor_noise, estimated_error,INITIAL_NOISE_LEVEL);
}

int SignalComparator::setInitialReading() {
	this->_previousNoiseLevel = this->_bluetooth->get_average_reading_for_address(this->_address);
	this->_previousNoiseLevel = this->_filter->getFilteredValue(this->_previousNoiseLevel);
	return this->_previousNoiseLevel;
}

void SignalComparator::init() {
	this->_bluetooth->at_ready();
	this->_bluetooth->at_role(e_roles::MASTER);
	this->_bluetooth->set_inqm_max_mediciones(9);
	this->_bluetooth->set_inqm_timeout(48);
	this->_bluetooth->at_inqm();
	this->_bluetooth->at_init();
	delay(SMALL_DELAY);
}

int SignalComparator::_getFilteredNoiseLevel() {
	Serial.println("Getting noise level...");
	this->_bluetooth->at_inq();
	int noiseLevel;
	Serial.println("Filtering noise level...");
	noiseLevel = this->_bluetooth->get_average_reading_for_address(this->_address);
	if(noiseLevel == e_generic_returns::ADDRESS_NOT_FOUND) {
		return ADDRESS_NOT_FOUND;
	}
	Serial.println("Kalmaning the fuck away from noise level...");
	noiseLevel = this->_filter->getFilteredValue(noiseLevel);
	return noiseLevel;
}


RelativeDistance SignalComparator::getRelativeDistance() {
	int noiseLevel = this->_getFilteredNoiseLevel();

	Serial.print("Noise Level: ");
	Serial.println(noiseLevel);

	if (noiseLevel == e_generic_returns::ADDRESS_NOT_FOUND) {
		return RelativeDistance::ERROR;
	}

	if (noiseLevel > MAXIMUM_NOISE_ON_TARGET) {
		return RelativeDistance::ON_TARGET;
	}

	if (noiseLevel < (this->_previousNoiseLevel + DEFAULT_THRESHOLD)) {
		this->_previousNoiseLevel = noiseLevel;
		return RelativeDistance::FURTHER_AWAY_FROM_TARGET;
	}

	if (noiseLevel > (this->_previousNoiseLevel - DEFAULT_THRESHOLD)) {
		this->_previousNoiseLevel = noiseLevel;
	}

	return RelativeDistance::NEARER_TO_TARGET;
}
