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
uint16_t violett_color = matrix.Color888(120,4,140);


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

long ballDirectionX;
long ballDirectionY;
long ballX;
long ballY;
long oldBallX = 0;
long oldBallY = 0;

int bot_paddle__poti = A4;
int top_paddle_poti = A6;
int left_paddle_poti = A5;
int right_paddle_poti = A7;

int ballSpeed = 150; //lower numbers are faster
unsigned long howLongToShowScore = 200;// in milliseconds

boolean isPlayerTopDead = false;
boolean isPlayerBotDead = false;
boolean isPlayerLeftDead = false;
boolean isPlayerRightDead = false;

const String PLAYER_TOP = "top";
const String PLAYER_BOT = "bot";
const String PLAYER_LEFT = "left";
const String PLAYER_RIGHT = "right";

unsigned long interval_for_Players = 2;
unsigned long interval_for_Select = 1;

boolean isGameStarted = true; // must be false by default
boolean isShow_2 = false;
boolean isShow_4 = true;
boolean isMode_2_Started = false;
boolean isMode_4_Started= true;
boolean isRoundStarted = false;
boolean isBallSet = false;

void clear() {
    matrix.fillScreen(black_color);
}

void clearScoreTop() {
    matrix.fillRect(14, 24, 5, 7, black_color);
}

void clearScoreBot() {
    matrix.fillRect(13, 1, 5, 7, black_color);
}

void clearScoreLeft() {
    matrix.fillRect(24, 13, 7, 6, black_color);
}

void clearScoreRight() {
    matrix.fillRect(1, 14, 7, 5, black_color);
}

void getStartBallPositionFor_2_Bot(){
    //random for bot player
    ballX = random(4, 28);
    ballY = random(2, 5);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ballDirectionX = 1;
    } else if (randDirection == 2) {
        ballDirectionX = -1;
    }
    ballDirectionY = 1;
}

void getStartBallPositionFor_2_Top(){

    //random for bot player
    ballX = random(4, 28);
    ballY = random(27, 30);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ballDirectionX = 1;
    } else if (randDirection == 2) {
        ballDirectionX = -1;
    }
    ballDirectionY = -1;
}

void getStartBallPositionFor_2(){
    long rand = random(1,3);

    if (rand == 2) {
        getStartBallPositionFor_2_Bot();
    } else if (rand == 1) {
        getStartBallPositionFor_2_Top();
    }
}

void getStartBallPositionFor_4_Top(){
    //random for bot player
    ballX = random(4, 28);
    ballY = random(27, 30);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ballDirectionX = 1;
    } else if (randDirection == 2) {
        ballDirectionX = -1;
    }
    ballDirectionY = -1;
}
void getStartBallPositionFor_4_Bot(){
    //random for bot player
    ballX = random(4, 28);
    ballY = random(2, 5);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ballDirectionX = 1;
    } else if (randDirection == 2) {
        ballDirectionX = -1;
    }
    ballDirectionY = 1;
}
void getStartBallPositionFor_4_Left(){
    //random for left player
    ballX = random(27, 30);
    ballY = random(4, 28);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ballDirectionY = -1;
    } else if (randDirection == 2) {
        ballDirectionY = 1;
    }
    ballDirectionX = -1;
}
void getStartBallPositionFor_4_Right(){
    //random for right player
    ballX = random(2, 5);
    ballY = random(4, 28);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ballDirectionY = -1;
    } else if (randDirection == 2) {
        ballDirectionY = 1;
    }
    ballDirectionX = 1;
}

void getStartBallPositionFor_4(){
    long rand = random(1,5);

    if (rand == 1) {
        getStartBallPositionFor_4_Bot();
    } else if (rand == 2) {
        getStartBallPositionFor_4_Top();
    } else if (rand == 3) {
        getStartBallPositionFor_4_Left();
    } else if (rand == 4) {
        getStartBallPositionFor_4_Right();
    }
}

void setup() {
    pinMode(bot_paddle__poti, INPUT);
    pinMode(top_paddle_poti, INPUT);
    pinMode(left_paddle_poti, INPUT);
    pinMode(right_paddle_poti, INPUT);
    randomSeed(analogRead(random(A15)));// for random to be random
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

void print_9_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(9);
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

void print_8_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(8);
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

void print_7_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(7);
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

void print_6_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(6);
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

void print_5_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(5);
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

void print_4_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(4);
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

void print_3_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(3);
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

void print_2_RIGHT() {
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

void print_2_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(2);
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

void print_1_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(1);
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

void print_0_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(0);
}

void printWIN(uint16_t color) {

    clear();
    //box
    matrix.drawRect(4, 7, 23, 18, color);

    //u
    matrix.drawLine(6, 21, 6, 17, color);
    matrix.drawLine(6, 17, 10, 17, color);
    matrix.drawLine(10, 17, 10, 21, color);
    //o
    matrix.drawLine(13, 21, 13, 17, color);
    matrix.drawLine(13, 21, 17, 21, color);
    matrix.drawLine(17, 21, 17, 17, color);
    matrix.drawLine(16, 17, 13, 17, color);

    //y
    matrix.drawLine(20, 21, 20, 17, color);
    matrix.drawLine(20, 17, 24, 17, color);
    matrix.drawLine(20, 19, 24, 19, color);
    matrix.drawLine(24, 19, 24, 21, color);

    //w
    matrix.drawLine(20, 14, 20, 10, color);
    matrix.drawLine(20, 10, 24, 10, color);
    matrix.drawLine(24, 10, 24, 14, color);
    matrix.drawLine(22, 10, 22, 12, color);

    //i
    matrix.drawLine(17, 14, 13, 14, color);
    matrix.drawLine(15, 14, 15, 10, color);
    matrix.drawLine(17, 10, 13, 10, color);

    //n
    matrix.drawLine(10, 14, 10, 10, color);
    matrix.drawLine(6, 14, 6, 10, color);
    matrix.drawLine(10, 14, 6, 10, color);

    delay(50000); // game over
}

void printScore(int print, String player) {
    unsigned long currentMillis = millis();
    unsigned long endMillis = millis() + howLongToShowScore;

    if (player == PLAYER_TOP) {
        switch (print) {
            case 0 :
                while (currentMillis <= endMillis) {
                    print_0_TOP();
                    currentMillis++;
                }
                clearScoreTop();
                break;
            case 1 :
                while (currentMillis <= endMillis) {
                    print_1_TOP();
                    currentMillis++;
                }
                clearScoreTop();
                break;
            case 2 :
                while (currentMillis <= endMillis) {
                    print_2_TOP();
                    currentMillis++;
                }
                clearScoreTop();
                break;
            case 3 :
                while (currentMillis <= endMillis) {
                    print_3_TOP();
                    currentMillis++;
                }
                clearScoreTop();
                break;
            case 4 :
                while (currentMillis <= endMillis) {
                    print_4_TOP();
                    currentMillis++;
                }
                clearScoreTop();
                break;
            case 5 :
                while (currentMillis <= endMillis) {
                    print_5_TOP();
                    currentMillis++;
                }
                clearScoreTop();
            case 6 :
                while (currentMillis <= endMillis) {
                    print_6_TOP();
                    currentMillis++;
                }
                clearScoreTop();
            case 7 :
                while (currentMillis <= endMillis) {
                    print_7_TOP();
                    currentMillis++;
                }
                clearScoreTop();
            case 8 :
                while (currentMillis <= endMillis) {
                    print_8_TOP();
                    currentMillis++;
                }
                clearScoreTop();
            case 9 :
                while (currentMillis <= endMillis) {
                    print_9_TOP();
                    currentMillis++;
                }
                clearScoreTop();
        }
    } else if (player == PLAYER_BOT) {
        switch (print) {
            case 0 :
                while (currentMillis <= endMillis) {
                    print_0_BOT();
                    currentMillis++;
                }
                clearScoreBot();
                break;
            case 1 :
                while (currentMillis <= endMillis) {
                    print_1_BOT();
                    currentMillis++;
                }
                clearScoreBot();
                break;
            case 2 :
                while (currentMillis <= endMillis) {
                    print_2_BOT();
                    currentMillis++;
                }
                clearScoreBot();
                break;
            case 3 :
                while (currentMillis <= endMillis) {
                    print_3_BOT();
                    currentMillis++;
                }
                clearScoreBot();
                break;
            case 4 :
                while (currentMillis <= endMillis) {
                    print_4_BOT();
                    currentMillis++;
                }
                clearScoreBot();
                break;
            case 5 :
                while (currentMillis <= endMillis) {
                    print_5_BOT();
                    currentMillis++;
                }
                clearScoreBot();
            case 6 :
                while (currentMillis <= endMillis) {
                    print_6_BOT();
                    currentMillis++;
                }
                clearScoreBot();
            case 7 :
                while (currentMillis <= endMillis) {
                    print_7_BOT();
                    currentMillis++;
                }
                clearScoreBot();
            case 8 :
                while (currentMillis <= endMillis) {
                    print_8_BOT();
                    currentMillis++;
                }
                clearScoreBot();
            case 9 :
                while (currentMillis <= endMillis) {
                    print_9_BOT();
                    currentMillis++;
                }
                clearScoreBot();
        }

    } else if (player == PLAYER_LEFT) {
        switch (print) {
            case 0 :
                while (currentMillis <= endMillis) {
                    print_0_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
                break;
            case 1 :
                while (currentMillis <= endMillis) {
                    print_1_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
                break;
            case 2 :
                while (currentMillis <= endMillis) {
                    print_2_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
                break;
            case 3 :
                while (currentMillis <= endMillis) {
                    print_3_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
                break;
            case 4 :
                while (currentMillis <= endMillis) {
                    print_4_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
                break;
            case 5 :
                while (currentMillis <= endMillis) {
                    print_5_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
            case 6 :
                while (currentMillis <= endMillis) {
                    print_6_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
            case 7 :
                while (currentMillis <= endMillis) {
                    print_7_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
            case 8 :
                while (currentMillis <= endMillis) {
                    print_8_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
            case 9 :
                while (currentMillis <= endMillis) {
                    print_9_LEFT();
                    currentMillis++;
                }
                clearScoreLeft();
        }

    } else if (player == PLAYER_RIGHT) {
        switch (print) {
            case 0 :
                while (currentMillis <= endMillis) {
                    print_0_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
                break;
            case 1 :
                while (currentMillis <= endMillis) {
                    print_1_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
                break;
            case 2 :
                while (currentMillis <= endMillis) {
                    print_2_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
                break;
            case 3 :
                while (currentMillis <= endMillis) {
                    print_3_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
                break;
            case 4 :
                while (currentMillis <= endMillis) {
                    print_4_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
                break;
            case 5 :
                while (currentMillis <= endMillis) {
                    print_5_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
            case 6 :
                while (currentMillis <= endMillis) {
                    print_6_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
            case 7 :
                while (currentMillis <= endMillis) {
                    print_7_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
            case 8 :
                while (currentMillis <= endMillis) {
                    print_8_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
            case 9 :
                while (currentMillis <= endMillis) {
                    print_9_RIGHT();
                    currentMillis++;
                }
                clearScoreRight();
        }
    }
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

void moveBall_Mode_4_Ball_1() {
    // if the ball goes offscreen, reverse the direction:

    if (ballX > matrix.width() - 1) {// *************************goes offscreen for LEFT player
//    if (ballX == 31) {// *************************goes offscreen for LEFT player
        ballDirectionX = -ballDirectionX;
        quicker();

        if (isPlayerLeftDead) {

        } else {
            printScore(left_score--, PLAYER_LEFT);
            if (left_score <= -1) isPlayerLeftDead = true;
        }
    } else if (ballX == 0) { // ***********************************goes offscreen for RIGHT player
        ballDirectionX = -ballDirectionX;
        quicker();

        if (isPlayerRightDead) {

        } else {
            printScore(right_score--, PLAYER_RIGHT);
            if (right_score <= -1) isPlayerRightDead = true;
        }
    }
    if (ballY > matrix.height() - 1) { // ************************goes offscreen for TOP player
//    if (ballY ==31) { // ************************goes offscreen for TOP player
        ballDirectionY = -ballDirectionY;
        quicker();

        if (isPlayerTopDead) {

        } else {
            printScore(top_score--, PLAYER_TOP);
            if (top_score <= -1) isPlayerTopDead = true;
        }
    } else if (ballY == 0) { // **************************************goes offscreen for BOT player
        ballDirectionY = -ballDirectionY;
        quicker();

        if (isPlayerBotDead) {

        } else {
            printScore(bot_score--, PLAYER_BOT);
            if (bot_score <= -1) isPlayerBotDead = true;
        }
    }

    // check if the ball and the paddle occupy the same space on screen
    if (inPaddle(ballX, ballY, bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight)) {
//        ballDirectionX = -ballDirectionX; //if uncomment --> ball moves only diagonally all the time
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

void moveBall_Mode_2_Ball_1() {
    // if the ball goes offscreen, reverse the direction:

    if (ballX == matrix.width() - 1) {// *************************goes offscreen for LEFT player
        ballDirectionX = -ballDirectionX;
        quicker();

    } else if (ballX == 1) { // ***********************************goes offscreen for RIGHT player
        ballDirectionX = -ballDirectionX;
        quicker();
    }
//    if (ballY == matrix.height() - 1) { // ************************goes offscreen for TOP player
    if (ballY > matrix.height() - 2) { // ************************goes offscreen for TOP player
        ballDirectionY = -ballDirectionY;
        quicker();

        if (isPlayerTopDead) {

        } else {
            printScore(top_score--, PLAYER_TOP);
            if (top_score <= -1) isPlayerTopDead = true;
        }
    } else if (ballY == 0) { // **************************************goes offscreen for BOT player
        ballDirectionY = -ballDirectionY;
        quicker();

        if (isPlayerBotDead) {

        } else {
            printScore(bot_score--, PLAYER_BOT);
            if (bot_score <= -1) isPlayerBotDead = true;
        }
    }

    // check if the ball and the paddle occupy the same space on screen
    if (inPaddle(ballX, ballY, bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight)) {
        ballDirectionY = -ballDirectionY;
        quicker();
    }
    if (inPaddle(ballX, ballY, top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight)) {
        ballDirectionY = -ballDirectionY;
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

void printStartScreen_2() {
    matrix.fillRect(11, 15, 10, 3, yellow_color);
    matrix.fillRect(11, 26, 10, 3, yellow_color);
    matrix.fillRect(11, 20, 4, 6, yellow_color);
    matrix.fillRect(17, 24, 4, 2, yellow_color);
    matrix.fillRect(17, 18, 4, 2, yellow_color);
    matrix.fillRect(15, 20, 6, 2, yellow_color);

}

void printStartScreen_4() {
    matrix.fillRect(11, 15, 3, 14, yellow_color);
    matrix.fillRect(18, 21, 3, 8, yellow_color);
    matrix.fillRect(14, 21, 4, 2, yellow_color);
}

void printStartScreen_SELECT() {
    //t
    matrix.drawLine(3, 4, 3, 9, yellow_color);
    matrix.drawLine(1, 9, 5, 9, yellow_color);

    //c
    matrix.drawPixel(7, 8, yellow_color);
    matrix.drawPixel(7, 5, yellow_color);
    matrix.drawLine(8, 9, 10, 9, yellow_color);
    matrix.drawLine(8, 4, 10, 4, yellow_color);
    matrix.drawLine(11, 5, 11, 8, yellow_color);

    //e
    matrix.drawLine(13, 4, 16, 4, yellow_color);
    matrix.drawLine(13, 9, 16, 9, yellow_color);
    matrix.drawLine(14, 7, 16, 7, yellow_color);
    matrix.drawLine(16, 4, 16, 9, yellow_color);

    //l
    matrix.drawLine(18, 4, 20, 4, yellow_color);
    matrix.drawLine(20, 4, 20, 9, yellow_color);

    //e
    matrix.drawLine(22, 4, 25, 4, yellow_color);
    matrix.drawLine(22, 9, 25, 9, yellow_color);
    matrix.drawLine(23, 7, 25, 7, yellow_color);
    matrix.drawLine(25, 4, 25, 9, yellow_color);

    //s
    matrix.drawPixel(27, 8, yellow_color);
    matrix.drawLine(30, 8, 27, 5, yellow_color);
    matrix.drawLine(28, 9, 29, 9, yellow_color);
    matrix.drawLine(28, 4, 29, 4, yellow_color);
    matrix.drawPixel(30, 5, yellow_color);

}

void printStartScreen_PLAYERS() {
    //s
    matrix.drawLine(2, 4, 4, 4, yellow_color);
    matrix.drawLine(2, 4, 2, 6, yellow_color);
    matrix.drawLine(2, 6, 4, 6, yellow_color);
    matrix.drawLine(4, 6, 4, 9, yellow_color);
    matrix.drawLine(4, 9, 2, 9, yellow_color);
    matrix.drawPixel(2, 8, yellow_color);

    //r
    matrix.drawLine(8, 4, 8, 9, yellow_color);
    matrix.drawLine(8, 9, 6, 9, yellow_color);
    matrix.drawLine(6, 9, 6, 7, yellow_color);
    matrix.drawLine(6, 5, 6, 4, yellow_color);
    matrix.drawPixel(7, 6, yellow_color);

    //e
    matrix.drawLine(12, 4, 12, 9, yellow_color);
    matrix.drawLine(10, 4, 12, 4, yellow_color);
    matrix.drawPixel(11, 6, yellow_color);
    matrix.drawLine(10, 9, 12, 9, yellow_color);

    //y
    matrix.drawLine(14, 4, 14, 9, yellow_color);
    matrix.drawLine(14, 4, 14, 9, yellow_color);
    matrix.drawPixel(15, 7, yellow_color);
    matrix.drawLine(16, 9, 16, 7, yellow_color);
    matrix.drawLine(14, 4, 16, 4, yellow_color);

    //a
    matrix.drawLine(18, 4, 18, 9, yellow_color);
    matrix.drawLine(20, 4, 20, 9, yellow_color);
    matrix.drawPixel(19, 9, yellow_color);
    matrix.drawPixel(19, 6, yellow_color);

    //l
    matrix.drawLine(24, 4, 24, 9, yellow_color);
    matrix.drawLine(22, 4, 24, 4, yellow_color);

    //p
    matrix.drawLine(28, 4, 28, 9, yellow_color);
    matrix.drawLine(26, 9, 26, 6, yellow_color);
    matrix.drawPixel(27, 6, yellow_color);
    matrix.drawPixel(27, 9, yellow_color);
}

void startScreen_printRedPointer(){
    matrix.drawLine(18, 2, 14, 2, red_color);
    matrix.drawLine(17, 1, 15, 1, red_color);
    matrix.drawPixel(16, 0, red_color);
}
void startScreen_printGreenPointer(){
    matrix.drawLine(18, 29, 14, 29, green_color);
    matrix.drawLine(17, 30, 15, 30, green_color);
    matrix.drawPixel(16, 31, green_color);
}
void startScreen_printBluePointer(){
    matrix.drawLine(2, 14, 2, 18, blue_color);
    matrix.drawLine(1, 15, 1, 17, blue_color);
    matrix.drawPixel(0, 16, blue_color);
}
void startScreen_printWhitePointer(){
    matrix.drawLine(29, 14, 29, 18, white_color);
    matrix.drawLine(30, 15, 30, 17, white_color);
    matrix.drawPixel(31, 16, white_color);
}

void startScreen_printRedPointer_clear(){
    matrix.drawLine(18, 2, 14, 2, black_color);
    matrix.drawLine(17, 1, 15, 1, black_color);
    matrix.drawPixel(16, 0, black_color);
}
void startScreen_printGreenPointer_clear(){
    matrix.drawLine(18, 29, 14, 29, black_color);
    matrix.drawLine(17, 30, 15, 30, black_color);
    matrix.drawPixel(16, 31, black_color);
}
void startScreen_printBluePointer_clear(){
    matrix.drawLine(2, 14, 2, 18, black_color);
    matrix.drawLine(1, 15, 1, 17, black_color);
    matrix.drawPixel(0, 16, black_color);
}
void startScreen_printWhitePointer_clear(){
    matrix.drawLine(29, 14, 29, 18, black_color);
    matrix.drawLine(30, 15, 30, 17, black_color);
    matrix.drawPixel(31, 16, black_color);
}

void mode_2_Draw_Blank_Players(){
    matrix.drawLine(0, 0, 0, 31, blue_color);
    matrix.drawLine(31, 0, 31, 31, violett_color);
}


void startScreenLogic() {

    // blinking animation for Select/Players
    if ((millis() / 1000) == interval_for_Select) {
        matrix.fillRect(0, 4, 31, 6, black_color);
        printStartScreen_SELECT();
        interval_for_Select += 2;
    } else if ((millis() / 1000) == interval_for_Players) {
        matrix.fillRect(0, 4, 31, 6, black_color);
        printStartScreen_PLAYERS();
        interval_for_Players += 2;
    }

    //changing 2 & 4 digits
    if ((isShow_4 && !isShow_2) && ((1023- analogRead(bot_paddle__poti))/50)>15){
        matrix.fillRect(11, 15, 10, 14, black_color); // clear the number rect

        startScreen_printBluePointer_clear();
        startScreen_printWhitePointer_clear();
        startScreen_printRedPointer();
        startScreen_printGreenPointer();
        printStartScreen_2();
        isShow_4 = false;
        isShow_2 = true;
    } else if((!isShow_4 && isShow_2) && ((1023- analogRead(bot_paddle__poti))/50)<15){
        matrix.fillRect(11, 15, 10, 14, black_color);// clear the number rect

        startScreen_printBluePointer();
        startScreen_printWhitePointer();
        printStartScreen_4();
        isShow_4 = true;
        isShow_2 = false;

    }
}

void mode_2_Players_1_Ball(){

    if (!isBallSet) {
        //generate random ball position
        getStartBallPositionFor_2();
        isBallSet = true;
    }

    mode_2_Draw_Blank_Players();

    if(isPlayerTopDead) { // bot winn
        matrix.setRotation(0);
        printWIN(red_color);
    } else if (isPlayerBotDead) { // top win
        matrix.setRotation(2);
        printWIN(green_color);
    }
    int matrixWidth = matrix.width();
    int matrixHeight = matrix.height();

    int curr_bot_paddle_position = analogRead(bot_paddle__poti);
    int curr_top_paddle_position = analogRead(top_paddle_poti);

    bot_paddleX = -1 * map(curr_bot_paddle_position, 0, 1023, 0, matrixWidth - 6) + 26;
    top_paddleX = map(curr_top_paddle_position, 0, 1023, 0, matrixWidth - 6);

    if (!isPlayerBotDead && (bot_oldPaddleX != bot_paddleX || bot_oldPaddleY != bot_paddleY)) {
        matrix.fillRect(bot_oldPaddleX, bot_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }

    if (!isPlayerTopDead && (top_oldPaddleX != top_paddleX || top_oldPaddleY != top_paddleY)) {
        matrix.fillRect(top_oldPaddleX, top_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }

    if (!isPlayerBotDead) {
        matrix.fillRect(bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight, red_color);
        bot_oldPaddleX = bot_paddleX;
        bot_oldPaddleY = bot_paddleY;
    } else if (isPlayerBotDead) {
        matrix.fillRect(bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!isPlayerTopDead) {
        matrix.fillRect(top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight, green_color);
        top_oldPaddleX = top_paddleX;
        top_oldPaddleY = top_paddleY;
    } else if (isPlayerTopDead) {
        matrix.fillRect(top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }

    //freeze the ball before round starts
    if(!isRoundStarted && millis() > 5000){
        matrix.drawPixel(ballX, ballY, black_color);
        isRoundStarted = true;
    }

    if (isRoundStarted && millis() % (ballSpeed / 2) < 2) {
        moveBall_Mode_2_Ball_1();
    } else if (!isRoundStarted) {
        matrix.drawPixel(ballX, ballY, yellow_color);
    }

}

void mode_4_Players_1_Ball() {
    if (!isBallSet) {
        //generate random ball position
        getStartBallPositionFor_4();
        isBallSet = true;
    }

    if (isPlayerTopDead && isPlayerLeftDead && isPlayerRightDead) { // bot win
        matrix.setRotation(0);
        printWIN(red_color);
    } else if (isPlayerBotDead && isPlayerLeftDead && isPlayerTopDead) { // right win
        matrix.setRotation(3);
        printWIN(blue_color);
    } else if (isPlayerBotDead && isPlayerTopDead && isPlayerRightDead) {// left win
        matrix.setRotation(1);
        printWIN(white_color);
    } else if (isPlayerBotDead && isPlayerLeftDead && isPlayerRightDead) {//top win
        matrix.setRotation(2);
        printWIN(green_color);
    }

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

    //next 8 if statements responsible for paddle moving
    if (!isPlayerBotDead && (bot_oldPaddleX != bot_paddleX || bot_oldPaddleY != bot_paddleY)) {
        matrix.fillRect(bot_oldPaddleX, bot_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!isPlayerTopDead && (top_oldPaddleX != top_paddleX || top_oldPaddleY != top_paddleY)) {
        matrix.fillRect(top_oldPaddleX, top_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!isPlayerLeftDead && (left_oldPaddleX != left_paddleX || left_oldPaddleY != left_paddleY)) {
        matrix.fillRect(left_oldPaddleX, left_oldPaddleY, vertical_paddleWidth, vertical_paddleHeight, black_color);
    }
    if (!isPlayerRightDead && (right_oldPaddleX != right_paddleX || right_oldPaddleY != right_paddleY)) {
        matrix.fillRect(right_oldPaddleX, right_oldPaddleY, vertical_paddleWidth, vertical_paddleHeight,
                        black_color);
    }

    if (!isPlayerBotDead) {
        matrix.fillRect(bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight, red_color);
        bot_oldPaddleX = bot_paddleX;
        bot_oldPaddleY = bot_paddleY;
    } else if (isPlayerBotDead) {
        matrix.fillRect(bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!isPlayerTopDead) {
        matrix.fillRect(top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight, green_color);
        top_oldPaddleX = top_paddleX;
        top_oldPaddleY = top_paddleY;
    } else if (isPlayerTopDead) {
        matrix.fillRect(top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!isPlayerLeftDead) {
        matrix.fillRect(left_paddleX, left_paddleY, vertical_paddleWidth, vertical_paddleHeight, white_color);
        left_oldPaddleX = left_paddleX;
        left_oldPaddleY = left_paddleY;
    } else if (isPlayerLeftDead) {
        matrix.fillRect(left_paddleX, left_paddleY, vertical_paddleWidth, vertical_paddleHeight, black_color);
    }
    if (!isPlayerRightDead) {
        matrix.fillRect(right_paddleX, right_paddleY, vertical_paddleWidth, vertical_paddleHeight, blue_color);
        right_oldPaddleX = right_paddleX;
        right_oldPaddleY = right_paddleY;
    } else if (isPlayerRightDead) {
        matrix.fillRect(right_paddleX, right_paddleY, vertical_paddleWidth, vertical_paddleHeight, black_color);
    }

    // freeze the ball before the round starts
    if(!isRoundStarted && millis() > 5000){
        matrix.drawPixel(ballX, ballY, black_color);
        isRoundStarted = true;
    }

    if (isRoundStarted && millis() % (ballSpeed / 2) < 2) {
        moveBall_Mode_4_Ball_1();
    } else if (!isRoundStarted) {
        matrix.drawPixel(ballX, ballY, yellow_color);
    }
}

void loop() {

    if (!isGameStarted) {
        startScreenLogic();

    } else if (isGameStarted && isMode_2_Started){
        mode_2_Players_1_Ball();

    } else if (isGameStarted && isMode_4_Started) {
        mode_4_Players_1_Ball();
    }
}




































