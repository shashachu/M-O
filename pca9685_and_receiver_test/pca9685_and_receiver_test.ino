/**
 * Test for PCA9685 servo expander board and 2 servos driven by an RC transmitter.
 *
 * Note: I am experiencing a little jitter on channel 1 (steering) so will likely implement a deadzone in the
 * final code.
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <EnableInterrupt.h>

#define USE_PCA9685_SERVO_EXPANDER

#include "ServoEasing.hpp"

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
ServoEasing SteeringServo(0x40, &Wire);
ServoEasing HeadTiltServo(0x40, &Wire);
ServoEasing HeadTurnServo(0x40, &Wire);

const int Steering_PIN = 0;
const int HeadTilt_PIN = 1;
const int HeadTurn_PIN = 2;

#define RC_CH1  0 // Steering
#define RC_CH3  2 // Head tilt
#define RC_CH4  3 // Head turn

// Inputs from the RC receiver
#define RC_CH1_INPUT  7 // Left/Right steering
#define RC_CH3_INPUT  9 // Head tilt
#define RC_CH4_INPUT  10 // Head turn

#define START_DEGREE_VALUE  90

int steeringmin = 15;
int steeringmax = 165;
int headtiltmin = 60;
int headtiltmax = 115;
int headturnmin = 15;
int headturnmax = 165;

#define RC_NUM_CHANNELS  6
uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

int ServoSteeringPos, ServoTiltHeadPos, ServoTurnHeadPos;
float inch1, inch3, inch4;
float CH1, CH3, CH4;

#define INPUT_SIZE 30

void setup() {
  pwm1.begin();
  pwm1.setPWMFreq(50);
  Serial.begin(115200);
  Wire.begin();
  SteeringServo.attach(Steering_PIN, START_DEGREE_VALUE);
  HeadTiltServo.attach(HeadTilt_PIN, START_DEGREE_VALUE);
  HeadTurnServo.attach(HeadTurn_PIN, START_DEGREE_VALUE);
  SteeringServo.setEasingType(EASE_LINEAR);
  HeadTiltServo.setEasingType(EASE_LINEAR);
  HeadTurnServo.setEasingType(EASE_LINEAR);
  SteeringServo.setSpeed(300);
  HeadTiltServo.setSpeed(300);
  HeadTurnServo.setSpeed(300);
  delay(1000);

  pinMode(RC_CH1_INPUT, INPUT);
  pinMode(RC_CH3_INPUT, INPUT);
  pinMode(RC_CH4_INPUT, INPUT);

  enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);
  enableInterrupt(RC_CH3_INPUT, calc_ch3, CHANGE);
  enableInterrupt(RC_CH4_INPUT, calc_ch4, CHANGE);
}

void loop() {
  rc_read_values();

  steering();
  headtilt();
  headturn();

  inch1 = rc_values[RC_CH1]; // read and store channel value from receiver
  inch3 = rc_values[RC_CH3];
  inch4 = rc_values[RC_CH4];
  
  //text();
}

static int lastSteeringPos = -1;
void steering() {
  CH1 = inch1;
  CH1 = constrain(CH1, 1000, 2000);
  ServoSteeringPos = map(CH1, 1000, 2000, steeringmin, steeringmax);
  // Reduce jitter
  if (ServoSteeringPos == 89 || ServoSteeringPos == 91) {
    ServoSteeringPos = 90;
  }
  if (ServoSteeringPos != lastSteeringPos) {
    SteeringServo.write(ServoSteeringPos);
    lastSteeringPos = ServoSteeringPos;
  }
}

static int lastTiltPos = -1;
void headtilt() {
  CH3 = inch3;
  CH3 = constrain(CH3, 1000, 2000);
  ServoTiltHeadPos = map(CH3, 1000, 2000, headtiltmin, headtiltmax);
  if (ServoTiltHeadPos != lastTiltPos) {
    HeadTiltServo.write(ServoTiltHeadPos);
    lastTiltPos = ServoTiltHeadPos;
  }
}

static int lastTurnPos = -1;
void headturn() {
  CH4 = inch4;
  CH4 = constrain(CH4, 1000, 2000);
  ServoTurnHeadPos = map(CH4, 1000, 2000, headturnmin, headturnmax);
  if (ServoTurnHeadPos != lastTurnPos) {
    HeadTurnServo.write(ServoTurnHeadPos);
    lastTurnPos = ServoTurnHeadPos;
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

void calc_ch1() {
  calc_input(RC_CH1, RC_CH1_INPUT);
}
void calc_ch3() {
  calc_input(RC_CH3, RC_CH3_INPUT);
}
void calc_ch4() {
  calc_input(RC_CH4, RC_CH4_INPUT);
}

void text() {
  Serial.print("inch1:"); Serial.print(inch1); Serial.print("\t"); // uncomment these inch values to see the raw data from the receiver
  Serial.print("inch3:"); Serial.print(inch3); Serial.print("\t");
  Serial.print("CH1:"); Serial.print(CH1); Serial.print("\t");
  Serial.print("CH3:"); Serial.print(CH3); Serial.print("\t");
  Serial.print("steer:"); Serial.print(ServoSteeringPos); Serial.print("\t");
  Serial.print("tilt:"); Serial.print(ServoTiltHeadPos); Serial.print("\t");
  Serial.print("inch4:"); Serial.print(inch4); Serial.print("\t");
  Serial.print("CH4:"); Serial.print(CH4); Serial.print("\t");
  Serial.print("turn:"); Serial.print(ServoTurnHeadPos); Serial.print("\t");
  Serial.println("\t");
}
