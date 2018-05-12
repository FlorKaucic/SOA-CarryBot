#include "src/ObstacleDetector/ObstacleDetector.h"

const int TRIGGER_PIN = 50;
const int ECHO_PIN = 51;

ObstacleDetector obstacleDetector(TRIGGER_PIN, ECHO_PIN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  bool roadBlocked = obstacleDetector.isRoadBlocked();
  Serial.println(roadBlocked);
  delay(1000);
}
