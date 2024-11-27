/**
 * Standalone test for PCA9685 servo expander board. Drives two servos back and forth.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define USE_PCA9685_SERVO_EXPANDER

#include "ServoEasing.hpp"

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
ServoEasing SteeringServo(0x40, &Wire);
ServoEasing HeadTurnServo(0x40, &Wire);

const int Steering_PIN = 0;
const int HeadTurn_PIN = 1;

#define START_DEGREE_VALUE  90

int steeringmin = 15;
int steeringmax = 165;
int headturnmin = 30;
int headturnmax = 150;

void setup() {
  pwm1.begin();
  pwm1.setPWMFreq(50);
  Serial.begin(115200);
  Wire.begin();
  SteeringServo.attach(Steering_PIN, START_DEGREE_VALUE);
  HeadTurnServo.attach(HeadTurn_PIN, START_DEGREE_VALUE);
  SteeringServo.setEasingType(EASE_SINE_IN_OUT);
  HeadTurnServo.setEasingType(EASE_SINE_IN_OUT);
  SteeringServo.setSpeed(100);
  HeadTurnServo.setSpeed(100);
  delay(1000);
}

static int count = 0;
void loop() {
  if (count % 2 == 0) {
    Serial.println("easing to min");
    SteeringServo.startEaseTo(steeringmin);
    HeadTurnServo.startEaseTo(headturnmin);
  } else {
    Serial.println("easing to max");
    SteeringServo.startEaseTo(steeringmax);
    HeadTurnServo.startEaseTo(headturnmax);
  }
  count++;
  delay(2000);
}
