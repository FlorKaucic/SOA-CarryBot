#include "src/MobileCommunication/MobileCommunication.h"
#include "src/ObstacleDetector/ObstacleDetector.h"
#include "src/CarControl/CarControl.h"
#include "src/LED/LED.h"

const int TRIGGER_PIN = 50;
const int ECHO_PIN = 51;

const int LEFT_MOTOR_CLOCKWISE_PIN = 30;
const int LEFT_MOTOR_COUNTER_CLOCKWISE_PIN = 31;
const int RIGHT_MOTOR_CLOCKWISE_PIN = 32;
const int RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN = 33;

// Pins used for the bluetooth module that communicates with an Android app
const int BT_COMM_VCC_PIN = 22;
const int BT_COMM_ENABLE_PIN = 23;

// Pins used for the bluetooth module that gets rssi signal from device
const int BT_RSSI_VCC_PIN = 24;
const int BT_RSSI_ENABLE_PIN = 25;

const int LED_PIN = 9;

const int MILLISECONDS_FOR_TURNING = 570;

MobileCommunication mobile(BT_COMM_VCC_PIN, BT_COMM_ENABLE_PIN);

ObstacleDetector obstacleDetector(TRIGGER_PIN, ECHO_PIN);

CarControl carControl(LEFT_MOTOR_CLOCKWISE_PIN,
                      LEFT_MOTOR_COUNTER_CLOCKWISE_PIN,
                      RIGHT_MOTOR_CLOCKWISE_PIN,
                      RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN,
                      MILLISECONDS_FOR_TURNING);

LED led(LED_PIN);

const int TERMINAL_BAUD_RATE = 9600;
const int COMM_BAUD_RATE = 9600;
const int CONFIG_BAUD_RATE = 9600;

void setup() {
  Serial.begin(TERMINAL_BAUD_RATE);
  Serial3.begin(CONFIG_BAUD_RATE);
  mobile.start();
  Serial3.begin(COMM_BAUD_RATE);
  String message = "";
  while(message != "CONNECTED\r\n") {
    if(Serial3.available()) {
      message = Serial3.readString();
    }
  }
}

void loop() {
  bool roadBlocked = obstacleDetector.isRoadBlocked();
  Serial.println(roadBlocked);
  mobile.send(roadBlocked);
  delay(1000);
}
