#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "cursescontroller.h"

int main() {
	srand(time(NULL));
	width = COLS;
	height = LINES;
	
	food = new_food(1);
	
	snake = new_snake(new_vec2d(width / 2, height / 2));
	
	speed = START_SPEED;

	init_curses(&cleanup);
	loop(&main_loop, speed);	
}

// Cleans up memory
void cleanup() {
	free(snake);
	free(food); // Did somebody say free food?
}

void main_loop() {
	
	switch (getch()) {
		case 'w':
		case 0x103:
			snake->direction = UP;
		case 'a':
		case 0x104:
			snake->direction = LEFT;
		case 's':
		case 0x102:
			snake->direction = DOWN;
		case 'd':
		case 0x105:
			snake->direction = RIGHT;
		case 0x1b:
			end_game(0,snake_length(*snake) + snake->growing);
	}
	
	int result = slither(snake, food);
	int score = snake_length(*snake) + snake->growing;
	if (result == -1) {
		end_game(0, score);
	} else if (score > COLS / 2) {
		end_game(1, score);
	} else {
		clear_and_border();
		// TODO: display score on screen
		draw_snake(*snake);
		draw_food(*food);
		draw();
	}
}

// Ends gameplay
void end_game(int success, int score) {
	abort_game("Not really an error, just game end"); // temporary
}
