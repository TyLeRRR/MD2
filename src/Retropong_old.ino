// http://arduino.cc/en/Tutorial/TFTPong
// and RGBmatrixPanel examples
// https://github.com/adafruit/RGB-matrix-Panel
// and Adafruit-GFX-Library
// https://github.com/adafruit/Adafruit-GFX-Library

// The Wiring setup is the same as in RGB-matrix-Panel examples,
// described at
// http://learn.adafruit.com/32x16-32x32-rgb-led-matrix/wiring-the-16x32-matrix

// also it has to connect 2 10k potentiometers at A4 and A5 as described in
// http://arduino.cc/en/Tutorial/TFTPong

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <SPI.h>

#define CLK  11 // MUST be on PORTB!
#define LAT 10
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define D   A3
// Last parameter = 'true' enables double-buffering, for flicker-free,
// buttery smooth animation.  Note that NOTHING WILL SHOW ON THE DISPLAY
// until the first call to swapBuffers().  This is normal.
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

uint16_t black_color = matrix.Color444(0, 0, 0);
uint16_t blue_color = matrix.Color444(0, 0, 255);
uint16_t red_color = matrix.Color444(255, 0, 0);
uint16_t green_color = matrix.Color444(0, 180, 0);
uint16_t white_color = matrix.Color444(255, 255, 255);
uint16_t yellow_color = matrix.Color444(201, 171, 0);

int horiz_paddleWidth = 6;
int horiz_paddleHeight = 1;

int vertical_paddleWidth = 1;
int vertical_paddleHeight = 6;

int ballDiameter = 1;

// paddle BOT
int bot_paddleX = 0;
int bot_paddleY = 0;
int bot_oldPaddleX, bot_oldPaddleY;

// paddle TOP
int top_paddleX = 0;
int top_paddleY = 31;
int top_oldPaddleX, top_oldPaddleY;

// paddle LEFT
int left_paddleX = 31;
int left_paddleY = 16;
int left_oldPaddleX, left_oldPaddleY;

// paddle RIGHT
int right_paddleX = 0;
int right_paddleY = 16;
int right_oldPaddleX, right_oldPaddleY;

// players score
int bot_score = 9;
int top_score = 9;
int left_score = 9;
int right_score = 9;

int ballDirectionX = 1;
int ballDirectionY = 1;

int bot_paddle__poti = A4;
int top_paddle_poti = A6;
int left_paddle_poti = A5;
int right_paddle_poti = A7;

int ballSpeed = 150; //lower numbers are faster
unsigned long howLongToShowScore = 500;// in miliseconds

boolean isPlayerTopDead = false;

int ballX, ballY, oldBallX, oldBallY;

void clear() {
    matrix.fillScreen(black_color);
}

void clearScoreTop() {
    matrix.fillRect(14, 24, 7, 7, black_color);
}

void setup() {
    pinMode(bot_paddle__poti, INPUT);
    pinMode(top_paddle_poti, INPUT);
    pinMode(left_paddle_poti, INPUT);
    pinMode(right_paddle_poti, INPUT);
    matrix.begin();
    clear();
}

void print_9_BOT() {
    matrix.drawPixel(17, 1, red_color);
    matrix.drawPixel(16, 1, red_color);
    matrix.drawPixel(15, 1, red_color);
    matrix.drawPixel(14, 2, red_color);
    matrix.drawPixel(13, 3, red_color);
    matrix.drawPixel(13, 4, red_color);
    matrix.drawPixel(13, 5, red_color);
    matrix.drawPixel(13, 6, red_color);
    matrix.drawPixel(14, 7, red_color);
    matrix.drawPixel(15, 7, red_color);
    matrix.drawPixel(16, 7, red_color);
    matrix.drawPixel(17, 6, red_color);
    matrix.drawPixel(17, 5, red_color);
    matrix.drawPixel(16, 4, red_color);
    matrix.drawPixel(15, 4, red_color);
    matrix.drawPixel(14, 4, red_color);
}

void print_9_RIGHT() {
    matrix.drawPixel(1, 14, blue_color);
    matrix.drawPixel(1, 15, blue_color);
    matrix.drawPixel(1, 16, blue_color);
    matrix.drawPixel(2, 17, blue_color);
    matrix.drawPixel(3, 18, blue_color);
    matrix.drawPixel(4, 18, blue_color);
    matrix.drawPixel(5, 18, blue_color);
    matrix.drawPixel(6, 18, blue_color);
    matrix.drawPixel(7, 17, blue_color);
    matrix.drawPixel(7, 16, blue_color);
    matrix.drawPixel(7, 15, blue_color);
    matrix.drawPixel(6, 14, blue_color);
    matrix.drawPixel(5, 14, blue_color);
    matrix.drawPixel(4, 15, blue_color);
    matrix.drawPixel(4, 16, blue_color);
    matrix.drawPixel(4, 17, blue_color);
}

void print_9_LEFT() {
    matrix.drawPixel(30, 17, white_color);
    matrix.drawPixel(30, 16, white_color);
    matrix.drawPixel(30, 15, white_color);
    matrix.drawPixel(29, 14, white_color);
    matrix.drawPixel(28, 13, white_color);
    matrix.drawPixel(27, 13, white_color);
    matrix.drawPixel(26, 13, white_color);
    matrix.drawPixel(25, 13, white_color);
    matrix.drawPixel(24, 14, white_color);
    matrix.drawPixel(24, 15, white_color);
    matrix.drawPixel(24, 16, white_color);
    matrix.drawPixel(25, 17, white_color);
    matrix.drawPixel(26, 17, white_color);
    matrix.drawPixel(27, 16, white_color);
    matrix.drawPixel(27, 15, white_color);
    matrix.drawPixel(27, 14, white_color);
}

void print_8_BOT() {
    matrix.drawPixel(16, 1, red_color);
    matrix.drawPixel(15, 1, red_color);
    matrix.drawPixel(14, 1, red_color);
    matrix.drawPixel(16, 4, red_color);
    matrix.drawPixel(15, 4, red_color);
    matrix.drawPixel(14, 4, red_color);
    matrix.drawPixel(16, 7, red_color);
    matrix.drawPixel(15, 7, red_color);
    matrix.drawPixel(14, 7, red_color);
    matrix.drawPixel(17, 2, red_color);
    matrix.drawPixel(17, 3, red_color);
    matrix.drawPixel(17, 5, red_color);
    matrix.drawPixel(17, 6, red_color);
    matrix.drawPixel(13, 2, red_color);
    matrix.drawPixel(13, 3, red_color);
    matrix.drawPixel(13, 5, red_color);
    matrix.drawPixel(13, 6, red_color);
}

void print_8_RIGHT() {
    matrix.drawPixel(1, 15, blue_color);
    matrix.drawPixel(1, 16, blue_color);
    matrix.drawPixel(1, 17, blue_color);
    matrix.drawPixel(4, 15, blue_color);
    matrix.drawPixel(4, 16, blue_color);
    matrix.drawPixel(4, 17, blue_color);
    matrix.drawPixel(7, 15, blue_color);
    matrix.drawPixel(7, 16, blue_color);
    matrix.drawPixel(7, 17, blue_color);
    matrix.drawPixel(2, 14, blue_color);
    matrix.drawPixel(3, 14, blue_color);
    matrix.drawPixel(5, 14, blue_color);
    matrix.drawPixel(6, 14, blue_color);
    matrix.drawPixel(2, 18, blue_color);
    matrix.drawPixel(3, 18, blue_color);
    matrix.drawPixel(5, 18, blue_color);
    matrix.drawPixel(6, 18, blue_color);

}

void print_8_LEFT() {
    matrix.drawPixel(30, 15, white_color);
    matrix.drawPixel(30, 16, white_color);
    matrix.drawPixel(30, 17, white_color);
    matrix.drawPixel(27, 15, white_color);
    matrix.drawPixel(27, 16, white_color);
    matrix.drawPixel(27, 17, white_color);
    matrix.drawPixel(24, 15, white_color);
    matrix.drawPixel(24, 16, white_color);
    matrix.drawPixel(24, 17, white_color);
    matrix.drawPixel(29, 14, white_color);
    matrix.drawPixel(28, 14, white_color);
    matrix.drawPixel(26, 14, white_color);
    matrix.drawPixel(25, 14, white_color);
    matrix.drawPixel(29, 18, white_color);
    matrix.drawPixel(28, 18, white_color);
    matrix.drawPixel(26, 18, white_color);
    matrix.drawPixel(25, 18, white_color);
}

void print_7_BOT() {
    matrix.drawPixel(17, 1, red_color);
    matrix.drawPixel(16, 2, red_color);
    matrix.drawPixel(15, 3, red_color);
    matrix.drawPixel(14, 4, red_color);
    matrix.drawPixel(13, 5, red_color);
    matrix.drawPixel(13, 6, red_color);
    matrix.drawPixel(13, 7, red_color);
    matrix.drawPixel(14, 7, red_color);
    matrix.drawPixel(15, 7, red_color);
    matrix.drawPixel(16, 7, red_color);
    matrix.drawPixel(17, 7, red_color);
}

void print_7_RIGHT() {
    matrix.drawPixel(1, 14, blue_color);
    matrix.drawPixel(2, 15, blue_color);
    matrix.drawPixel(3, 16, blue_color);
    matrix.drawPixel(4, 17, blue_color);
    matrix.drawPixel(5, 18, blue_color);
    matrix.drawPixel(6, 18, blue_color);
    matrix.drawPixel(7, 18, blue_color);
    matrix.drawPixel(7, 17, blue_color);
    matrix.drawPixel(7, 16, blue_color);
    matrix.drawPixel(7, 15, blue_color);
    matrix.drawPixel(7, 14, blue_color);
}

void print_7_LEFT() {
    matrix.drawPixel(30, 18, white_color);
    matrix.drawPixel(29, 17, white_color);
    matrix.drawPixel(28, 16, white_color);
    matrix.drawPixel(27, 15, white_color);
    matrix.drawPixel(26, 14, white_color);
    matrix.drawPixel(25, 14, white_color);
    matrix.drawPixel(24, 14, white_color);
    matrix.drawPixel(24, 15, white_color);
    matrix.drawPixel(24, 16, white_color);
    matrix.drawPixel(24, 17, white_color);
    matrix.drawPixel(24, 18, white_color);
}

void print_6_BOT() {
    matrix.drawPixel(16, 1, red_color);
    matrix.drawPixel(15, 1, red_color);
    matrix.drawPixel(14, 1, red_color);
    matrix.drawPixel(13, 2, red_color);
    matrix.drawPixel(13, 3, red_color);
    matrix.drawPixel(14, 4, red_color);
    matrix.drawPixel(15, 4, red_color);
    matrix.drawPixel(16, 4, red_color);
    matrix.drawPixel(17, 2, red_color);
    matrix.drawPixel(17, 3, red_color);
    matrix.drawPixel(17, 4, red_color);
    matrix.drawPixel(17, 5, red_color);
    matrix.drawPixel(16, 6, red_color);
    matrix.drawPixel(15, 7, red_color);
    matrix.drawPixel(14, 7, red_color);
    matrix.drawPixel(13, 7, red_color);
}

void print_6_RIGHT() {
    matrix.drawPixel(1, 15, blue_color);
    matrix.drawPixel(1, 16, blue_color);
    matrix.drawPixel(1, 17, blue_color);
    matrix.drawPixel(2, 18, blue_color);
    matrix.drawPixel(3, 18, blue_color);
    matrix.drawPixel(4, 17, blue_color);
    matrix.drawPixel(4, 16, blue_color);
    matrix.drawPixel(4, 15, blue_color);
    matrix.drawPixel(2, 14, blue_color);
    matrix.drawPixel(3, 14, blue_color);
    matrix.drawPixel(4, 14, blue_color);
    matrix.drawPixel(5, 14, blue_color);
    matrix.drawPixel(6, 15, blue_color);
    matrix.drawPixel(7, 16, blue_color);
    matrix.drawPixel(7, 17, blue_color);
    matrix.drawPixel(7, 18, blue_color);
}

void print_6_LEFT() {
    matrix.drawPixel(30, 14, white_color);
    matrix.drawPixel(30, 15, white_color);
    matrix.drawPixel(30, 16, white_color);
    matrix.drawPixel(29, 13, white_color);
    matrix.drawPixel(28, 13, white_color);
    matrix.drawPixel(27, 14, white_color);
    matrix.drawPixel(27, 15, white_color);
    matrix.drawPixel(27, 16, white_color);
    matrix.drawPixel(29, 17, white_color);
    matrix.drawPixel(28, 17, white_color);
    matrix.drawPixel(27, 17, white_color);
    matrix.drawPixel(26, 17, white_color);
    matrix.drawPixel(25, 16, white_color);
    matrix.drawPixel(24, 15, white_color);
    matrix.drawPixel(24, 14, white_color);
    matrix.drawPixel(24, 13, white_color);
}

void print_5_BOT() {
    matrix.drawPixel(16, 1, red_color);
    matrix.drawPixel(15, 1, red_color);
    matrix.drawPixel(14, 1, red_color);
    matrix.drawPixel(17, 2, red_color);
    matrix.drawPixel(13, 2, red_color);
    matrix.drawPixel(13, 3, red_color);
    matrix.drawPixel(13, 4, red_color);
    matrix.drawPixel(14, 5, red_color);
    matrix.drawPixel(15, 5, red_color);
    matrix.drawPixel(16, 5, red_color);
    matrix.drawPixel(17, 5, red_color);
    matrix.drawPixel(17, 6, red_color);
    matrix.drawPixel(17, 7, red_color);
    matrix.drawPixel(16, 7, red_color);
    matrix.drawPixel(15, 7, red_color);
    matrix.drawPixel(14, 7, red_color);
    matrix.drawPixel(13, 7, red_color);
}

void print_5_RIGHT() {
    matrix.drawPixel(1, 15, blue_color);
    matrix.drawPixel(1, 16, blue_color);
    matrix.drawPixel(1, 17, blue_color);
    matrix.drawPixel(2, 14, blue_color);
    matrix.drawPixel(2, 18, blue_color);
    matrix.drawPixel(3, 18, blue_color);
    matrix.drawPixel(4, 18, blue_color);
    matrix.drawPixel(5, 17, blue_color);
    matrix.drawPixel(5, 16, blue_color);
    matrix.drawPixel(5, 15, blue_color);
    matrix.drawPixel(5, 14, blue_color);
    matrix.drawPixel(6, 14, blue_color);
    matrix.drawPixel(7, 14, blue_color);
    matrix.drawPixel(7, 15, blue_color);
    matrix.drawPixel(7, 16, blue_color);
    matrix.drawPixel(7, 17, blue_color);
    matrix.drawPixel(7, 18, blue_color);
}

void print_5_LEFT() {
    matrix.drawPixel(29, 17, white_color);
    matrix.drawPixel(30, 16, white_color);
    matrix.drawPixel(30, 15, white_color);
    matrix.drawPixel(30, 14, white_color);
    matrix.drawPixel(29, 13, white_color);
    matrix.drawPixel(28, 13, white_color);
    matrix.drawPixel(27, 13, white_color);
    matrix.drawPixel(26, 14, white_color);
    matrix.drawPixel(26, 15, white_color);
    matrix.drawPixel(26, 16, white_color);
    matrix.drawPixel(26, 17, white_color);
    matrix.drawPixel(25, 17, white_color);
    matrix.drawPixel(24, 17, white_color);
    matrix.drawPixel(24, 16, white_color);
    matrix.drawPixel(24, 15, white_color);
    matrix.drawPixel(24, 14, white_color);
    matrix.drawPixel(24, 13, white_color);
}

void print_4_BOT() {
    matrix.drawPixel(14, 1, red_color);
    matrix.drawPixel(14, 2, red_color);
    matrix.drawPixel(14, 3, red_color);
    matrix.drawPixel(14, 4, red_color);
    matrix.drawPixel(14, 5, red_color);
    matrix.drawPixel(14, 6, red_color);
    matrix.drawPixel(14, 7, red_color);
    matrix.drawPixel(15, 6, red_color);
    matrix.drawPixel(16, 5, red_color);
    matrix.drawPixel(17, 4, red_color);
    matrix.drawPixel(17, 3, red_color);
    matrix.drawPixel(16, 3, red_color);
    matrix.drawPixel(15, 3, red_color);
    matrix.drawPixel(13, 3, red_color);
}

void print_4_RIGHT() {
    matrix.drawPixel(1, 17, blue_color);
    matrix.drawPixel(2, 17, blue_color);
    matrix.drawPixel(3, 17, blue_color);
    matrix.drawPixel(4, 17, blue_color);
    matrix.drawPixel(5, 17, blue_color);
    matrix.drawPixel(6, 17, blue_color);
    matrix.drawPixel(7, 17, blue_color);
    matrix.drawPixel(6, 16, blue_color);
    matrix.drawPixel(5, 15, blue_color);
    matrix.drawPixel(4, 14, blue_color);
    matrix.drawPixel(3, 14, blue_color);
    matrix.drawPixel(3, 15, blue_color);
    matrix.drawPixel(3, 16, blue_color);
    matrix.drawPixel(3, 18, blue_color);
}

void print_4_LEFT() {
    matrix.drawPixel(30, 14, white_color);
    matrix.drawPixel(29, 14, white_color);
    matrix.drawPixel(28, 14, white_color);
    matrix.drawPixel(27, 14, white_color);
    matrix.drawPixel(26, 14, white_color);
    matrix.drawPixel(25, 14, white_color);
    matrix.drawPixel(24, 14, white_color);
    matrix.drawPixel(25, 15, white_color);
    matrix.drawPixel(26, 16, white_color);
    matrix.drawPixel(27, 17, white_color);
    matrix.drawPixel(28, 17, white_color);
    matrix.drawPixel(28, 16, white_color);
    matrix.drawPixel(28, 15, white_color);
    matrix.drawPixel(28, 13, white_color);
}

void print_3_BOT() {
    matrix.drawPixel(17, 2, red_color);
    matrix.drawPixel(16, 1, red_color);
    matrix.drawPixel(15, 1, red_color);
    matrix.drawPixel(14, 1, red_color);
    matrix.drawPixel(13, 2, red_color);
    matrix.drawPixel(13, 3, red_color);
    matrix.drawPixel(14, 4, red_color);
    matrix.drawPixel(15, 4, red_color);
    matrix.drawPixel(14, 5, red_color);
    matrix.drawPixel(13, 6, red_color);
    matrix.drawPixel(13, 7, red_color);
    matrix.drawPixel(14, 7, red_color);
    matrix.drawPixel(15, 7, red_color);
    matrix.drawPixel(16, 7, red_color);
    matrix.drawPixel(17, 7, red_color);
}

void print_3_RIGHT() {
    matrix.drawPixel(2, 14, blue_color);
    matrix.drawPixel(1, 15, blue_color);
    matrix.drawPixel(1, 16, blue_color);
    matrix.drawPixel(1, 17, blue_color);
    matrix.drawPixel(2, 18, blue_color);
    matrix.drawPixel(3, 18, blue_color);
    matrix.drawPixel(4, 17, blue_color);
    matrix.drawPixel(4, 16, blue_color);
    matrix.drawPixel(5, 17, blue_color);
    matrix.drawPixel(6, 18, blue_color);
    matrix.drawPixel(7, 18, blue_color);
    matrix.drawPixel(7, 17, blue_color);
    matrix.drawPixel(7, 16, blue_color);
    matrix.drawPixel(7, 15, blue_color);
    matrix.drawPixel(7, 14, blue_color);

}

void print_3_LEFT() {
    matrix.drawPixel(29, 17, white_color);
    matrix.drawPixel(30, 16, white_color);
    matrix.drawPixel(30, 15, white_color);
    matrix.drawPixel(30, 14, white_color);
    matrix.drawPixel(29, 13, white_color);
    matrix.drawPixel(28, 13, white_color);
    matrix.drawPixel(27, 14, white_color);
    matrix.drawPixel(27, 15, white_color);
    matrix.drawPixel(26, 14, white_color);
    matrix.drawPixel(25, 13, white_color);
    matrix.drawPixel(24, 13, white_color);
    matrix.drawPixel(24, 14, white_color);
    matrix.drawPixel(24, 15, white_color);
    matrix.drawPixel(24, 16, white_color);
    matrix.drawPixel(24, 17, white_color);
}

void print_2_BOT() {
    matrix.drawPixel(17, 1, red_color);
    matrix.drawPixel(16, 1, red_color);
    matrix.drawPixel(15, 1, red_color);
    matrix.drawPixel(14, 1, red_color);
    matrix.drawPixel(13, 1, red_color);
    matrix.drawPixel(17, 2, red_color);
    matrix.drawPixel(17, 3, red_color);
    matrix.drawPixel(16, 4, red_color);
    matrix.drawPixel(15, 4, red_color);
    matrix.drawPixel(14, 4, red_color);
    matrix.drawPixel(13, 5, red_color);
    matrix.drawPixel(13, 6, red_color);
    matrix.drawPixel(14, 7, red_color);
    matrix.drawPixel(15, 7, red_color);
    matrix.drawPixel(16, 7, red_color);
    matrix.drawPixel(17, 6, red_color);
}

void print_2_RIGHT(){
    matrix.drawPixel(1, 14, blue_color);
    matrix.drawPixel(1, 15, blue_color);
    matrix.drawPixel(1, 16, blue_color);
    matrix.drawPixel(1, 17, blue_color);
    matrix.drawPixel(1, 18, blue_color);
    matrix.drawPixel(2, 14, blue_color);
    matrix.drawPixel(3, 14, blue_color);
    matrix.drawPixel(4, 15, blue_color);
    matrix.drawPixel(4, 16, blue_color);
    matrix.drawPixel(4, 17, blue_color);
    matrix.drawPixel(5, 18, blue_color);
    matrix.drawPixel(6, 18, blue_color);
    matrix.drawPixel(7, 17, blue_color);
    matrix.drawPixel(7, 16, blue_color);
    matrix.drawPixel(7, 15, blue_color);
    matrix.drawPixel(6, 14, blue_color);
}

void print_2_LEFT() {
    matrix.drawPixel(30, 17, white_color);
    matrix.drawPixel(30, 16, white_color);
    matrix.drawPixel(30, 15, white_color);
    matrix.drawPixel(30, 14, white_color);
    matrix.drawPixel(30, 13, white_color);
    matrix.drawPixel(29, 17, white_color);
    matrix.drawPixel(28, 17, white_color);
    matrix.drawPixel(27, 16, white_color);
    matrix.drawPixel(27, 15, white_color);
    matrix.drawPixel(27, 14, white_color);
    matrix.drawPixel(26, 13, white_color);
    matrix.drawPixel(25, 13, white_color);
    matrix.drawPixel(24, 14, white_color);
    matrix.drawPixel(24, 15, white_color);
    matrix.drawPixel(24, 16, white_color);
    matrix.drawPixel(25, 17, white_color);
}

void print_1_BOT() {
    matrix.drawPixel(16, 1, red_color);
    matrix.drawPixel(15, 1, red_color);
    matrix.drawPixel(14, 1, red_color);
    matrix.drawPixel(15, 2, red_color);
    matrix.drawPixel(15, 3, red_color);
    matrix.drawPixel(15, 4, red_color);
    matrix.drawPixel(15, 5, red_color);
    matrix.drawPixel(15, 6, red_color);
    matrix.drawPixel(15, 7, red_color);
    matrix.drawPixel(16, 6, red_color);
}

void print_1_RIGHT() {
    matrix.drawPixel(1, 15, blue_color);
    matrix.drawPixel(1, 17, blue_color);
    matrix.drawPixel(1, 16, blue_color);
    matrix.drawPixel(2, 16, blue_color);
    matrix.drawPixel(3, 16, blue_color);
    matrix.drawPixel(4, 16, blue_color);
    matrix.drawPixel(5, 16, blue_color);
    matrix.drawPixel(6, 16, blue_color);
    matrix.drawPixel(7, 16, blue_color);
    matrix.drawPixel(6, 15, blue_color);
}

void print_1_LEFT() {
    matrix.drawPixel(30, 16, white_color);
    matrix.drawPixel(30, 14, white_color);
    matrix.drawPixel(30, 15, white_color);
    matrix.drawPixel(29, 15, white_color);
    matrix.drawPixel(28, 15, white_color);
    matrix.drawPixel(27, 15, white_color);
    matrix.drawPixel(26, 15, white_color);
    matrix.drawPixel(25, 15, white_color);
    matrix.drawPixel(24, 15, white_color);
    matrix.drawPixel(25, 16, white_color);
}

void print_0_BOT() {
    matrix.drawPixel(16, 1, red_color);
    matrix.drawPixel(15, 1, red_color);
    matrix.drawPixel(14, 1, red_color);
    matrix.drawPixel(16, 7, red_color);
    matrix.drawPixel(15, 7, red_color);
    matrix.drawPixel(14, 7, red_color);
    matrix.drawPixel(17, 2, red_color);
    matrix.drawPixel(17, 3, red_color);
    matrix.drawPixel(17, 4, red_color);
    matrix.drawPixel(17, 5, red_color);
    matrix.drawPixel(17, 6, red_color);
    matrix.drawPixel(13, 2, red_color);
    matrix.drawPixel(13, 3, red_color);
    matrix.drawPixel(13, 4, red_color);
    matrix.drawPixel(13, 5, red_color);
    matrix.drawPixel(13, 6, red_color);
    matrix.drawPixel(16, 3, red_color);
    matrix.drawPixel(15, 4, red_color);
    matrix.drawPixel(14, 5, red_color);
}

void print_0_RIGHT() {
    matrix.drawPixel(1, 15, blue_color);
    matrix.drawPixel(1, 16, blue_color);
    matrix.drawPixel(1, 17, blue_color);
    matrix.drawPixel(7, 15, blue_color);
    matrix.drawPixel(7, 16, blue_color);
    matrix.drawPixel(7, 17, blue_color);
    matrix.drawPixel(2, 18, blue_color);
    matrix.drawPixel(3, 18, blue_color);
    matrix.drawPixel(4, 18, blue_color);
    matrix.drawPixel(5, 18, blue_color);
    matrix.drawPixel(6, 18, blue_color);
    matrix.drawPixel(2, 14, blue_color);
    matrix.drawPixel(3, 14, blue_color);
    matrix.drawPixel(4, 14, blue_color);
    matrix.drawPixel(5, 14, blue_color);
    matrix.drawPixel(6, 14, blue_color);
    matrix.drawPixel(3, 15, blue_color);
    matrix.drawPixel(4, 16, blue_color);
    matrix.drawPixel(5, 17, blue_color);
}

void print_0_LEFT() {
    matrix.drawPixel(30, 14, white_color);
    matrix.drawPixel(30, 15, white_color);
    matrix.drawPixel(30, 16, white_color);
    matrix.drawPixel(24, 14, white_color);
    matrix.drawPixel(24, 15, white_color);
    matrix.drawPixel(24, 16, white_color);
    matrix.drawPixel(29, 17, white_color);
    matrix.drawPixel(28, 17, white_color);
    matrix.drawPixel(27, 17, white_color);
    matrix.drawPixel(26, 17, white_color);
    matrix.drawPixel(25, 17, white_color);
    matrix.drawPixel(29, 13, white_color);
    matrix.drawPixel(28, 13, white_color);
    matrix.drawPixel(27, 13, white_color);
    matrix.drawPixel(26, 13, white_color);
    matrix.drawPixel(25, 13, white_color);
    matrix.drawPixel(28, 16, white_color);
    matrix.drawPixel(27, 15, white_color);
    matrix.drawPixel(26, 14, white_color);
}

void printScoreTop(int print) {
    unsigned long currentMillis = millis();
    unsigned long endMillis = millis()+howLongToShowScore;

    while (currentMillis <= endMillis) {
        matrix.setCursor(14, 24);
        matrix.setTextSize(1);
        matrix.setTextColor(green_color);
        matrix.print(print);
        currentMillis++;
    }
        clearScoreTop();
}


void quicker() {
    if (ballSpeed > 20) ballSpeed--;
}

boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
    boolean result = false;

    if ((x >= rectX && x <= (rectX + rectWidth)) &&
        (y >= rectY && y <= (rectY + rectHeight))) {
        result = true;
    }
    return result;
}

void moveBall() {
    // if the ball goes offscreen, reverse the direction:
    if (ballX > matrix.width() - 1 || ballX < 0) {
        ballDirectionX = -ballDirectionX;
        quicker();

        if(isPlayerTopDead) {

        } else {
        printScoreTop(top_score--);
            if(top_score == -1) isPlayerTopDead = true;
        }
    }
    if (ballY > matrix.height() - 1 || ballY < 0) {
        ballDirectionY = -ballDirectionY;
        quicker();

        if(isPlayerTopDead) {

        } else {
            printScoreTop(top_score--);
            if(top_score == -1) isPlayerTopDead = true;
        }
    }

    // check if the ball and the paddle occupy the same space on screen
    if (inPaddle(ballX, ballY, bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight)) {
//        ballDirectionX = -ballDirectionX; if uncomment --> ball moves only diagonally all the time
        ballDirectionY = -ballDirectionY;
        quicker();
    }
    if (inPaddle(ballX, ballY, top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight)) {
        ballDirectionY = -ballDirectionY;
        quicker();
    }
    if (inPaddle(ballX, ballY, left_paddleX, left_paddleY, vertical_paddleWidth, vertical_paddleHeight)) {
        ballDirectionX = -ballDirectionX;
        quicker();
    }
    if (inPaddle(ballX, ballY, right_paddleX, right_paddleY, vertical_paddleWidth, vertical_paddleHeight)) {
        ballDirectionX = -ballDirectionX;
        quicker();
    }

    // update the ball's position
    ballX += ballDirectionX;
    ballY += ballDirectionY;

    // erase the ball's previous position
    if (oldBallX != ballX || oldBallY != ballY) {
        matrix.fillRect(oldBallX, oldBallY, ballDiameter, ballDiameter, black_color);
    }

    // draw the ball's current position
    matrix.fillRect(ballX, ballY, ballDiameter, ballDiameter, yellow_color);

    oldBallX = ballX;
    oldBallY = ballY;
}

void loop() {
    int matrixWidth = matrix.width();
    int matrixHeight = matrix.height();

    // update paddles positions
    int curr_bot_paddle_position = analogRead(bot_paddle__poti);
    int curr_top_paddle_position = analogRead(top_paddle_poti);
    int curr_left_paddle_position = analogRead(left_paddle_poti);
    int curr_right_paddle_position = analogRead(right_paddle_poti);

    // lower the 1023 --> paddle moves faster
    bot_paddleX = -1 * map(curr_bot_paddle_position, 0, 1023, 0, matrixWidth - 6) + 26;
    top_paddleX = map(curr_top_paddle_position, 0, 1023, 0, matrixWidth - 6);
    left_paddleY = -1 * map(curr_left_paddle_position, 0, 1023, 0, matrixHeight - 6) + 26;
    right_paddleY = map(curr_right_paddle_position, 0, 1023, 0, matrixHeight - 6);

    if (bot_oldPaddleX != bot_paddleX || bot_oldPaddleY != bot_paddleY) {
        matrix.fillRect(bot_oldPaddleX, bot_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!isPlayerTopDead && (top_oldPaddleX != top_paddleX || top_oldPaddleY != top_paddleY)) {
        matrix.fillRect(top_oldPaddleX, top_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (left_oldPaddleX != left_paddleX || left_oldPaddleY != left_paddleY) {
        matrix.fillRect(left_oldPaddleX, left_oldPaddleY, vertical_paddleWidth, vertical_paddleHeight, black_color);
    }
    if (right_oldPaddleX != right_paddleX || right_oldPaddleY != right_paddleY) {
        matrix.fillRect(right_oldPaddleX, right_oldPaddleY, vertical_paddleWidth, vertical_paddleHeight, black_color);
    }

    matrix.fillRect(bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight, red_color);

    if (!isPlayerTopDead) {
    matrix.fillRect(top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight, green_color);

    }
    matrix.fillRect(left_paddleX, left_paddleY, vertical_paddleWidth, vertical_paddleHeight, white_color);
    matrix.fillRect(right_paddleX, right_paddleY, vertical_paddleWidth, vertical_paddleHeight, blue_color);

    bot_oldPaddleX = bot_paddleX;
    bot_oldPaddleY = bot_paddleY;

    top_oldPaddleX = top_paddleX;
    top_oldPaddleY = top_paddleY;

    left_oldPaddleX = left_paddleX;
    left_oldPaddleY = left_paddleY;

    right_oldPaddleX = right_paddleX;
    right_oldPaddleY = right_paddleY;

    if (millis() % (ballSpeed / 2) < 2) {
        moveBall();
    }
}

