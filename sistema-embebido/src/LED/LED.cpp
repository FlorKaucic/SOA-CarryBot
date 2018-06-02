/*
  LED.cpp - Library for controlling a LED

  Refer to LED.h for method signature and explanation
*/

#include "LED.h"

LED::LED(int ledPin) {
	_ledPin = ledPin;

	pinMode(_ledPin, OUTPUT);
}

void LED::on() {
	analogWrite(_ledPin, INTENSITY_MAX);
}

void LED::off() {
	analogWrite(_ledPin, INTENSITY_MIN);
}

void LED::_fadeIn(int fadingDelay) {
	for (int fadeValue = INTENSITY_MIN ; fadeValue <= INTENSITY_MAX; fadeValue += INTENSITY_LEAP) {
		analogWrite(_ledPin, fadeValue);
		delay(fadingDelay);
	}
}

void LED::fadeIn() {
	_fadeIn(DEFAULT_FADING_DELAY);
}

void LED::fadeIn(int fadingDelay) {
	_fadeIn(fadingDelay);
}

void LED::_fadeOut(int fadingDelay) {
	for (int fadeValue = INTENSITY_MAX ; fadeValue >= INTENSITY_MIN; fadeValue -= INTENSITY_LEAP) {
		analogWrite(_ledPin, fadeValue);
		delay(fadingDelay);
	}
}

void LED::fadeOut() {
	_fadeOut(DEFAULT_FADING_DELAY);
}

void LED::fadeOut(int fadingDelay) {
	_fadeOut(fadingDelay);
}

void LED::blink() {
	_fadeIn(DEFAULT_FADING_DELAY);
	_fadeOut(DEFAULT_FADING_DELAY);
}

void LED::blinkNTimes(int n) {
	for(int i = 0; i < n; i++) {
		blink();
	}
}
