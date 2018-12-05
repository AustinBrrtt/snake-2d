#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake.h"
#include "cursescontroller.h"

Snake *new_snake(Vec2D *pos) {
	Snake *snake = malloc(sizeof(Snake));
	snake->head = malloc(sizeof(SnakeNode));
	
	snake->head->pos = pos;
	snake->head->next = NULL;
	
	snake->direction = rand() % 4;
	snake->growing = 0;
	
	return snake;
}

// Moves snake in direciton it is currently facing and updates food
int slither(Snake *snake, Food *food) {
	Vec2D *next_square = clone_vec2d(*(snake->head->pos));
	
	switch (snake->direction) {
		case UP:
			next_square->y -= 1;
			//next_square->y = next_square->y > 0 ? next_square->y : 0;
			break;
		case RIGHT:
			next_square->x += 1;
			//next_square->x = next_square->x < COLS - 1 ? next_square->x : COLS - 1;
			break;
		case LEFT:
			next_square->x -= 1;
			//next_square->x = next_square->x > 0 ? next_square->x : 0;
			break;
		case DOWN:
			next_square->y += 1;
			//next_square->y = next_square->y < LINES - 1 ? next_square->y : LINES - 1;
	}
	
	if (!check_living(*snake)) {
		return -1;
	}
	
	int result = 0;
	if (vec2d_equals(*next_square, *(food->pos))) {
		snake->growing += eat_food(food, snake_length(*snake) + snake->growing);
		result += 1;
	} else {
		update_food(food, snake_length(*snake) + snake->growing);
	}
	
	// Add head in front of snake
	SnakeNode *new_head = malloc(sizeof(SnakeNode));
	new_head->pos = next_square;
	new_head->next = snake->head;
	snake->head = new_head;
	
	// Remove tail unless snake should grow
	if (snake->growing > 0) {
		snake->growing -= 1;
	} else {
		trim_tail(snake);	
	}
	
	return result;
}

// Returns 1 if alive, 0 if dead
int check_living(const Snake snake) {
	Vec2D head_pos = *(snake.head->pos);
	if(snake.head->pos->x < 1 || snake.head->pos->y < 1 ||
		snake.head->pos->x > COLS - 2 || snake.head->pos->y > LINES - 2) {
			return 0;
		}
	SnakeNode *cur = snake.head->next;
	while (cur != NULL) {
		if (vec2d_equals(head_pos, *(cur->pos))) {
			return 0;
		}
		cur = cur->next;
	}
	return 1;
}

// Removes last piece from tail
void trim_tail(Snake *snake) {
	if (snake->head->next == NULL) {
		abort_game("Snake trimmed to invalid length 0", 1);
	}
	
	// Set cur to the second-to-last node
	SnakeNode *cur = snake->head;
	while (cur->next->next != NULL) {
		cur = cur->next;
	}
	
	// Remove the last node
	cur->next = NULL;
}

// Draws snake to screen
void draw_snake(const Snake snake) {
	SnakeNode *cur = snake.head;
	while (cur != NULL) {
		set_pixel(*(cur->pos), ACS_CKBOARD, GREEN);
		cur = cur->next;
	}
}

// Returns how long the snake is
int snake_length(const Snake snake) {
	int total = 0;
	
	SnakeNode *cur = snake.head;
	while (cur != NULL) {
		total++;
		cur = cur->next;
	}
	
	return total;
}
