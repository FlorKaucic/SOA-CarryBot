#include "src/ObstacleDetector/ObstacleDetector.h"
#include "src/CarControl/CarControl.h"
#include "src/LED/LED.h"

const int TRIGGER_PIN = 50;
const int ECHO_PIN = 51;

const int LEFT_MOTOR_CLOCKWISE_PIN = 30;
const int LEFT_MOTOR_COUNTER_CLOCKWISE_PIN = 31;
const int RIGHT_MOTOR_CLOCKWISE_PIN = 32;
const int RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN = 33;

const int LED_PIN = 9;

const int MILLISECONDS_FOR_TURNING = 570;

ObstacleDetector obstacleDetector(TRIGGER_PIN, ECHO_PIN);

LED led(LED_PIN);

CarControl carControl(LEFT_MOTOR_CLOCKWISE_PIN,
                      LEFT_MOTOR_COUNTER_CLOCKWISE_PIN,
                      RIGHT_MOTOR_CLOCKWISE_PIN,
                      RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN,
                      MILLISECONDS_FOR_TURNING);

void setup() {
  Serial.begin(9600);
}

void loop() {
  bool roadBlocked = obstacleDetector.isRoadBlocked();
  Serial.println(roadBlocked);
  delay(1000);
}
