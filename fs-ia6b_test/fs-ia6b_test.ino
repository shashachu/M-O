
/**
 * Standalone test for the FlySky FS i6x transmitter and FS-IA6B 6-channel receiver.
 *
 * Note: Channel 5 mapped to SWD (must be enabled in Settings in the transmitter)
 *       Channel 6 mapped to VRA
 * Adapted from https://medium.com/@werneckpaiva/how-to-read-rc-receiver-signal-with-arduino-54e0447f6c3f
 */

#define CH1 A0
#define CH2 A1
#define CH3 A2
#define CH4 A3
#define CH5 A4
#define CH6 A5

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup(){
  Serial.begin(115200);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH5, INPUT);
}

int ch1Value, ch2Value, ch3Value, ch4Value, ch6Value;
bool ch5Value;

void loop() {
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, -100);
  ch4Value = readChannel(CH4, -100, 100, 0);
  ch5Value = readSwitch(CH5, false);
  ch6Value = readChannel(CH6, -100, 100, 0);
  
  Serial.print("Ch1: ");
  Serial.print(ch1Value);
  Serial.print(" Ch2: ");
  Serial.print(ch2Value);
  Serial.print(" Ch3: ");
  Serial.print(ch3Value);
  Serial.print(" Ch4: ");
  Serial.print(ch4Value);
  Serial.print(" Ch5: ");
  Serial.print(ch5Value);
  Serial.print(" Ch6: ");
  Serial.println(ch6Value);
  delay(500);
}
