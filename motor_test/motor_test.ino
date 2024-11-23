// Motor pin connections
const int EN_A = 2;
const int IN_1 = 3;
const int IN_2 = 4;

/**
 * Isolated test of 12V motor + L298N motor driver
 * Adapted from https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/ 
 */

void setup() {
  pinMode(EN_A, OUTPUT);
	pinMode(IN_1, OUTPUT);
	pinMode(IN_1, OUTPUT);

  // Turn off motor
  digitalWrite(IN_1, LOW);
	digitalWrite(IN_2, LOW);
}

void loop() {
  directionControl();
	delay(1000);
	speedControl();
	delay(1000);
}

void directionControl() {
	// Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	analogWrite(EN_A, 255);

	// Turn on motor forward
	digitalWrite(IN_1, HIGH);
	digitalWrite(IN_2, LOW);
	delay(2000);
	
	// Change to backwards
	digitalWrite(IN_1, LOW);
	digitalWrite(IN_2, HIGH);
	delay(2000);
	
	// Turn off motor
	digitalWrite(IN_1, LOW);
	digitalWrite(IN_2, LOW);
}

void speedControl() {
	// Turn on motor
	digitalWrite(IN_1, LOW);
	digitalWrite(IN_2, HIGH);
	
	// Accelerate from zero to maximum speed
	for (int i = 0; i < 256; i++) {
		analogWrite(EN_A, i);
		delay(20);
	}
	
	// Decelerate from maximum speed to zero
	for (int i = 255; i >= 0; --i) {
		analogWrite(EN_A, i);
		delay(20);
	}
	
	// Now turn off motor
	digitalWrite(IN_1, LOW);
	digitalWrite(IN_2, LOW);
}
