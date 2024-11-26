Test sketches and modifications for an M-O build based on Matthew Zwarts' files and code.

[https://wallebuilders.club/](https://wallebuilders.club/)

# Contents:
* [warning_light_test](https://github.com/shashachu/M-O/tree/main/warning_light_test) - Standalone warning light test
* [motor_test](https://github.com/shashachu/M-O/tree/main/motor_test) - Standalone motor test
* [fs-ia6b_test](https://github.com/shashachu/M-O/tree/main/fs-ia6b_test/) - Standalone RC transmitter/receiver test
* [dfplayermini_test](https://github.com/shashachu/M-O/tree/main/dfplayermini_test/) - Standalone DFPlayerMini test, with potentiometer for volume control
* [tft_test](https://github.com/shashachu/M-O/tree/main/tft_test) - Standalone TFT screen test

# Planned Pin Assignments (Nano)

RC Receiver
* D7 - RC Ch1
* D8 - RC Ch2
* D9 - RC Ch3
* D10 - RC Ch4
* D11 - RC Ch5
* D12 - RC Ch6

Motor Driver (As digital pins)
* A0
* A1
* A2

PCA9685
* A4 (SDA)
* A5 (SCL)

DFPlayerMini
* D5 (RX, TX of DFPlayerMini)
* D6 (TX, RX of DFPlayerMini)

Volume Control
* A6

Warning Light LEDs
* D2
