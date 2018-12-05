#ifndef GAME_H
#define GAME_H

#include "vec2d.h"
#include "snake.h"

#define START_SPEED 250 // ms per frame
#define SPEED_INCREMENT 10 // ms per frame
#define MAX_SPEED 50 // ms per frame

#define SINGLE_PLAYER 0
#define MP_HOST 1
#define MP_JOIN 2

int width;
int height;
int speed;
int score;
int game_mode;
Snake *snake;
Food *food;

// Starts the game
void start_game(int mode);

// Cleans up memory
void cleanup();

// Main game logic loop
void main_loop();

// Main host game logic loop
void host_loop();

// Main client game logic loop
void client_loop();

// Draw the score in the upper right
void draw_score();

// Increases game speed
void speed_up();

// Ends gameplay
void end_game(int success, int score);

#endif // GAME_H