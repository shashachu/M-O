/**
 * Standalone test for DFPlayerMini, with volume control driven from a potentiometer.
 *
 * Note: Format MicroSD card as FAT32. Filenames are 1-based, 4-digit numbers. e.g. 0001.wav. Despite what the docs say,
 * I had to copy them into the root of the card, not a folder named mp3/.
 *
 * Note: Connect ground to left/counterclockwise pin of potentiometer, or else values will be reversed.
 *
 * Note: DFPlayerMini and Arduino RX/TX pins are swapped. RX of DFPlayerMini goes to TX of Arduino, and vice versa. 1k resistor should also 
 * be connected in serial with the DFPlayerMini's RX pin.
 */

#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

const int DFPLAYER_PIN_RX = 10;
const int DFPLAYER_PIN_TX = 11;
const int SWITCH_PIN = 2;
const int VOLUME_PIN = A6;

SoftwareSerial mySerial(DFPLAYER_PIN_RX, DFPLAYER_PIN_TX); // RX, TX for DFPlayer Mini
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  mySerial.begin(9600);
  Serial.begin(115200);
  Serial.println("Begin DFPlayerMini test");
  delay(2000);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println(F("Error initializing DFPlayer!"));
    while (true); // Error initializing DFPlayer Mini
  }
  Serial.println(F("DFPlayerMini is online"));
}

void loop() {
  int potValue = analogRead(VOLUME_PIN); // Read potentiometer
  Serial.print(F("potValue: ")); Serial.println((String)potValue);
  int volume = map(potValue, 0, 1023, 0, 30); // Map potentiometer to volume range
  Serial.print("volume: "); Serial.println((String)volume);
  myDFPlayer.volume(volume);
  myDFPlayer.play(1);
  delay(5000);
}