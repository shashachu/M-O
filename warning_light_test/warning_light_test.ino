#include <ServoEasing.h>
#include <OneButton.h>
#include <Adafruit_NeoPixel.h>

#include "ServoEasing.hpp"

/**
 * Code for testing the warning light servo and LED strip with a push button with an Arduino Nano.
 *
 */
const int SWITCH_PIN = 2;
const int LED_PIN = 3;
const int SERVO1_PIN = 7;

OneButton button = OneButton(SWITCH_PIN, true, true);

#define NUM_LEDS 9
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

const int RAISED_ANGLE = 180;
const int LOWERED_ANGLE = 0;
const int SERVO_SPEED = 300; // degrees per second

int switchState = 0;
bool raised = false; // button state

ServoEasing Servo1;

void simDelay(long period){
  long delayMillis = millis() + period;
  while (millis() <= delayMillis)
  {
    int x = 0; // dummy variable, does nothing
  }
}

void setup() {
  Serial.begin(9600);              // Initialize serial communication

  // Attach the servo and start it lowered
  Servo1.attach(SERVO1_PIN, LOWERED_ANGLE);
  simDelay(500);
  Servo1.detach();
  strip.begin();
  turnOffSiren();
  button.attachClick(handleClick);
}

static void handleClick() {
  raised = !raised;
  // Since the servo is detached we can just attach it at the desired new angle.
  if (raised) {
    Servo1.attach(SERVO1_PIN, RAISED_ANGLE);
  } else {
    Servo1.attach(SERVO1_PIN, LOWERED_ANGLE);
    turnOffSiren();
  }
  simDelay(500);
  // Detach the servo so it doesn't idle/tick
  Servo1.detach();
}

void loop() {
  button.tick();
  if (raised) {
    runSirenEffect();
  }
}

void turnOffSiren() {
  strip.clear();  // Turn off all LEDs
  strip.show();   // Update the strip to reflect the cleared state
}

void runSirenEffect() {
  static int currentLED = 0; // Track the current LED in the "circle"
  strip.clear();             // Clear all LEDs to ensure only one is lit

  // Light up the current LED in red
  strip.setPixelColor(currentLED, strip.Color(255, 0, 0)); // Red color
  strip.show();

  // Move to the next LED
  currentLED = (currentLED + 1) % NUM_LEDS; // Loop back to the first LED after the last one
  delay(50);  // Adjust delay to control the speed of the siren effect
}
