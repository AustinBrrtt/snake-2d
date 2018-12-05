#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "game.h"
#include "menu.h"
#include "cursescontroller.h"

int main() {
	srand(time(NULL));
	
	// Has to be before getting width and height or COLS and LINES will be 0
	init_curses(cleanup);
	
	width = COLS;
	height = LINES;
	score = 0;
	
	food = new_food(1);
	
	snake = new_snake(new_vec2d(width / 2, height / 2));
	
	speed = START_SPEED;
	
	//main_menu();
	start_game(SINGLE_PLAYER);
	while(1);
}

void start_game(int mode) {
	loop(main_loop, START_SPEED);
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
	
	int old_length = snake_length(*snake);
	score = old_length + snake->growing;
	
	switch (getch()) {
		case 'w':
		case 0x103:
			if (snake->direction == DOWN) {
				end_game(0,score);
			}
			snake->direction = UP;
			break;
		case 'a':
		case 0x104:
			if (snake->direction == RIGHT) {
				end_game(0,score);
			}
			snake->direction = LEFT;
			break;
		case 's':
		case 0x102:
			if (snake->direction == UP) {
				end_game(0,score);
			}
			snake->direction = DOWN;
			break;
		case 'd':
		case 0x105:
			if (snake->direction == LEFT) {
				end_game(0,score);
			}
			snake->direction = RIGHT;
			break;
		case 0x1b:
			end_game(0, score);
	}
	
	// Update snake and food
	int result = slither(snake, food);
	
	// Update score
	score = snake_length(*snake) + snake->growing;
	if (result == -1) {
		end_game(0, score);
	} else if (score > COLS / 2) {
		end_game(1, score);
	} else {
		if (!drawing) {
			clear_and_border();
			move(0,COLS - 3);
			draw_score();
			draw_snake(*snake);
			draw_food(*food);
			draw();
		}
		if (snake_length(*snake) - old_length) {
			speed_up();
		}
	}
}

void draw_score() {
	if (color_supported) {
		attron(COLOR_PAIR(YELLOW));
	}
	int a = score;
	int x = COLS - 2;
	while (a > 0) {
		move(0, x--);
		addch(get_digit(a));
		a /= 10;
	}
	if (color_supported) {
		attroff(COLOR_PAIR(YELLOW));
	}
}

void speed_up() {
	speed -= SPEED_INCREMENT;
	if (speed < MAX_SPEED) {
		speed = MAX_SPEED;
	}
	stop_timer();
	start_timer(speed);
}

// Ends gameplay
void end_game(int success, int score) {
	char *status = "died";
	if (success) {
		strcpy(status, "won");
	}
	char buffer[200];
	sprintf(buffer, "You %s!\nScore: %d.", status, score);
	abort_game("", 0); // temporary
}
