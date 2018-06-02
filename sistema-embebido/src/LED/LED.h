/*
  LED.cpp - Library for controlling a LED

  -- Setup:
  #include "src/LED/LED.h"

  const int LED_PIN = 9; // PWM pin needed here

  LED led(LED_PIN);

  -- Usage:
  led.on(); // Turns on the led (without fading in)
	led.off(); // Turns off the led (without fading out)
	led.fadeIn(); // Turns on the led, fading in, with default delay time between increments
	led.fadeIn(fadingDelay); // Turns on the led, fading in, with custom delay time between increments
	led.fadeOut(); // Turns off the led, fading out, with default delay time between decrements
	led.fadeOut(fadingDelay); // Turns off the led, fading out, with custom delay time between decrements
	led.blink(); // Blinks the led once, fading in and fading out, with default delay time
	led.blinkNTimes(n); // Blinks the led n times, fading in and fading out, with default delay time
*/

#ifndef CARRYBOT_LED_H
#define CARRYBOT_LED_H

#include "Arduino.h"

class LED
{
	public:
		LED(int ledPin);
		void on(void);
		void off(void);
		void fadeIn(void);
		void fadeIn(int fadingDelay);
		void fadeOut(void);
		void fadeOut(int fadingDelay);
		void blink(void);
		void blinkNTimes(int n);

	private:
		int _ledPin;
		const int INTENSITY_MAX = 255;
		const int INTENSITY_MIN = 0;
		const int INTENSITY_LEAP = 5;
		const int DEFAULT_FADING_DELAY = 10;
		void _fadeIn(int fadingDelay);
		void _fadeOut(int fadingDelay);
};

#endif //CARRYBOT_LED_H
