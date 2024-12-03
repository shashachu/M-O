/*
 * Sketch to test driving two TFT screens from an ESP32. Uses some eye animations for M-O.
 * 
 * TFT_eSPI() User_Setup.h - uncomment and set following lines:
 *    #define ILI9341_2_DRIVER
 *    #define TFT_MISO 19
 *    #define TFT_MOSI 23
 *    #define TFT_SCLK 18 
 *    #define TFT_DC   15
 *    #define TFT_RST  2 
 *
 *
 *
 *                    Wiring Diagram:
 *
 *                   ┌──────────────────────────────────────────────────────────┐                                          
 *                   │                                                          │    Shared Pins:                          
 *                   │                            ┌────────────────┐            │    23 (MOSI)                             
 *                   │                            │                │            │    19 (MISO)                             
 *                   │                            │       23 (MOSI)┼────┐       │    18 (SCK)                              
 *                   o───┐                        │                │    │       │    2  (RST)                              
 *                   │   │                        │     ESP32      │    │       │    DC (15)                               
 *       ┌───────┐   │   │                        │                │    │       │                                          
 *       │     + │   │ - │                        │                │    │       │    Unique Pins:                          
 *       │     ┌─┼───┼──┐│                        │       19 (MISO)┼──┐ │       │    TFT1 - CS -> 4                        
 *       │     │        ││                        │       18 (SCK) ├─┐│ │       │    TFT  - CS -> 25                       
 *       │     │   5V   ││                        │                │ ││ │       │                                          
 *       │     │        ││                    ┌───┼25 (CS2)        │ ││ │       │                                          
 *       │     └────────┘│                    │   │                │ ││ │       │                                          
 *       │               │                    │   │                │ ││ │       │                                          
 *       │               │                    │   │                │ ││ │       │                                          
 *       │               │                    │   │                │ ││ │       │                                          
 *       │               └────────────────────┼───┼GND     4  (CS1)┼─┼┼─┼─────┐ │                                          
 *       │                                    │   │        2  (RST)┼─┼┼─┼────┐│ │                                          
 *       └──────────────────────────────────o─┼─o─┼VIN     15 (DC) ┼─┼┼─┼─┐  ││ │                                          
 *                                          │ │ │ └────────────────┘ ││ │ │  ││ │                                          
 *                                          │ │ │                    ││ │ │  ││ │                                          
 *                                          │ │ │                    ││ │ │  ││ │                                          
 *                                          │ │ │                    ││ │ │  ││ │                                          
 * ┌───────────────────────────────────────┐│ │ │                    ││ │ │  ││ │ ┌───────────────────────────────────────┐
 * │                                       ││ │ │                    ││ │ │  ││ │ │                                       │
 * │                                       ││ │ │                    ││ │ │  ││ │ │                                       │
 * │                                 VCC   ┼┘ │ o────────o───────────┼┼─┼─┼──┼┼─┼─┼ VCC                                   │
 * │                                 GND   ┼──┼─┘        │           ││ │ │  ││ └─┼ GND                                   │
 * │                                 CS    ├──┘          │           ││ │ │  │└───┼ CS                                    │
 * │                                 RESET ┼─────────────┼───────────┼┼─┼─┼──o────┼ RESET                                 │
 * │            TFT2                 DC    ┼─────────────┼───────────┼┼─┼─o───────┤ DC             TFT1                   │
 * │                                 MOSI  ┼─────────────┼───────────┼┼─o─────────┼ MOSI                                  │
 * │                                 SCK   ┼─────────────┼───────────o┼───────────┼ SCK                                   │
 * │                                 LED   ┼─────────────o────────────┼───────────┼ LED                                   │
 * │                                 MISO  ┼──────────────────────────o───────────┼ MISO                                  │
 * │                                       │                                      │                                       │
 * │                                       │                                      │                                       │
 * │                                       │                                      │                                       │
 * └───────────────────────────────────────┘                                      └───────────────────────────────────────┘
 */

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#define TFT_CS_LEFT   4
#define TFT_CS_RIGHT 25

#define BLACK TFT_BLACK
#define YELLOW TFT_YELLOW

static int sDisplayWidth = 0;
static int sDisplayHeight = 0;

class Expression {
public:
  void animateIn(TFT_eSPI* tft) {
    log("animateIn");
    doAnimateIn(tft);
  }
  void draw(TFT_eSPI* tft) {
    log("draw");
    doDraw(tft);
  }
  void animateOut(TFT_eSPI* tft) {
    log("animateOut");
    doAnimateOut(tft);
  }
protected:
  virtual void doAnimateIn(TFT_eSPI* tft) {
    tft->fillScreen(BLACK);
  }

  virtual void doDraw(TFT_eSPI* tft) = 0;

  virtual void doAnimateOut(TFT_eSPI* tft) {
    tft->fillScreen(BLACK);
  }

  virtual char* debugTag() = 0;

  void log(char * log) {
    //Serial.print(debugTag()); Serial.print(":\t"); Serial.println(log);
  }
};

class Normal : public Expression {
public:  
  char* debugTag() { return "Normal"; }

  void doAnimateIn(TFT_eSPI* tft) override {
    int i;
    int height = sDisplayHeight - Y0;
    float halfHeight = (sDisplayHeight - Y0) / 2.0f;
    int step = (int)(halfHeight / STEPS);
    for (i = 0; i < STEPS - 1; i++) {
      tft->fillRoundRect(X0, halfHeight, sDisplayWidth - X0, 2 * step * i, 0, YELLOW);
    }
  }

  void doDraw(TFT_eSPI* tft) override {
    tft->fillRoundRect(X0, Y0, sDisplayWidth - X0, sDisplayHeight - Y0, CORNER, YELLOW);
  }

  void doAnimateOut(TFT_eSPI* tft) override {
    int i;
    int height = sDisplayHeight - Y0;
    float halfHeight = height / 2.0f;
    int step = (int)(halfHeight / STEPS);
    for (i = 0; i <= STEPS; i++) {
      tft->fillRoundRect(X0, Y0, sDisplayWidth - X0, step * i, 0, BLACK);
      tft->fillRoundRect(X0, sDisplayHeight - step * i, sDisplayWidth - X0, step * i, 0, BLACK);
    }
  }
private:
  const int X0 = 40;
  const int Y0 = 60;
  const int CORNER = 8;
  const int STEPS = 10;
};

class Up : public Expression {
public:  
  char* debugTag() { return "Up"; }

  void doAnimateIn(TFT_eSPI* tft) override {
    int i;
    int height = sDisplayHeight - Y_BOTTOM;
    float halfHeight = height / 2.0f;
    int step = (int)(halfHeight / STEPS);
    for (i = 0; i < STEPS; i++) {
      tft->fillRoundRect(X0, halfHeight - step * i, sDisplayWidth - X0, 2 * step * i, CORNER, YELLOW);
    }
  }

  void doDraw(TFT_eSPI* tft) override {
    tft->fillRoundRect(X0, 0, sDisplayWidth - X0, sDisplayHeight - Y_BOTTOM, CORNER, YELLOW);
  }

  void doAnimateOut(TFT_eSPI* tft) override {
    int i;
    int height = sDisplayHeight - Y_BOTTOM;
    float halfHeight = height / 2.0f;
    int step = (int)(halfHeight / STEPS);
    for (i = 0; i <= STEPS; i++) {
      tft->fillRoundRect(X0, Y0, sDisplayWidth - X0, step * i, 0, BLACK);
      tft->fillRoundRect(X0, height - step * i, sDisplayWidth - X0, step * i, 0, BLACK);
    }
  }
private:
  const int X0 = 40;
  const int Y0 = 0;
  const int Y_BOTTOM = 120;
  const int CORNER = 8;
  const int STEPS = 10;
};

class Sad : public Expression {
public:  
  char* debugTag() { return "Sad"; }

  void doDraw(TFT_eSPI* tft) override {
    tft->fillRoundRect(20, 120, sDisplayWidth, 180, 0, YELLOW);
    tft->fillTriangle(20, 120, sDisplayWidth, 120, 20, 40, YELLOW);
  }
private:
  const int X0 = 40;
  const int Y0 = 0;
  const int CORNER = 8;
  const int STEPS = 10;
};

class Angry2 : public Expression {
public:  
  char* debugTag() { return "Angry2"; }

  void doDraw(TFT_eSPI* tft) override {
    tft->fillRoundRect(20, 140, sDisplayWidth, 220, 0, YELLOW);
    tft->fillTriangle(20, 140, sDisplayWidth, 140, sDisplayWidth, 100, YELLOW);
  }
private:
  const int X0 = 40;
  const int Y0 = 0;
  const int CORNER = 8;
  const int STEPS = 10;
};

class Happy : public Expression {
public:  
  char* debugTag() { return "Happy"; }

  void doDraw(TFT_eSPI* tft) override {
    tft->fillCircle(160, 250 , 220, YELLOW);
    tft->fillCircle(160, 350, 220, BLACK);
    tft->fillRect(0, 0, 40, 240, BLACK);
    tft->fillRect(280, 0, 320, 240, BLACK);
  }
private:
  const int X0 = 40;
  const int Y0 = 0;
  const int STEPS = 10;
};

#define NUM_EXPRESSIONS 5

Expression* sExpressions[NUM_EXPRESSIONS] = {
   new Normal(),
   new Up(),
   new Sad(),
   new Angry2(),
   new Happy(),
};

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void setup(void) {
  pinMode(TFT_CS_LEFT, OUTPUT);
  pinMode(TFT_CS_RIGHT, OUTPUT);
  digitalWrite(TFT_CS_LEFT, LOW);
  digitalWrite(TFT_CS_RIGHT, LOW);
  tft.init();

  // Left eye rotation 1
  digitalWrite(TFT_CS_RIGHT, HIGH);
  tft.setRotation(1);

  delay(100);

  // Right eye rotation 5 (mirrored)
  digitalWrite(TFT_CS_LEFT, HIGH);
  digitalWrite(TFT_CS_RIGHT, LOW);
  tft.setRotation(5);

  delay(100);

  // Re-enable left screen
  digitalWrite(TFT_CS_LEFT, LOW);

  // Clear both displays
  tft.fillScreen(BLACK);
  sDisplayWidth = tft.width();
  sDisplayHeight = tft.height();

  delay(100);
}

const int DELAY = 2000;

void loop(void) {
  delay(1000);
  for (int i = 0; i < NUM_EXPRESSIONS; i++) {
    sExpressions[i]->animateIn(&tft);
    sExpressions[i]->draw(&tft);
    delay(DELAY);
    sExpressions[i]->animateOut(&tft);
    delay(DELAY);
  }
}