// Random Colored Boxes
// This code is based on the Adafruit graphics and RGBmatrix panel library

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);


void setup() {
  matrix.begin();
}

void loop() {
  matrix.fillRect(random(0,32), random(0,32), random(0,32), random(0,32), matrix.Color333(random(0,8), random(0,8), random(0,8)));
}
