#include <stdlib.h>

#include "food.h"
#include "cursescontroller.h"

// Constructor for Food
Food *new_food() {
	Food *food = malloc(sizeof(Food));
	food->pos = malloc(sizeof(Vec2D));
	spawn_food(food);
	return food;
}

// Randomly chooses location and lifespan for food 
void spawn_food(Food *food) {
	// Life is between 1 and 9
	food->life = (rand() % 8 + 1) * TICKS_PER_DECAY;
	
	if (COLS > 0 && LINES > 0) {
		// Pick position on screen (border excluded)
		food->pos->x = rand() % (COLS - 2) + 1;
		food->pos->y = rand() % (LINES - 2) + 1;
	}
	
}

// Draws food on screen
void draw_food(const Food food) {
	set_pixel(*(food.pos), get_digit(food.life / TICKS_PER_DECAY), RED);
}

// Converts int to single digit or + if larger
char get_digit(int i) {
	switch (i % 10) {
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
	}
}

// Decrement life and reset if EOL
void update_food(Food *food) {
	food->life -= 1;
	if (food->life < 0) {
		spawn_food(food);
	}
}

// Returns amount for snake to grow
int eat_food(Food *food) {
	int score = food->life % TICKS_PER_DECAY;
	if (score < 1) {
		score = 1;
	}
	spawn_food(food);
	return score;
}
