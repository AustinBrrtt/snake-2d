#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "cursescontroller.h"

int main() {
	srand(time(NULL));
	
	// Has to be before getting width and height or COLS and LINES will be 0
	init_curses(cleanup);
	
	width = COLS;
	height = LINES;
	
	food = new_food(1);
	
	snake = new_snake(new_vec2d(width / 2, height / 2));
	
	speed = START_SPEED;
	
	loop(main_loop, speed);
	
	while(1);
}

// Cleans up memory
void cleanup() {
	free(snake);
	free(food); // Did somebody say free food?
}

void main_loop() {
	if (COLS == 0 || LINES == 0) {
		return;
	}
	
	switch (getch()) {
		case 'w':
		case 0x103:
			snake->direction = UP;
			break;
		case 'a':
		case 0x104:
			snake->direction = LEFT;
			break;
		case 's':
		case 0x102:
			snake->direction = DOWN;
			break;
		case 'd':
		case 0x105:
			snake->direction = RIGHT;
			break;
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
	char *status = "died";
	if (success) {
		status = "won";
	}
	char buffer[200];
	sprintf(buffer, "the game is over. You %s, and your score is %d.", success, score);
	abort_game(buffer); // temporary
}
