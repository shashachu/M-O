#ifndef RCSERVO_H
#define RCSERVO_H

#define USE_PCA9685_SERVO_EXPANDER
// #define DEBUG_RCSERVO  // Uncomment to enable debug output

#include <Wire.h>
#include <ServoEasing.hpp>

/**
 * Class to encapsulate config/control of a servo controlled via RC. Created with the help of ChatGPT + Claude
 */

#define START_ANGLE  90

struct ServoConfig {
  uint8_t channel;
  uint8_t pcaPin;
  uint8_t inputPin;
  int minAngle;
  int maxAngle;
  uint8_t speed;
  uint8_t easing;
  // Smoothing parameters
  uint8_t filterSize;      // Moving average window size
  uint8_t deadband;        // Ignore changes smaller than this (degrees)
  float smoothingFactor;   // Exponential smoothing (0.1-0.5 typical)
};

class RCServo {
public:
  RCServo(ServoConfig config)
    : 
    config(config),
    lastAngle(-1),
    rawPulse(1500),
    smoothedPulse(1500.0),
    filterIndex(0),
    filterSum(0),
    isInitialized(false) {
      
    // Initialize moving average filter with neutral position
    for (int i = 0; i < MAX_FILTER_SIZE; i++) {
      pulseHistory[i] = 1500;
    }
    filterSum = 1500 * config.filterSize;
  }

  void attach() {
    servo.attach(config.pcaPin, START_ANGLE);
    pinMode(config.inputPin, INPUT);
    
    simDelay(200);

    // Update our tracking variables
    lastAngle = START_ANGLE;
    isInitialized = true;
  }

  void configureEasing(uint8_t speed = 300, uint8_t easing = EASE_LINEAR) {
    servo.setSpeed(speed);
    servo.setEasingType(easing);
  }

  void updateFromRaw() {
    // Don't process until properly initialized
    if (!isInitialized) {
      return;
    }
    
    // Apply smoothing to raw pulse
    uint16_t smoothedValue = applySmoothingFilters();
    
    // Convert to angle
    int value = constrain(smoothedValue, 1000, 2000);
    int angle = map(value, 1000, 2000, config.minAngle, config.maxAngle);
    
    // Apply deadband - only move if change is significant
    if (abs(angle - lastAngle) >= config.deadband) {
#ifdef DEBUG_RCSERVO
      Serial.print("Filtered angle: "); Serial.println(angle);
#endif
      servo.write(angle);
      lastAngle = angle;
    }
  }

  void setRawPulse(uint16_t pulse) {
    // Constrain input to valid RC range
    rawPulse = constrain(pulse, 1000, 2000);
#ifdef DEBUG_RCSERVO
    Serial.print("Raw pulse: "); Serial.println(rawPulse);
#endif
  }

  uint8_t getInputPin() const {
    return config.inputPin;
  }

  uint8_t getRCChannel() const {
    return config.channel;
  }

  // Get current smoothed values for debugging
  uint16_t getSmoothedPulse() const {
    return (uint16_t)smoothedPulse;
  }

  int getCurrentAngle() const {
    return lastAngle;
  }

private:
  static const uint8_t MAX_FILTER_SIZE = 10;
  
  ServoConfig config;
  int lastAngle;
  uint16_t rawPulse;
  bool isInitialized;
  
  // Smoothing variables
  float smoothedPulse;                    // Exponentially smoothed value
  uint16_t pulseHistory[MAX_FILTER_SIZE]; // Moving average buffer
  uint8_t filterIndex;                    // Current position in buffer
  uint32_t filterSum;                     // Sum for moving average
  
  ServoEasing servo = ServoEasing(0x40, &Wire);

  uint16_t applySmoothingFilters() {
    // 1. Moving Average Filter
    uint16_t movingAvg = applyMovingAverage(rawPulse);
    
    // 2. Exponential Smoothing
    smoothedPulse = (config.smoothingFactor * movingAvg) + 
                    ((1.0 - config.smoothingFactor) * smoothedPulse);
    
    return (uint16_t)smoothedPulse;
  }

  uint16_t applyMovingAverage(uint16_t newValue) {
    // Remove oldest value from sum
    filterSum -= pulseHistory[filterIndex];
    
    // Add new value
    pulseHistory[filterIndex] = newValue;
    filterSum += newValue;
    
    // Move to next position (circular buffer)
    filterIndex = (filterIndex + 1) % config.filterSize;
    
    // Return average
    return filterSum / config.filterSize;
  }
};

#endif // RCSERVO_H