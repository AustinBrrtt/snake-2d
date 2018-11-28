#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include "snake.h"
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 4

int main() {
	init_curses();
	
	Vec2D target = {
		width / 2,
		height / 2
	};
	
	srand(time(NULL));
	
	food = (Vec2D){
		rand() % width,
		rand() % height
	};
	
	Snake snake = start(target);
	int dir = rand() % 4;
	
	while (1) {
		switch (getch()) {
			case 'w':
			case 0x103:
				dir = UP;
			case 'a':
			case 0x104:
				dir = LEFT;
			case 's':
			case 0x102:
				dir = DOWN;
			case 'd':
			case 0x105:
				dir = RIGHT;
			case 0x1b:
				return 0;
		}
		
		switch (dir) {
			case UP:
				target.y -= 1;
				target.y = target.y > 0 ? target.y : 0;
				break;
			case RIGHT:
				target.x += 1;
				target.x = target.x < width - 1 ? target.x : width - 1;
				break;
			case LEFT:
				target.x -= 1;
				target.x = target.x > 0 ? target.x : 0;
				break;
			case DOWN:
				target.y += 1;
				target.y = target.y < height - 1 ? target.y : height - 1;
		}
		
		if (target.x == food.x && target.y == food.y) {
			snake_eat(&snake, food);
		} else {
			snake_move(&snake, target);
		}
		
		snake_draw(snake);
		sleep(1);
		refresh();
	}
}

Snake start(const Vec2D pos) {
	init_curses();
	
	Snake snake = {
		malloc(sizeof(SnakeNode))
	};
	
	snake.head->pos = pos;
	snake.head->next = NULL;
	
	return snake;
}

void init_curses() {
	tty_mode(0);
	signal(SIGINT, cleanup_curses);
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	color_supported = has_colors();
	
	if (color_supported) {
		start_color();
		init_pair(SUCCESS, COLOR_GREEN, COLOR_BLACK);
		init_pair(NEUTRAL, COLOR_BLUE, COLOR_BLACK);
		init_pair(DANGER, COLOR_RED, COLOR_BLACK);
		init_pair(WARN, COLOR_YELLOW, COLOR_BLACK);
	}
	width = COLS;
	height = LINES;	
}

void cleanup_curses() {
	tty_mode(1);
}

void snake_eat(Snake *snake, const Vec2D food_pos) {
	SnakeNode *head = malloc(sizeof(SnakeNode));
	head->pos = food_pos;
	head->next = snake->head;
	snake->head = head;
	
	food = (Vec2D){
		rand() % width,
		rand() % height
	};
}

void snake_move(Snake *snake, const Vec2D spot) {
	SnakeNode *cur = snake->head;
	Vec2D prev = spot;
	while (cur != NULL) {
		Vec2D temp = cur->pos;
		cur->pos = prev;
		prev = temp;
		cur = cur->next;
	}
}

void snake_draw(const Snake snake) {
	SnakeNode *cur = snake.head;
	while (cur != NULL) {
		move(cur->pos.y, cur->pos.x);
		addch(ACS_CKBOARD);
		
		cur = cur->next;
	}
	move(LINES - 1, 0);
}

// Saves current TTY mode with how = 0, Restores saved mode with how = 1
// (From in class play_again examples)
void tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;

	if ( how == 0 ){
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
		stored = 1;
	}
	else if ( stored ) {
		tcsetattr(0, TCSANOW, &original_mode); 
		fcntl( 0, F_SETFL, original_flags);	
	}
}
