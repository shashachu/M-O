#ifndef WARNINGLIGHT_H
#define WARNINGLIGHT_H

#define USE_PCA9685_SERVO_EXPANDER

#include "Utils.h"
#include <Wire.h>
#include <ServoEasing.hpp>
#include <Adafruit_NeoPixel.h>

struct WarningLightConfig {
  uint8_t channel;
  uint8_t neoPixelPin;
  uint8_t numLEDs;  
  uint8_t servoPcaPin;
  uint8_t servoSpeed;
  uint8_t loweredAngle;
  uint8_t raisedAngle;  
};

class WarningLightController {
public:
  WarningLightController(WarningLightConfig config)
    : config(config), raised(false) {
      strip = Adafruit_NeoPixel(config.numLEDs, config.neoPixelPin, NEO_GRB + NEO_KHZ800);
    }

  void attach() {
    // Attach servo and immediately set to center of its configured range
    servo.attach(config.servoPcaPin, config.loweredAngle);
    simDelay(500);
    servo.detach();
    strip.begin();
  }

  void toggle() {
    raised = !raised;
    if (raised) {
      servo.attach(config.servoPcaPin, config.raisedAngle);
    } else {
      servo.attach(config.servoPcaPin, config.loweredAngle);
      turnOffSiren();
    }
    simDelay(500);
    // Detach the servo so it doesn't idle/tick
    servo.detach();
  }

private:
  void turnOffSiren() {
    strip.clear();  // Turn off all LEDs
    strip.show();   // Update the strip to reflect the cleared state
  }

  void runSirenEffect() {
    currentLED = 0;
    strip.clear();             // Clear all LEDs to ensure only one is lit

    // Light up the current LED in red
    strip.setPixelColor(currentLED, strip.Color(255, 0, 0)); // Red color
    strip.show();

    // Move to the next LED
    currentLED = (currentLED + 1) % config.numLEDs; // Loop back to the first LED after the last one
    simDelay(50);  // Adjust delay to control the speed of the siren effect
  }

  bool raised;
  uint8_t currentLED;
  WarningLightConfig config;
  ServoEasing servo = ServoEasing(0x40, &Wire);
  Adafruit_NeoPixel strip;
};

#endif // WARNINGLIGHT_H
