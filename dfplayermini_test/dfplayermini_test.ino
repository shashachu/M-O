#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

const int DFPLAYER_PIN = 6;
const int SWITCH_PIN = 2;

int switchState = 0;

SoftwareSerial mySerial(10, 11); // RX, TX for DFPlayer Mini
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Use internal pull-up resistor
  mySerial.begin(9600);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println(F("Error initializing DFPLayer!"));
    while (true); // Error initializing DFPlayer Mini
  }
  Serial.println(F("DFPlayerMini is online"));
  myDFPlayer.volume(15); // Initial volume (0-30)
}

void loop() {
  int potValue = analogRead(A0); // Read potentiometer
  Serial.print(F("potValue: ")); Serial.println((String)potValue);
  int volume = map(potValue, 0, 1023, 0, 30); // Map potentiometer to volume range
  Serial.print("volume: "); Serial.println((String)volume);
  myDFPlayer.volume(volume); // Set volume

  switchState = digitalRead(SWITCH_PIN);

  if (switchState == LOW) { // Button is pressed
    myDFPlayer.play(1);
  }

  delay(100);
}