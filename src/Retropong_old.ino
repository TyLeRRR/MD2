#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>
#include <SPI.h>
#include <../lib/avdweb_AnalogReadFast-1.0.0/avdweb_AnalogReadFast.h>
#include <../lib/SoftReset.h>

#define CLK  11
#define LAT 10
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

uint16_t black_color = matrix.Color444(0, 0, 0);
uint16_t blue_color = matrix.Color444(0, 0, 255);
uint16_t red_color = matrix.Color444(255, 0, 0);
uint16_t green_color = matrix.Color444(0, 180, 0);
uint16_t white_color = matrix.Color444(255, 255, 255);
uint16_t yellow_color = matrix.Color444(201, 171, 0);
uint16_t violett_color = matrix.Color888(120, 4, 140);

int horiz_paddleWidth = 6;
int horiz_paddleHeight = 1;

int vertical_paddleWidth = 1;
int vertical_paddleHeight = 6;

int ballDiameter = 1;

// paddle BOT position
int bot_paddleX = 0;
int bot_paddleY = 0;
int bot_oldPaddleX, bot_oldPaddleY;

// paddle TOP position
int top_paddleX = 0;
int top_paddleY = 31;
int top_oldPaddleX, top_oldPaddleY;

// paddle LEFT position
int left_paddleX = 31;
int left_paddleY = 16;
int left_oldPaddleX, left_oldPaddleY;

// paddle RIGHT position
int right_paddleX = 0;
int right_paddleY = 16;
int right_oldPaddleX, right_oldPaddleY;

// players score
int bot_score = 9;
int top_score = 9;
int left_score = 9;
int right_score = 9;

long ball_direction_X;
long ball_direction_Y;
long ball_X;
long ball_Y;
long oldBall_X = 0;
long oldBall_Y = 0;

//potis read
int bot_paddle__poti = A4;
int left_paddle_poti = A5;
int top_paddle_poti = A6;
int right_paddle_poti = A7;

//button read
//int button_select = 41;
//int buttonState = 0;

int ball_speed = 150; //lower numbers are faster
unsigned long how_long_show_score_each_player = 200;// in milliseconds

boolean is_player_TOP_dead = false;
boolean is_player_bot_dead = false;
boolean is_player_left_dead = false;
boolean is_player_right_dead = false;

const String PLAYER_TOP = "top";
const String PLAYER_BOT = "bot";
const String PLAYER_LEFT = "left";
const String PLAYER_RIGHT = "right";

unsigned long interval_for_showing_Players = 2;
unsigned long interval_for_Select = 1;

/**
 * @TOM - important here!!
 *
 * If you wanna start directly 2 player mode set -->
 *      is_game_started = true
 *      is_mode_2_started = true;
 *      is_mode_4_started = false;
 *
 * If you wanna start directly 4 player mode set -->
 *       is_game_started = true
 *       is_mode_2_started = false
 *       is_mode_4_started = true;
 *
 * If you wanna start directly start screen set -->
 *      is_game_started = false
 *       is_mode_2_started = false;
 *       is_mode_4_started = false;
 *
 * But keep ALWAYS is_Show_4 = true and is_Show_2=false
 *
 */
boolean is_game_started = false; // must be false by default
boolean is_show_2 = false;
boolean is_show_4 = true;
boolean is_mode_2_started = false;
boolean is_mode_4_started = false;
boolean is_round_started = false;
boolean is_ball_set = false;

int matrix_width = 32;
int matrix_height = 32;

void clear_screen() {
    matrix.fillScreen(black_color);

}

void clear_score_top() {
    matrix.fillRect(14, 24, 5, 7, black_color);
}

void clear_score_bot() {
    matrix.fillRect(13, 1, 5, 7, black_color);
}

void clear_score_left() {
    matrix.fillRect(24, 13, 7, 6, black_color);
}

void clear_score_right() {
    matrix.fillRect(1, 14, 7, 5, black_color);
}

void get_start_ball_position_for_2_bot() {
    //random for bot player
    ball_X = random(4, 28);
    ball_Y = random(2, 5);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ball_direction_X = 1;
    } else if (randDirection == 2) {
        ball_direction_X = -1;
    }
    ball_direction_Y = 1;
}

void get_start_ball_position_for_2_top() {

    //random for top player
    ball_X = random(4, 28);
    ball_Y = random(27, 30);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ball_direction_X = 1;
    } else if (randDirection == 2) {
        ball_direction_X = -1;
    }
    ball_direction_Y = -1;
}

void get_start_ball_position_for_2_mode() {
    long rand = random(1, 3);

    if (rand == 2) {
        get_start_ball_position_for_2_bot();
    } else if (rand == 1) {
        get_start_ball_position_for_2_top();
    }
}

void get_start_ball_position_for_4_top() {
    //random for top player
    ball_X = random(4, 28);
    ball_Y = random(27, 30);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ball_direction_X = 1;
    } else if (randDirection == 2) {
        ball_direction_X = -1;
    }
    ball_direction_Y = -1;
}

void get_start_ball_position_for_4_bot() {
    //random for bot player
    ball_X = random(4, 28);
    ball_Y = random(2, 5);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ball_direction_X = 1;
    } else if (randDirection == 2) {
        ball_direction_X = -1;
    }
    ball_direction_Y = 1;
}

void get_start_ball_position_for_4_left() {
    //random for left player
    ball_X = random(27, 30);
    ball_Y = random(4, 28);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ball_direction_Y = -1;
    } else if (randDirection == 2) {
        ball_direction_Y = 1;
    }
    ball_direction_X = -1;
}

void get_start_ball_position_for_4_right() {
    //random for right player
    ball_X = random(2, 5);
    ball_Y = random(4, 28);

    long randDirection = random(1, 3);

    if (randDirection == 1) {
        ball_direction_Y = -1;
    } else if (randDirection == 2) {
        ball_direction_Y = 1;
    }
    ball_direction_X = 1;
}

void get_start_ball_position_for_4_mode() {
    long rand = random(1, 5);

    if (rand == 1) {
        get_start_ball_position_for_4_bot();
    } else if (rand == 2) {
        get_start_ball_position_for_4_top();
    } else if (rand == 3) {
        get_start_ball_position_for_4_left();
    } else if (rand == 4) {
        get_start_ball_position_for_4_right();
    }
}

void setup() {
    pinMode(bot_paddle__poti, INPUT);
    pinMode(top_paddle_poti, INPUT);
    pinMode(left_paddle_poti, INPUT);
    pinMode(right_paddle_poti, INPUT);

//    pinMode(button_select, INPUT); // button setup

    randomSeed(analogRead(random(A15)));// for random to be really random. Otherwise some shit result is returned
    matrix.begin();

    clear_screen();
}

void print_9_score_BOT() {
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

void print_9_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(9);
}

void print_9_score_RIGHT() {
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

void print_9_score_LEFT() {
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

void print_8_score_BOT() {
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

void print_8_score_RIGHT() {
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

void print_8_score_LEFT() {
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

void print_8_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(8);
}

void print_7_score_BOT() {
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

void print_7_score_RIGHT() {
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

void print_7_score_LEFT() {
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

void print_7_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(7);
}

void print_6_score_BOT() {
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

void print_6_score_RIGHT() {
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

void print_6_score_LEFT() {
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

void print_6_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(6);
}

void print_5_score_BOT() {
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

void print_5_score_RIGHT() {
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

void print_5_score_LEFT() {
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

void print_5_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(5);
}

void print_4_score_BOT() {
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

void print_4_score_RIGHT() {
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

void print_4_score_LEFT() {
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

void print_4_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(4);
}

void print_3_score_BOT() {
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

void print_3_score_RIGHT() {
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

void print_3_score_LEFT() {
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

void print_3_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(3);
}

void print_2_score_BOT() {
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

void print_2_score_RIGHT() {
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

void print_2_score_LEFT() {
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

void print_2_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(2);
}

void print_1_score_BOT() {
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

void print_1_score_RIGHT() {
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

void print_1_score_LEFT() {
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

void print_1_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(1);
}

void print_0_score_BOT() {
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

void print_0_score_RIGHT() {
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

void print_0_score_LEFT() {
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

void print_0_score_TOP() {
    matrix.setCursor(14, 24);
    matrix.setTextSize(1);
    matrix.setTextColor(green_color);
    matrix.print(0);
}

void print_WIN(uint16_t color) {

    clear_screen();

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

    delay(5000); // game over
}

void print_score_for_player(int print, String player) {

    unsigned long currentMillis = millis();
    unsigned long endMillis = millis() + how_long_show_score_each_player;

    if (player == PLAYER_TOP) {
        switch (print) {
            case 0 :
                while (currentMillis <= endMillis) {
                    print_0_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
                break;
            case 1 :
                while (currentMillis <= endMillis) {
                    print_1_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
                break;
            case 2 :
                while (currentMillis <= endMillis) {
                    print_2_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
                break;
            case 3 :
                while (currentMillis <= endMillis) {
                    print_3_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
                break;
            case 4 :
                while (currentMillis <= endMillis) {
                    print_4_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
                break;
            case 5 :
                while (currentMillis <= endMillis) {
                    print_5_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
            case 6 :
                while (currentMillis <= endMillis) {
                    print_6_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
            case 7 :
                while (currentMillis <= endMillis) {
                    print_7_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
            case 8 :
                while (currentMillis <= endMillis) {
                    print_8_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
            case 9 :
                while (currentMillis <= endMillis) {
                    print_9_score_TOP();
                    currentMillis++;
                }
                clear_score_top();
        }
    } else if (player == PLAYER_BOT) {
        switch (print) {
            case 0 :
                while (currentMillis <= endMillis) {
                    print_0_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
                break;
            case 1 :
                while (currentMillis <= endMillis) {
                    print_1_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
                break;
            case 2 :
                while (currentMillis <= endMillis) {
                    print_2_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
                break;
            case 3 :
                while (currentMillis <= endMillis) {
                    print_3_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
                break;
            case 4 :
                while (currentMillis <= endMillis) {
                    print_4_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
                break;
            case 5 :
                while (currentMillis <= endMillis) {
                    print_5_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
            case 6 :
                while (currentMillis <= endMillis) {
                    print_6_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
            case 7 :
                while (currentMillis <= endMillis) {
                    print_7_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
            case 8 :
                while (currentMillis <= endMillis) {
                    print_8_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
            case 9 :
                while (currentMillis <= endMillis) {
                    print_9_score_BOT();
                    currentMillis++;
                }
                clear_score_bot();
        }

    } else if (player == PLAYER_LEFT) {
        switch (print) {
            case 0 :
                while (currentMillis <= endMillis) {
                    print_0_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
                break;
            case 1 :
                while (currentMillis <= endMillis) {
                    print_1_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
                break;
            case 2 :
                while (currentMillis <= endMillis) {
                    print_2_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
                break;
            case 3 :
                while (currentMillis <= endMillis) {
                    print_3_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
                break;
            case 4 :
                while (currentMillis <= endMillis) {
                    print_4_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
                break;
            case 5 :
                while (currentMillis <= endMillis) {
                    print_5_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
            case 6 :
                while (currentMillis <= endMillis) {
                    print_6_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
            case 7 :
                while (currentMillis <= endMillis) {
                    print_7_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
            case 8 :
                while (currentMillis <= endMillis) {
                    print_8_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
            case 9 :
                while (currentMillis <= endMillis) {
                    print_9_score_LEFT();
                    currentMillis++;
                }
                clear_score_left();
        }

    } else if (player == PLAYER_RIGHT) {
        switch (print) {
            case 0 :
                while (currentMillis <= endMillis) {
                    print_0_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
                break;
            case 1 :
                while (currentMillis <= endMillis) {
                    print_1_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
                break;
            case 2 :
                while (currentMillis <= endMillis) {
                    print_2_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
                break;
            case 3 :
                while (currentMillis <= endMillis) {
                    print_3_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
                break;
            case 4 :
                while (currentMillis <= endMillis) {
                    print_4_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
                break;
            case 5 :
                while (currentMillis <= endMillis) {
                    print_5_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
            case 6 :
                while (currentMillis <= endMillis) {
                    print_6_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
            case 7 :
                while (currentMillis <= endMillis) {
                    print_7_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
            case 8 :
                while (currentMillis <= endMillis) {
                    print_8_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
            case 9 :
                while (currentMillis <= endMillis) {
                    print_9_score_RIGHT();
                    currentMillis++;
                }
                clear_score_right();
        }
    }
}

void quicker() {
    if (ball_speed > 20) ball_speed--;
}

boolean inPaddle_4_mode(int ball_x, int ball_y, int rectX, int rectY, int rectWidth, int rectHeight) {

    if ((ball_y == 30 || ball_y == 1) && ((ball_x >= rectX && ball_x <= (rectX + rectWidth)))) {
        return true;
    } else if ((ball_x == 30 || ball_x == 1) && ((ball_y >= rectY && ball_y <= (rectY + rectHeight)))) {
        return true;
    }

    return false;
}

boolean inPaddle_2_mode(int ball_x, int ball_y, int rectX, int rectY, int rectWidth, int rectHeight) {

    if ((ball_y == 30 || ball_y == 1) && ((ball_x >= rectX && ball_x <= (rectX + rectWidth)))) {
        return true;
    }

    return false;
}

void move_ball_4_mode() {
    // if the ball goes offscreen, reverse the direction:

    if (ball_X == 31) {// *************************goes offscreen for LEFT player
        ball_direction_X = -ball_direction_X;
        quicker();

        if (is_player_left_dead) {
            // do nothing because the paddle dissappers. nothing left to be done
        } else {
            print_score_for_player(left_score--, PLAYER_LEFT);
            if (left_score <= -1) is_player_left_dead = true;
        }
    } else if (ball_X == 0) { // ***********************************goes offscreen for RIGHT player
        ball_direction_X = -ball_direction_X;
        quicker();

        if (is_player_right_dead) {
            // do nothing because the paddle dissappers. nothing left to be done
        } else {
            print_score_for_player(right_score--, PLAYER_RIGHT);
            if (right_score <= -1) is_player_right_dead = true;
        }
    }
    if (ball_Y == 31) { // ************************goes offscreen for TOP player
        ball_direction_Y = -ball_direction_Y;
        quicker();

        if (is_player_TOP_dead) {
            // do nothing because the paddle dissappers. nothing left to be done
        } else {
            print_score_for_player(top_score--, PLAYER_TOP);
            if (top_score <= -1) is_player_TOP_dead = true;
        }
    } else if (ball_Y == 0) { // **************************************goes offscreen for BOT player
        ball_direction_Y = -ball_direction_Y;
        quicker();

        if (is_player_bot_dead) {
            // do nothing because the paddle dissappers. nothing left to be done
        } else {
            print_score_for_player(bot_score--, PLAYER_BOT);
            if (bot_score <= -1) is_player_bot_dead = true;
        }
    }

    // check if the ball and the paddle occupy the same space on screen
    if (inPaddle_4_mode(ball_X, ball_Y, bot_paddleX, bot_paddleY, horiz_paddleWidth,
                        horiz_paddleHeight)) { // check for BOT player
//        ball_direction_X = -ball_direction_X; //if uncomment --> ball moves only diagonally all the time
        ball_direction_Y = -ball_direction_Y;
        quicker();
    }
    if (inPaddle_4_mode(ball_X, ball_Y, top_paddleX, top_paddleY, horiz_paddleWidth,
                        horiz_paddleHeight)) { // check for TOP player
        ball_direction_Y = -ball_direction_Y;
        quicker();
    }
    if (inPaddle_4_mode(ball_X, ball_Y, left_paddleX, left_paddleY, vertical_paddleWidth,
                        vertical_paddleHeight)) { // check for LEFT player
        ball_direction_X = -ball_direction_X;
        quicker();
    }
    if (inPaddle_4_mode(ball_X, ball_Y, right_paddleX, right_paddleY, vertical_paddleWidth,
                        vertical_paddleHeight)) { // check for RIGHT player
        ball_direction_X = -ball_direction_X;
        quicker();
    }

    // update the ball's position
    ball_X += ball_direction_X;
    ball_Y += ball_direction_Y;

    // erase the ball's previous position
    if (oldBall_X != ball_X || oldBall_Y != ball_Y) {
        matrix.fillRect(oldBall_X, oldBall_Y, ballDiameter, ballDiameter, black_color);
    }

    // draw the ball's current position
    matrix.fillRect(ball_X, ball_Y, ballDiameter, ballDiameter, yellow_color);

    oldBall_X = ball_X;
    oldBall_Y = ball_Y;
}

void move_ball_2_mode() {
    // if the ball goes offscreen, reverse the direction:

    if (ball_X == matrix_width - 2) {// *************************goes offscreen for LEFT WALL
        ball_direction_X = -ball_direction_X;
        quicker();

    } else if (ball_X == 1) { // ***********************************goes offscreen for RIGHT WALL
        ball_direction_X = -ball_direction_X;
        quicker();
    }
    if (ball_Y == 31) { // ************************goes offscreen for TOP player
        ball_direction_Y = -ball_direction_Y;
        quicker();

        if (is_player_TOP_dead) {
            // player is dead
        } else {
            print_score_for_player(top_score--, PLAYER_TOP);
            if (top_score <= -1) is_player_TOP_dead = true;
        }
    } else if (ball_Y == 0) { // **************************************goes offscreen for BOT player
        ball_direction_Y = -ball_direction_Y;
        quicker();

        if (is_player_bot_dead) {
            // player is dead
        } else {
            print_score_for_player(bot_score--, PLAYER_BOT);
            if (bot_score <= -1) is_player_bot_dead = true;
        }
    }

    // check if the ball and the paddle occupy the same space on screen
    if (inPaddle_2_mode(ball_X, ball_Y, bot_paddleX, bot_paddleY, horiz_paddleWidth,
                        horiz_paddleHeight)) { // check for BOT player
        ball_direction_Y = -ball_direction_Y;
        quicker();
    }
    if (inPaddle_2_mode(ball_X, ball_Y, top_paddleX, top_paddleY, horiz_paddleWidth,
                        horiz_paddleHeight)) { // check for TOP player
        ball_direction_Y = -ball_direction_Y;
        quicker();
    }

    // update the ball's position
    ball_X += ball_direction_X;
    ball_Y += ball_direction_Y;

    // erase the ball's previous position
    if (oldBall_X != ball_X || oldBall_Y != ball_Y) {
        matrix.fillRect(oldBall_X, oldBall_Y, ballDiameter, ballDiameter, black_color);
    }

    // draw the ball's current position
    matrix.fillRect(ball_X, ball_Y, ballDiameter, ballDiameter, yellow_color);

    oldBall_X = ball_X;
    oldBall_Y = ball_Y;
}

void print_start_screen_2() {
    matrix.fillRect(11, 15, 10, 3, yellow_color);
    matrix.fillRect(11, 26, 10, 3, yellow_color);
    matrix.fillRect(11, 20, 4, 6, yellow_color);
    matrix.fillRect(17, 24, 4, 2, yellow_color);
    matrix.fillRect(17, 18, 4, 2, yellow_color);
    matrix.fillRect(15, 20, 6, 2, yellow_color);

}

void print_start_screen_4() {
    matrix.fillRect(11, 15, 3, 14, yellow_color);
    matrix.fillRect(18, 21, 3, 8, yellow_color);
    matrix.fillRect(14, 21, 4, 2, yellow_color);
}

void print_start_screen_SELECT() {
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

void print_start_screen_PLAYERS() {
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

void print_start_screen_RED_pointer() {
    matrix.drawLine(18, 2, 14, 2, red_color);
    matrix.drawLine(17, 1, 15, 1, red_color);
    matrix.drawPixel(16, 0, red_color);
}

void print_start_screen_GREEN_pointer() {
    matrix.drawLine(18, 29, 14, 29, green_color);
    matrix.drawLine(17, 30, 15, 30, green_color);
    matrix.drawPixel(16, 31, green_color);
}

void print_start_screen_BLUE_pointer() {
    matrix.drawLine(2, 14, 2, 18, blue_color);
    matrix.drawLine(1, 15, 1, 17, blue_color);
    matrix.drawPixel(0, 16, blue_color);
}

void print_start_screen_WHITE_pointer() {
    matrix.drawLine(29, 14, 29, 18, white_color);
    matrix.drawLine(30, 15, 30, 17, white_color);
    matrix.drawPixel(31, 16, white_color);
}

void print_start_screen_RED_pointer_clear() {
    matrix.drawLine(18, 2, 14, 2, black_color);
    matrix.drawLine(17, 1, 15, 1, black_color);
    matrix.drawPixel(16, 0, black_color);
}

void print_start_screen_GREEN_pointer_clear() {
    matrix.drawLine(18, 29, 14, 29, black_color);
    matrix.drawLine(17, 30, 15, 30, black_color);
    matrix.drawPixel(16, 31, black_color);
}

void print_start_screen_BLUE_pointer_clear() {
    matrix.drawLine(2, 14, 2, 18, black_color);
    matrix.drawLine(1, 15, 1, 17, black_color);
    matrix.drawPixel(0, 16, black_color);
}

void print_start_screen_WHITE_pointer_clear() {
    matrix.drawLine(29, 14, 29, 18, black_color);
    matrix.drawLine(30, 15, 30, 17, black_color);
    matrix.drawPixel(31, 16, black_color);
}

void print_walls_2_mode() {
    matrix.drawLine(0, 0, 0, 31, blue_color);
    matrix.drawLine(31, 0, 31, 31, violett_color);
}

void load_start_screen() {
    clear_screen();
    soft_restart();
}


void start_screen_logic() {

//    buttonState = digitalRead(button_select);

    //check if pressed. when pressed it is HIGH
//    if (buttonState == HIGH) {
//        clear_screen();
//        is_game_started = true;
//        is_mode_2_started = true;
//    }

    // blinking animation for Select/Players
    if ((millis() / 1000) == interval_for_Select) {
        matrix.fillRect(0, 4, 31, 6, black_color);
        print_start_screen_SELECT();
        interval_for_Select += 2;
    } else if ((millis() / 1000) == interval_for_showing_Players) {
        matrix.fillRect(0, 4, 31, 6, black_color);
        print_start_screen_PLAYERS();
        interval_for_showing_Players += 2;
    }

    //changing 2 & 4 digits
    if ((is_show_4 && !is_show_2) && ((1023 - analogRead(bot_paddle__poti)) / 50) > 15) {
        matrix.fillRect(11, 15, 10, 14, black_color); // clear_screen the number rect

        print_start_screen_BLUE_pointer_clear();
        print_start_screen_WHITE_pointer_clear();
        print_start_screen_RED_pointer();
        print_start_screen_GREEN_pointer();
        print_start_screen_2();
        is_show_4 = false;
        is_show_2 = true;
    } else if ((!is_show_4 && is_show_2) && ((1023 - analogRead(bot_paddle__poti)) / 50) < 15) {
        matrix.fillRect(11, 15, 10, 14, black_color);// clear_screen the number rect

        print_start_screen_BLUE_pointer();
        print_start_screen_WHITE_pointer();
        print_start_screen_4();
        is_show_4 = true;
        is_show_2 = false;

    }
}

void mode_2_players() {

    if (!is_ball_set) {
        //generate random ball position
        get_start_ball_position_for_2_mode();
        is_ball_set = true;
    }

    print_walls_2_mode();

    if (is_player_TOP_dead) { // bot winn
        matrix.setRotation(0);
        print_WIN(red_color);
        load_start_screen();
    } else if (is_player_bot_dead) { // top win
        matrix.setRotation(2);
        print_WIN(green_color);
        load_start_screen();
    }

    //prewarm potis before reading the values
    analogRead(bot_paddle__poti);
    bot_paddleX = -1 * map(analogRead(bot_paddle__poti), 0, 1023, 0, matrix_width - 6) + 26;

    analogRead(top_paddle_poti);
    top_paddleX = map(analogRead(top_paddle_poti), 0, 1023, 0, matrix_width - 6);

    //next 4 if statements responsible for paddle moving
    if (!is_player_bot_dead && (bot_oldPaddleX != bot_paddleX || bot_oldPaddleY != bot_paddleY)) {
        matrix.fillRect(bot_oldPaddleX, bot_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }

    if (!is_player_TOP_dead && (top_oldPaddleX != top_paddleX || top_oldPaddleY != top_paddleY)) {
        matrix.fillRect(top_oldPaddleX, top_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }

    if (!is_player_bot_dead) {
        matrix.fillRect(bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight, red_color);
        bot_oldPaddleX = bot_paddleX;
        bot_oldPaddleY = bot_paddleY;
    } else if (is_player_bot_dead) {
        matrix.fillRect(bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!is_player_TOP_dead) {
        matrix.fillRect(top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight, green_color);
        top_oldPaddleX = top_paddleX;
        top_oldPaddleY = top_paddleY;
    } else if (is_player_TOP_dead) {
        matrix.fillRect(top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }

    //freeze the ball for 5 sec before round starts
    if (!is_round_started && millis() > 5000) {
        matrix.drawPixel(ball_X, ball_Y, black_color);
        is_round_started = true;
    }

    if (is_round_started
        &&
        millis() % (ball_speed / 5) < 2) {

        move_ball_2_mode();
    } else if (!is_round_started) {
        matrix.drawPixel(ball_X, ball_Y, yellow_color);
    }
}

void mode_4_players() {

    if (!is_ball_set) {
        //generate random ball position
        get_start_ball_position_for_4_mode();
        is_ball_set = true;
    }

    if (is_player_TOP_dead && is_player_left_dead && is_player_right_dead) { // bot win
        matrix.setRotation(0);
        print_WIN(red_color);
    } else if (is_player_bot_dead && is_player_left_dead && is_player_TOP_dead) { // right win
        matrix.setRotation(3);
        print_WIN(blue_color);
    } else if (is_player_bot_dead && is_player_TOP_dead && is_player_right_dead) {// left win
        matrix.setRotation(1);
        print_WIN(white_color);
    } else if (is_player_bot_dead && is_player_left_dead && is_player_right_dead) {//top win
        matrix.setRotation(2);
        print_WIN(green_color);
    }

    // lower the 1023 --> paddle moves faster
    analogReadFast(bot_paddle__poti);
    bot_paddleX = -1 * map(analogReadFast(bot_paddle__poti), 0, 1023, 0, matrix_width - 6) + 26;


    analogReadFast(left_paddle_poti);
    left_paddleY = -1 * map(analogReadFast(left_paddle_poti), 0, 1023, 0, matrix_height - 6) + 26;

    analogReadFast(top_paddle_poti);
    top_paddleX = map(analogReadFast(top_paddle_poti), 0, 1023, 0, matrix_width - 6);

    analogReadFast(right_paddle_poti);
    right_paddleY = map(analogReadFast(right_paddle_poti), 0, 1023, 0, matrix_height - 6);

    //next 8 if statements responsible for paddle moving
    if (!is_player_bot_dead && (bot_oldPaddleX != bot_paddleX || bot_oldPaddleY != bot_paddleY)) {
        matrix.fillRect(bot_oldPaddleX, bot_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!is_player_TOP_dead && (top_oldPaddleX != top_paddleX || top_oldPaddleY != top_paddleY)) {
        matrix.fillRect(top_oldPaddleX, top_oldPaddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!is_player_left_dead && (left_oldPaddleX != left_paddleX || left_oldPaddleY != left_paddleY)) {
        matrix.fillRect(left_oldPaddleX, left_oldPaddleY, vertical_paddleWidth, vertical_paddleHeight, black_color);
    }
    if (!is_player_right_dead && (right_oldPaddleX != right_paddleX || right_oldPaddleY != right_paddleY)) {
        matrix.fillRect(right_oldPaddleX, right_oldPaddleY, vertical_paddleWidth, vertical_paddleHeight,
                        black_color);
    }

    if (!is_player_bot_dead) {
        matrix.fillRect(bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight, red_color);
        bot_oldPaddleX = bot_paddleX;
        bot_oldPaddleY = bot_paddleY;
    } else if (is_player_bot_dead) {
        matrix.fillRect(bot_paddleX, bot_paddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!is_player_TOP_dead) {
        matrix.fillRect(top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight, green_color);
        top_oldPaddleX = top_paddleX;
        top_oldPaddleY = top_paddleY;
    } else if (is_player_TOP_dead) {
        matrix.fillRect(top_paddleX, top_paddleY, horiz_paddleWidth, horiz_paddleHeight, black_color);
    }
    if (!is_player_left_dead) {
        matrix.fillRect(left_paddleX, left_paddleY, vertical_paddleWidth, vertical_paddleHeight, white_color);
        left_oldPaddleX = left_paddleX;
        left_oldPaddleY = left_paddleY;
    } else if (is_player_left_dead) {
        matrix.fillRect(left_paddleX, left_paddleY, vertical_paddleWidth, vertical_paddleHeight, black_color);
    }
    if (!is_player_right_dead) {
        matrix.fillRect(right_paddleX, right_paddleY, vertical_paddleWidth, vertical_paddleHeight, blue_color);
        right_oldPaddleX = right_paddleX;
        right_oldPaddleY = right_paddleY;
    } else if (is_player_right_dead) {
        matrix.fillRect(right_paddleX, right_paddleY, vertical_paddleWidth, vertical_paddleHeight, black_color);
    }

    //freeze the ball for 5 sec before the round starts
    if (!is_round_started && millis() > 5000) {
        matrix.drawPixel(ball_X, ball_Y, black_color);
        is_round_started = true;
    }

    if (is_round_started
        &&
        millis() % (ball_speed / 2) < 2) {

        move_ball_4_mode();
    } else if (!is_round_started) {
        matrix.drawPixel(ball_X, ball_Y, yellow_color);
    }
}

void loop() {
    if (!is_game_started) {
        start_screen_logic();

    } else if (is_game_started && is_mode_2_started) {
        mode_2_players();

    } else if (is_game_started && is_mode_4_started) {
        mode_4_players();
    }
}
