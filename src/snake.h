#ifndef SNAKE_H
#define SNAKE_H

#include "vec2d.h"
#include "food.h"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 4

typedef struct snake_node {
	Vec2D *pos;
	struct snake_node *next;
} SnakeNode;

typedef struct {
	SnakeNode *head;
	int direction;
	int growing;
} Snake;

// Creates new snake
Snake *new_snake(Vec2D *pos);

// Moves snake in direciton it is currently facing and updates food
// Returns result of check_living after move
int slither(Snake *snake, Food *food);

// Returns 1 if grown, -1 if dead, 0 otherwise
int check_living(const Snake snake);

// Cuts off last piece of snake tail
void trim_tail(Snake *snake);

// Draws snake to screen
void draw_snake(const Snake snake);

// Returns how long the snake is
int snake_length(const Snake snake);

#endif // SNAKE_H