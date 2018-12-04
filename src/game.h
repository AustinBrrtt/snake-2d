#ifndef GAME_H
#define GAME_H

#include "vec2d.h"
#include "snake.h"

#define START_SPEED 250 // ms per frame
#define SPEED_INCREMENT 10 // ms per frame
#define MAX_SPEED 50 // ms per frame

int width;
int height;
int speed;
Snake *snake;
Food *food;

// Cleans up memory
void cleanup();

// Main game logic loop
void main_loop();

// Increases game speed
void speed_up();

// Ends gameplay
void end_game(int success, int score);

#endif // GAME_H