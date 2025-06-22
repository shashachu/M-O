#ifndef WARNINGLIGHT_H
#define WARNINGLIGHT_H

#define USE_PCA9685_SERVO_EXPANDER
// #define DEBUG_WARNING_LIGHT  // Uncomment to enable debug output

#include "Utils.h"
#include "RCInputDevice.h"
#include <Wire.h>
#include <ServoEasing.hpp>
#include <Adafruit_NeoPixel.h>

struct WarningLightConfig {
  // The RC channel that will control this warning light. 1-based
  uint8_t channel;
  // The pin on the Arduino that this channel is connected to
  uint8_t inputPin;
  // The pin on the Arduino the NeoPixel strip is connected to
  uint8_t neoPixelPin;
  // The number of LEDs in the NeoPixel trip
  uint8_t numLEDs;  
  // The pin this servo is attached to on the PCA expander board. 0-based.
  uint8_t servoPcaPin;
  // The speed the servo should move at
  uint8_t servoSpeed;
  // The servo angle for the lowered position
  uint8_t loweredAngle;
  // The servo angle for the raised position
  uint8_t raisedAngle;  
};

class WarningLightController : public RCInputDevice {
public:
  WarningLightController(WarningLightConfig config)
    : config(config), raised(false) {
      strip = Adafruit_NeoPixel(config.numLEDs, config.neoPixelPin, NEO_GRB + NEO_KHZ800);
    }

  void attach() override {
    // Attach servo and set to lowered position
    servo.attach(config.servoPcaPin, config.loweredAngle);
    simDelay(500);
    servo.detach();
    strip.begin();
  }

  void updateFromRC(uint16_t pulse) override {
#ifdef DEBUG_WARNING_LIGHT
    Serial.print("Raw pulse: "); Serial.println(pulse);
#endif
    bool shouldRaise = pulse > 1500;
#ifdef DEBUG_WARNING_LIGHT
    Serial.print("shouldRaise: "); Serial.print(shouldRaise); Serial.print(" raised: "); Serial.println(raised);
#endif
    if (raised != shouldRaise) {
      raised = shouldRaise;
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
  }

  void loop() {
    if (raised) {
      runSirenEffect();
    }
  }

  uint8_t getInputPin() const override{
    return config.inputPin;
  }

  uint8_t getRCChannel() const override {
    return config.channel - 1;
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
