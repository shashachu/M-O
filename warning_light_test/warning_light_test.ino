#include <ServoEasing.h>

#include "ServoEasing.hpp"

/**
 * Code for testing the warning light with a push button with an Arduino Uno.
 *
 * TODO: Add LED strip
 */

const int SWITCH_PIN = 2;
const int SERVO1_PIN = 7;

const int RAISED_ANGLE = 0;
const int LOWERED_ANGLE = 180;
const int SERVO_SPEED = 300; // degrees per second

int switchState = 0;
bool toggle = true; // button state

ServoEasing Servo1;

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Use internal pull-up resistor
  Serial.begin(9600);              // Initialize serial communication

  // Attach the servo and start it lowered
  Servo1.attach(SERVO1_PIN, LOWERED_ANGLE);
}

void loop() {
  // Read the state of the switch (LOW when pressed, HIGH when not pressed)
  switchState = digitalRead(SWITCH_PIN);

  if (switchState == LOW) { // Button is pressed
    Serial.println("Button Pressed");
    toggle = !toggle;
  } else {
    //Serial.println("Button Released");
  }

  if (toggle) {
    Servo1.setSpeed(SERVO_SPEED);
    Servo1.easeTo(RAISED_ANGLE);
  } else {
    Servo1.setSpeed(SERVO_SPEED);
    Servo1.easeTo(LOWERED_ANGLE);
  }

  delay(100); // Small delay for readability
}
