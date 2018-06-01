#include "src/ObstacleDetector/ObstacleDetector.h"
#include "src/LED/LED.h"

const int TRIGGER_PIN = 50;
const int ECHO_PIN = 51;

const int LED_PIN = 9;

ObstacleDetector obstacleDetector(TRIGGER_PIN, ECHO_PIN);
LED led(LED_PIN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  bool roadBlocked = obstacleDetector.isRoadBlocked();
  Serial.println(roadBlocked);
  delay(1000);
}
