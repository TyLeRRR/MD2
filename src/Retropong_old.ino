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

#define CLK  8 // MUST be on PORTB!
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
uint16_t black = matrix.Color444(0, 0, 0);
//uint16_t white = matrix.Color444(15, 15, 15);
uint16_t white = matrix.Color444(0, 0, 255);
uint16_t yellow = matrix.Color444(15, 15, 0);

int paddleWidth=6;
int paddleHeight=1;

int ballDiameter=1;
int paddleX = 0;
int paddleY = 0;
int oldPaddleX, oldPaddleY;
int ballDirectionX = 1;
int ballDirectionY = 1;

int ballSpeed = 200; //lower numbers are faster

int ballX, ballY, oldBallX, oldBallY;

void clear() {
    matrix.fillScreen(black);
}

void setup() {
    matrix.begin();
    clear();
}

void quicker(){
    if (ballSpeed>20) ballSpeed--;
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
    if (ballX > matrix.width()-1 || ballX < 0) {
        ballDirectionX = -ballDirectionX;
        quicker();
    }

    if (ballY > matrix.height()-1 || ballY < 0) {
        ballDirectionY = -ballDirectionY;
        quicker();
    }

    // check if the ball and the paddle occupy the same space on screen
    if (inPaddle(ballX, ballY, paddleX, paddleY, paddleWidth, paddleHeight)) {
        //    ballDirectionX = -ballDirectionX;
        ballDirectionY = -ballDirectionY;
        quicker();
    }

    // update the ball's position
    ballX += ballDirectionX;
    ballY += ballDirectionY;

    // erase the ball's previous position
    if (oldBallX != ballX || oldBallY != ballY) {
        matrix.fillRect(oldBallX, oldBallY, ballDiameter, ballDiameter,black);
    }

    // draw the ball's current position
    matrix.fillRect(ballX, ballY, ballDiameter, ballDiameter,white);

    oldBallX = ballX;
    oldBallY = ballY;
}

void loop() {
    int myWidth = matrix.width();
    int myHeight = matrix.height();
    int a0= analogRead(A4);
//    int a1= analogRead(A5);

    paddleX = map(a0, 0, 1023, 0, myWidth)- paddleWidth/2 ;
//    paddleY = map(a1, 0, 1023, 0, myHeight)-paddleHeight/2 ;

    if (oldPaddleX != paddleX || oldPaddleY != paddleY) {
        matrix.fillRect(oldPaddleX, oldPaddleY, paddleWidth, paddleHeight,black);
    }
    matrix.fillRect(paddleX, paddleY, paddleWidth, paddleHeight,white);

    oldPaddleX = paddleX;
    oldPaddleY = paddleY;
    if (millis() % (ballSpeed/2) < 2) {
        moveBall();
    }
}







































































































































//#include <HardwareSerial.h>
//#include <HID.h>
//
//#include <Adafruit_GFX.h>   // Core graphics library
//#include <RGBmatrixPanel.h>// Hardware-specific library
//#include <SPI.h>
//
//// If your 32x32 matrix has the SINGLE HEADER input,
//// use this pinout:
//#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
//#define OE  9
//#define LAT 10
////#define temp_A   DD2
////#define temp_B   DD3
////#define temp_C   DD4
////#define temp_D   DD5
//#define temp_A   A0
//#define temp_B   A1
//#define temp_C   A2
//#define temp_D   A3
//#define LEFT_BOT_PLAYER A4
//#define WHITE_COLOR 0xFFFF // White
//#define BLACK_COLOR 0x0000  // Black
//
//int board[32][32] = {
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
//};
//
//int playerPosition;
//
//RGBmatrixPanel matrix(temp_A, temp_B, temp_C, temp_D, CLK, LAT, OE, false);
//
// //const int buttonSelect = 12;
////const int buttonPause = 11;
////int myColor = 0;
////int redColor = 0;
////int greenColor = 0;
////int blueColor = 0;
////int pixelX = 0;
////int lineY = 0;
////int myDelay = 5;
//
//void setup() {
//    pinMode(LEFT_BOT_PLAYER, INPUT);
////    Serial.begin(9600);
////    delay(2000);
//    matrix.begin();
//
//}
//
//void loop() {
////    matrix.drawPixel(0, 31, 0xFFFF);
//    //this *2 defines the speed of the bar. The higher --> the faster
//    playerPosition = (1023 - (analogRead(LEFT_BOT_PLAYER)*4)) / 50;
//
//    for (int pixelX = 0; pixelX < 32; pixelX++) {
////        if (pixelX >= playerPosition && pixelX < playerPosition+6) {
//        if (pixelX >= playerPosition) {
////            matrix.drawPixel(31, pixelX, WHITE_COLOR);
//            matrix.drawFastHLine(0, pixelX, 6, WHITE_COLOR);
//        } else {
////            matrix.drawPixel(31, pixelX, BLACK_COLOR);
//            matrix.drawFastHLine(0, pixelX, 6, BLACK_COLOR);
//        }
//    }
//}
//
//
//
//
////        matrix.drawFastHLine( 0, 31, 6, BACKCOLOR);
////        delay(50);  // increase to make the line move slower.  Decrease to move faster
////        matrix.drawFastHLine( 0, 31, 6, PIXELCOLOR);
//
////        delay((1023 - analogRead(LEFT_BOT_PLAYER)) / 50);
//
////    playerPosition = analogRead(LEFT_BOT_PLAYER) * 6 / 1024;
////
////    for(int i = 0; i < 32; i++){
////        if(i >= playerPosition && i < playerPosition + 6){
////            shape[31][i] = 1;
////        }else{
////            shape[31][i] = 0;
////        }
////    }
