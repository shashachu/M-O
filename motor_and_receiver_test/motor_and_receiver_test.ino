/**
 * Test for motor driven by an RC transmitter.
 */

#include <Wire.h>
#include <EnableInterrupt.h>

// Motor pin connections
const int ENA = A0;
const int IN1 = A1;
const int IN2 = A2;

#define RC_CH2  1

// Inputs from the RC receiver
#define RC_CH2_INPUT  8  // Forward/backwards

#define RC_NUM_CHANNELS  6
uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

float inch2;
float CH2;

#define INPUT_SIZE 30

int deadzonexy = 20;          // adust value to higher if motor not moving when stick moved

void setup() {
  pinMode(RC_CH2_INPUT, INPUT);

  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);
}

void loop() {
  rc_read_values();

  motordrive();

  inch2 = rc_values[RC_CH2]; // read and store channel value from receiver
  
  text();
}

int output1 = 0;
void motordrive() {
  CH2 = inch2;
  if (CH2 < 500 ) { // Failsafe Mode in case transmitter is turned off or loses signal
    output1 = 0;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, output1); // output the PWM to Motor driver pins
  }
  else {
    CH2 = constrain(CH2, 1000, 2000);
    CH2 = map(CH2, 1000, 2000, -255, 255);
    CH2 = constrain(CH2, -255, 255);

    if (CH2 < -deadzonexy) {
      output1 = abs(CH2);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    else if (CH2 > deadzonexy) {
      output1 = CH2;
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }
    else {
      output1 = 0;
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    }
    output1 = constrain(output1, 0, 255);
    analogWrite(ENA, output1); // output the PWM to Motor driver pins
  }
}

// Below functions are to read the analog inputs and store the values from the RC receiver
void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}
////////////////////////////////////////////////////////////////////////////////////////

void calc_ch2() {
  calc_input(RC_CH2, RC_CH2_INPUT);
}

void text() {
  Serial.print("inch2:"); Serial.print(inch2); Serial.print("\t"); // uncomment these inch values to see the raw data from the receiver
  Serial.print("CH2:"); Serial.print(CH2); Serial.print("\t");
  Serial.println("\t");
}
