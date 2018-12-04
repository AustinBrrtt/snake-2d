#include <stdlib.h>

#include "food.h"
#include "cursescontroller.h"

// Constructor for Food
Food *new_food(int difficulty) {
	Food *food = malloc(sizeof(Food));
	food->pos = malloc(sizeof(Vec2D));
	spawn_food(food, difficulty);
	return food;
}

// Randomly chooses location and lifespan for food 
void spawn_food(Food *food, int difficulty) {
	// Life is between (9 - difficulty) and 9
	food->life = rand() % difficulty + 9 - difficulty;
	
	// Pick position on screen
	food->pos->x = rand() % COLS;
	food->pos->y = rand() % LINES;
}

// Draws food on screen
void draw_food(const Food food) {
	set_pixel(*(food.pos), get_digit(food.life), RED);
}

// Converts int to single digit or + if larger
char get_digit(int i) {
	switch (i) {
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 4: return '3';
		case 5: return '4';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		default: return '+';
	}
}

// Decrement life and reset if EOL
void update_food(Food *food, int difficulty) {
	food->life -= 1;
	if (food->life < 0) {
		spawn_food(food, difficulty);
	}
}

// Returns amount for snake to grow
int eat_food(Food *food, int difficulty) {
	int score = food->life;
	spawn_food(food, difficulty);
	return score;
}
