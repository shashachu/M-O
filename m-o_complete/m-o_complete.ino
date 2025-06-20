/**
 * Sketch for controlling M-O.
 */

#include "RCServo.h"
#include "MOConfig.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <EnableInterrupt.h>

#define USE_PCA9685_SERVO_EXPANDER

#include "ServoEasing.hpp"

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);

RCServo steering(steeringConfig);
RCServo headTilt(headTiltConfig);
RCServo headTurn(headTurnConfig);

#define RC_NUM_CHANNELS  6
uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

float inch1, inch3, inch4;
float CH1, CH3, CH4;

RCServo* servos[] = { &steering, &headTilt, &headTurn };

void setup() {
  pwm1.begin();
  pwm1.setPWMFreq(50);
  Serial.begin(115200);
  Wire.begin();

  for (RCServo* s : servos) {
    s->attach();
    s->configureEasing();
  }

  enableInterrupt(servos[0]->getInputPin(), isr_ch1, CHANGE);
  enableInterrupt(servos[1]->getInputPin(), isr_ch3, CHANGE);
  enableInterrupt(servos[2]->getInputPin(), isr_ch4, CHANGE);
}

void loop() {
  rc_read_values();

  for (int i = 0; i < 3; i++) {
    servos[i]->setRawPulse(rc_values[servos[i]->getRCChannel()]);
    servos[i]->updateFromRaw();
  }

  //text();
  delay(10);
}

// Below functions are to read the analog inputs and store the values from the RC receiver
void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}
////////////////////////////////////////////////////////////////////////////////////////

void isr_ch1() { calc_input(0, steering.getInputPin()); }
//void isr_ch2() { calc_input(1, headTilt.getInputPin()); }
void isr_ch3() { calc_input(2, headTilt.getInputPin()); }
void isr_ch4() { calc_input(3, headTurn.getInputPin()); }
//void isr_ch5() { calc_input(4, headTurn.getInputPin()); }
//void isr_ch6() { calc_input(5, headTurn.getInputPin()); }

void calc_input(uint8_t channel, uint8_t pin) {
  if (digitalRead(pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    rc_shared[channel] = micros() - rc_start[channel];
  }
}

void text() {
  Serial.print("inch1:"); Serial.print(rc_shared[steeringConfig.channel]); Serial.print("\t"); // uncomment these inch values to see the raw data from the receiver
  Serial.print("inch3:"); Serial.print(rc_shared[headTiltConfig.channel]); Serial.print("\t");
  Serial.print("inch4:"); Serial.print(rc_shared[headTurnConfig.channel]); Serial.print("\t");
  Serial.println("\t");
}
