/**
 * Test for mapping the VRB knob to M-O's facial expressions
 */

#include <Wire.h>
#include <EnableInterrupt.h>

// Motor pin connections
const int ENA = A0;
const int IN1 = A1;
const int IN2 = A2;

#define RC_CH6  5

// Inputs from the RC receiver
#define RC_CH6_INPUT  12  // Expressions

#define RC_NUM_CHANNELS  6
uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

float inch6;
float CH6;

#define INPUT_SIZE 30

int curExpression = -1;
int newExpression;
int expressionMin = 1;
int expressionMax = 25;

void setup() {
  Serial.begin(115200);
  pinMode(RC_CH6_INPUT, INPUT);

  enableInterrupt(RC_CH6_INPUT, calc_ch6, CHANGE);
}

void loop() {
  rc_read_values();

  expressions();

  inch6 = rc_values[RC_CH6]; // read and store channel value from receiver
  
  text();
}

void expressions() {
  CH6 = inch6;
  CH6 = constrain(CH6, 1000, 2000);
  // There's a little jitter at the top end of the knob
  if (CH6 >= 1996) {
    CH6 = 2000;
  }
  newExpression = map(CH6, 1000, 2000, expressionMin, expressionMax);
  if (newExpression != curExpression) {
    curExpression = newExpression;
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

void calc_ch6() {
  calc_input(RC_CH6, RC_CH6_INPUT);
}

void text() {
  Serial.print("inch6:"); Serial.print(inch6); Serial.print("\t"); // uncomment these inch values to see the raw data from the receiver
  Serial.print("CH6:"); Serial.print(CH6); Serial.print("\t");
  Serial.print("curExpression: "); Serial.print(curExpression); Serial.print("\t");
  Serial.println("\t");
}
