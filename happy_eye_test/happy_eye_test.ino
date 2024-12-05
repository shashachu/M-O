/**
 * Test sketch for faster happy eye rendering using arc drawing
 */

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#define TFT_CS_LEFT   4
#define TFT_CS_RIGHT 25

#define BLACK TFT_BLACK
#define YELLOW TFT_YELLOW

// Define your TFT pins
#define TFT_CS 4
TFT_eSPI tft = TFT_eSPI();  

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  drawFilledArc(160, 200, 150, 20, 160, YELLOW);
  drawFilledArc(160, 240, 150, 20, 160, BLACK);
}

void loop() {
  // nothing
}

/**
 * Function to draw an arc using drawFastHLine. Note: this only works for angles between 0 and 180.
 */
void drawFilledArc(int x, int y, int radius, int startAngle, int endAngle, uint16_t color) {
  // Convert angles to radians
  float startRad = startAngle * PI / 180;
  float endRad = endAngle * PI / 180;

  // Loop through vertical pixels within the circle's bounding box
  for (int i = 0; i <= radius; i++) {
    int yOffset = y - i; // Change to y + i if you want to draw the bottom half of the arc
    float xSpan = sqrt(radius * radius - i * i); // Half-width of the arc at this height

    int xStart = x - xSpan;
    int xEnd = x + xSpan;

    // Check angle range at the edges of this span
    float angleStart = atan2(i, -xSpan);
    float angleEnd = atan2(i, xSpan);

    if (angleStart < 0) angleStart += 2 * PI;
    if (angleEnd < 0) angleEnd += 2 * PI;

    // Adjust order if the angles are inverted
    if (angleStart > angleEnd) std::swap(angleStart, angleEnd);

    // If the arc overlaps the current segment, draw the line
    if (angleStart <= endRad && angleEnd >= startRad) {
      int arcStart = x - xSpan;
      if (startAngle != 0 && startAngle != 180) {
        if (startAngle == 90 || startAngle == 270) {
          arcStart = 0;
        } else {
          arcStart = (int) x - abs(i / tan(startRad));
        }
      }
      int arcEnd = x + xSpan;
      if (endAngle != 0 && endAngle != 180) {
        if (endAngle == 90 || endAngle == 270) {
          arcEnd = 0;
        } else {
          arcEnd = (int) x + abs(i / tan(endRad));
        }
      }
      int constrainedStart = max(xStart, arcStart);
      int constrainedEnd = min(xEnd, arcEnd);
      tft.drawFastHLine(constrainedStart, yOffset, constrainedEnd - constrainedStart + 1, color);
    }
  }
}
