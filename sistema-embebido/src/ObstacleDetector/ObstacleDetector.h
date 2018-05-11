/*
  ObstacleDetector.h - Library for simplifying obstacle detection using an ultrasonic sensor (HC-SR04)

  Initialize your object indicating the triggerPin and echoPin number.
  Also, you can indicate the timeout in microseconds to change how far it would look for obstacles, but that's optional. 
  Default value is around 20 cm (2000 ms).
  
  Then, all you have to do is called "isRoadBlocked()" whenever you want to know if there's an obstacle in front of your ultrasonic sensor.
  The result would be a boolean value: true if there's indeed an obstacle ahead, false otherwise.
  
  Disclaimer:
  This library only works for HC-SR04, or any model that has two one-way communication pins.
*/

#ifndef ObstacleDetector_h
#define ObstacleDetector_h

class ObstacleDetector
{
  public:
    ObstacleDetector(int triggerPin, int echoPin);
    ObstacleDetector(int triggerPin, int echoPin, long timeout);
    bool isRoadBlocked(void);

  private:
    int _triggerPin;
    int _echoPin;
    int _timeout;
    long _getDistance(void);
    long _convertToCentimeters(long microseconds);
    void _init(int triggerPin, int echoPin, long timeout);
    const int DEFAULT_REGULAR_DELAY = 2;
    const int DEFAULT_PULSE_LENGTH = 10;
    const long DEFAULT_TIMEOUT = 2000;
    const int MICROSECONDS_PER_CENTIMETER = 29;
    const int NUMBER_OF_TIMES_TRAVELLING_THE_DISTANCE = 2;
};

#endif
