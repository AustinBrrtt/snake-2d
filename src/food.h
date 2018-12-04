#ifndef FOOD_H
#define FOOD_H

#include "vec2d.h"

typedef struct {
	Vec2D *pos; // Position on screen
	int life; // How long to stick around
} Food;

// Constructor for Food
Food *new_food(int difficulty);

// Randomly chooses location and lifespan for food 
void spawn_food(Food *food, int difficulty);

// Draws food on screen
void draw_food(const Food food);

// Converts int to single digit or + if larger
char get_digit(int i);

// Decrement life and reset if EOL
void update_food(Food *food, int difficulty);

// Returns amount for snake to grow
int eat_food(Food *food, int difficulty);

#endif // FOOD_H