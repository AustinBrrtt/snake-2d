#ifndef SNAKE_H
#define SNAKE_H

#define SUCCESS 0
#define NEUTRAL 1
#define WARN 2
#define DANGER 3

typedef struct {
	int x;
	int y;
} Vec2D;

typedef struct snake_node {
	Vec2D pos;
	struct snake_node *next;
} SnakeNode;

typedef struct {
	SnakeNode *head;
} Snake;

int width;
int height;
int color_supported;
Vec2D food;

Snake start(const Vec2D pos);
void snake_eat(Snake *snake, const Vec2D food);
void snake_move(Snake *snake, const Vec2D spot);
void init_curses();
void snake_draw(const Snake snake);
void tty_mode(int how);
void cleanup_curses();

#endif // SNAKE_H