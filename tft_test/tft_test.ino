/**
 * Standalone test for TFT screen with Arduino Nano. Ported from Matthew Zwarts' 'left eye' M-O code.
 *
 * The screen I am using: https://www.amazon.com/gp/product/B073R7BH1B/
 * Helpful links: 
 *   - https://simple-circuit.com/interfacing-arduino-ili9341-tft-display/
 *   - https://www.youtube.com/watch?v=4DtuOeeYHys
 */
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS    10      
#define TFT_RST   8      
#define TFT_DC    9     
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

#define BLACK ILI9341_BLACK
#define YELLOW ILI9341_YELLOW

static int sDisplayWidth = 0;
static int sDisplayHeight = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("TFT Test!"); 
 
  tft.begin();
  tft.setRotation(1);
  sDisplayWidth = tft.width();
  sDisplayHeight = tft.height();
}

void eyesnormal() {
  Serial.println("normal");
  tft.fillScreen(BLACK);
  tft.fillRoundRect(40, 60, sDisplayWidth - 40, sDisplayHeight - 60, 8, YELLOW);
}

void eyessad() {
  Serial.println("sad");
  tft.fillScreen(BLACK);
  tft.fillRoundRect(20, 120, sDisplayWidth, 180, 0, YELLOW);
  tft.fillTriangle(20, 120, sDisplayWidth, 120, 20, 40, YELLOW);
}

void eyesup() {
  Serial.println("up");
  tft.fillScreen(BLACK);
  tft.fillRoundRect(40, 0, sDisplayWidth - 40, sDisplayHeight - 120, 8, YELLOW);
}

void eyesangry2() {
  Serial.println("angry2");
  
  tft.fillScreen(BLACK);
  tft.fillRoundRect(20, 140, sDisplayWidth, 220, 0, YELLOW);
  tft.fillTriangle(20, 140, sDisplayWidth, 140, sDisplayWidth, 100, YELLOW);
}

void eyeshappy() {
  Serial.println("happy");
  tft.fillScreen(BLACK);
  tft.fillCircle(160, 250 , 220, YELLOW);
  tft.fillCircle(160, 350, 220, BLACK);
  tft.fillRect(0, 0, 40, 240, BLACK);
  tft.fillRect(280, 0, 320, 240, BLACK);
}

const int DELAY = 2000;

void loop(void) {
  eyesnormal();
  delay(DELAY);
  eyessad();
  delay(DELAY);
  eyesup();
  delay(DELAY);
  eyesangry2();
  delay(DELAY);
  eyeshappy();
  delay(DELAY);
}