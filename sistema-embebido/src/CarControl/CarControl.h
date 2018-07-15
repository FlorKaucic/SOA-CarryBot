/*
  CarControl.h - Library for controlling two DC motors

  Initialize your object indicating the output pin for:
  - Left Motor Clockwise Pin
  - Left Motor Counter Clockwise Pin
	- Right Motor Clockwise Pin
	- Right Motor Counter Clockwise Pin

  -- Example of CarControl library:
  #include "src/CarControl/CarControl.h"

  const int LEFT_MOTOR_CLOCKWISE_PIN = 30;
	const int LEFT_MOTOR_COUNTER_CLOCKWISE_PIN = 31;
	const int RIGHT_MOTOR_CLOCKWISE_PIN = 32;
	const int RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN = 33;

  -- During setup:
  CarControl carControl(LEFT_MOTOR_CLOCKWISE_PIN,
                        LEFT_MOTOR_COUNTER_CLOCKWISE_PIN,
                        RIGHT_MOTOR_CLOCKWISE_PIN,
                        RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN);

  -- Control the movement:
  carControl.goForward();
  carControl.goBackwards();
  carControl.turnLeft();
  carControl.turnRight();
  carControl.stop();
*/

#ifndef CARRYBOT_CARCONTROL_H
#define CARRYBOT_CARCONTROL_H

#include "Arduino.h"

class CarControl
{
	public:
		CarControl(const int leftMotorClockwisePin,
		           const int leftMotorCounterClockwisePin,
		           const int rightMotorClockwisePin,
		           const int rightMotorCounterClockwisePin);
		void goForward(void);
		void goBackwards(void);
		void turnLeft(void);
		void turnRight(void);
		void stop(void);
		void setMillisecondsForTurning(const int);

	private:
		int _leftMotorClockwisePin;
		int _leftMotorCounterClockwisePin;
		int _rightMotorClockwisePin;
		int _rightMotorCounterClockwisePin;
		int _millisecondsForTurning; 
		const static int DEFAULT_MILLISECONDS_FOR_TURNING = 600;
		void _goLeft(void);
		void _goRight(void);
};

#endif // CARRYBOT_CARCONTROL_H
