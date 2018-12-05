#ifndef FOOD_H
#define FOOD_H

#include "vec2d.h"

#define TICKS_PER_DECAY 10 // how many frames until food decays by 1

typedef struct {
	Vec2D *pos; // Position on screen
	int life; // How many frames to stick around
} Food;

// Constructor for Food
Food *new_food();

// Randomly chooses location and lifespan for food 
void spawn_food(Food *food);

// Draws food on screen
void draw_food(const Food food);

// Converts int to single digit or + if larger
char get_digit(int i);

// Decrement life and reset if EOL
void update_food(Food *food);

// Returns amount for snake to grow
int eat_food(Food *food);

#endif // FOOD_H
