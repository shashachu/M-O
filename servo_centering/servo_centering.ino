/**
 * Centers the 3 servos for M-O.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define USE_PCA9685_SERVO_EXPANDER

#include "ServoEasing.hpp"

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
ServoEasing SteeringServo(0x40, &Wire);
ServoEasing HeadTurnServo(0x40, &Wire);
ServoEasing HeadTiltServo(0x40, &Wire);

const int Steering_PIN = 0;
const int HeadTurn_PIN = 1;
const int HeadTilt_PIN = 2;

#define START_DEGREE_VALUE  90

void setup() {
  pwm1.begin();
  pwm1.setPWMFreq(50);
  Serial.begin(115200);

  Wire.begin();
  
  SteeringServo.attach(Steering_PIN, START_DEGREE_VALUE);
  HeadTurnServo.attach(HeadTurn_PIN, START_DEGREE_VALUE);
  HeadTiltServo.attach(HeadTilt_PIN, START_DEGREE_VALUE);

  SteeringServo.setEasingType(EASE_SINE_IN_OUT);
  HeadTurnServo.setEasingType(EASE_SINE_IN_OUT);
  HeadTiltServo.setEasingType(EASE_SINE_IN_OUT);

  SteeringServo.setSpeed(100);
  HeadTurnServo.setSpeed(100);
  HeadTiltServo.setSpeed(100);

  SteeringServo.startEaseTo(90);
  HeadTurnServo.startEaseTo(90);
  HeadTiltServo.startEaseTo(90);
}

void loop() {
  // nothing
}
