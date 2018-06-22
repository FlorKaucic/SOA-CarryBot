#include "src/ObstacleDetector/ObstacleDetector.h"
#include "src/CarControl/CarControl.h"
#include "src/LED/LED.h"
#include "src/Alarm/Alarm.h"
#include "src/Bluetooth/BTHandler/BTHandler.h"
#include "src/SignalComparator/SignalComparator.h"

const int TRIGGER_PIN = 50;
const int ECHO_PIN = 51;
const int SPEAKER_PIN = 53;
const int LEFT_MOTOR_CLOCKWISE_PIN = 30;
const int LEFT_MOTOR_COUNTER_CLOCKWISE_PIN = 31;
const int RIGHT_MOTOR_CLOCKWISE_PIN = 32;
const int RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN = 33;
const int LED_PIN = 2;

const int MILLISECONDS_FOR_TURNING = 1300;

/*
Sebas = "305A:3A:11CB29";
TV Flor = "F877:B8:C3662F";
Flor = "E458:B8:90E129";
*/

char BLUETOOTH_DEVICE_ADDRESS[] = "E458:B8:90E129";
const double PROCESS_NOISE = 0.125;
const double SENSOR_NOISE = 32;
const double ESTIMATED_ERROR = 1023;

const int LOOPS_BETWEEN_INQUIRIES = 15;

bool roadBlocked;

ObstacleDetector obstacleDetector(TRIGGER_PIN, ECHO_PIN);
Alarm alarm(SPEAKER_PIN);
LED led(LED_PIN);
CarControl carControl(LEFT_MOTOR_CLOCKWISE_PIN,
                      LEFT_MOTOR_COUNTER_CLOCKWISE_PIN,
                      RIGHT_MOTOR_CLOCKWISE_PIN,
                      RIGHT_MOTOR_COUNTER_CLOCKWISE_PIN,
                      MILLISECONDS_FOR_TURNING);

SignalComparator signalComparator(&Serial2, BLUETOOTH_DEVICE_ADDRESS);

void setup() {
  Serial.begin(9600);
  Serial2.begin(38400);
  delay(LONG_DELAY);

  String ping = "";
  /*while(!ping.startsWith("PING")) {
    if(Serial3.available()) {
      ping = Serial3.readString();
      Serial.println(ping);
    }
  }
  */
  Serial.println("PONG");
  alarm.connected();

  Serial.println("Signal comparator init");
  signalComparator.init();
  Serial.println("Signal comparator set parameters");
  signalComparator.setFilterParameters(PROCESS_NOISE, SENSOR_NOISE, ESTIMATED_ERROR);
  signalComparator.setInitialReading();
}

void loop() {
  Serial.println("GO GO GO");
  carControl.goForward();
  int loopCount = 0;
  roadBlocked = obstacleDetector.isRoadBlocked();

  //Serial.print("Road blocked: ");
  //Serial.println(roadBlocked);
  //Serial.print("Loop count: ");
  //Serial.print(loopCount);
  //Serial.print("\n\r\n");

  while(!roadBlocked && loopCount < LOOPS_BETWEEN_INQUIRIES) {
    roadBlocked = obstacleDetector.isRoadBlocked();
    loopCount++;

    //Serial.print("Road blocked: ");
    //Serial.println(roadBlocked);
    //Serial.print("Loop count: ");
    //Serial.print(loopCount);
    //Serial.print("\n\r\n");

    delay(300);
  }

  Serial.println("STOP");
  carControl.stop();

  while(roadBlocked) {
    led.fadeIn();
    alarm.error();
    led.fadeOut();
    
    Serial.println("ERROR");
    
    delay(200);
    
    roadBlocked = obstacleDetector.isRoadBlocked();
  }


  Serial.println("Relative distance measuring: ");
  RelativeDistance distance = signalComparator.getRelativeDistance();

  Serial.print("Result: ");
  if(distance == RelativeDistance::FURTHER_AWAY_FROM_TARGET) {
    Serial.println("TIME TO TURN LEFT");
    alarm.alert();
    carControl.turnLeft();
  } else if (distance == RelativeDistance::ON_TARGET) {
    Serial.println("ON TARGET");
    while(true){
      led.fadeIn();
      alarm.arrival();
      led.fadeOut();
      delay(3000);
    }
  } else if (distance == RelativeDistance::ERROR) {
    alarm.disconnected();
    delay(2000);
  }
}
