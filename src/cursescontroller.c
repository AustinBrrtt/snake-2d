#include "cursescontroller.h"
#include "food.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>

// Sets up terminal for curses and sets capability flags
void init_curses(void (*cleanup)()) {
	tty_mode(0);
	game_cleanup = cleanup;
	signal(SIGINT, cleanup_curses);
	signal(SIGSEGV, cleanup_curses);
	initscr();
	cbreak();
	set_cr_noecho_mode();
	set_nodelay_mode();
	keypad(stdscr, TRUE);
	curs_set(0);
	
	color_supported = has_colors();
	
	if (color_supported) {
		start_color();
		init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
		init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
		init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
		init_pair(RED, COLOR_RED, COLOR_BLACK);
		init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
	}
	
	drawing = 0;
}

// Restores terminal to normal state
void cleanup_curses(int signum) {
	stop_timer();
	
	if (game_cleanup) {
		game_cleanup();
	}
	
	clear();
	refresh();
	tty_mode(1);
	exit(0);
}

// Cleans up and kills process
void abort_game(const char *message, int status) {
	cleanup_curses(0);
	
	if (status) {
		fprintf(stderr, "Stopping game because %s\n", message);
	}
	
	exit(status);
}

// Resets to plain window with border
void clear_and_border() {
	drawing = 1;
	clear();
	
	if (color_supported) {
		attron(COLOR_PAIR(WHITE));
	}
	
	// Draw border corners
	move(0,0);
	addch(ACS_ULCORNER);
	
	move(0, COLS - 1);
	addch(ACS_URCORNER);
	
	move(LINES - 1, 0);
	addch(ACS_LLCORNER);
	
	move(LINES - 1, COLS - 1);
	addch(ACS_LRCORNER);
	
	// Draw left and right border
	for (int i = 1; i < LINES - 1; i++) {
		move(i, 0);
		addch(ACS_VLINE);
		move(i, COLS - 1);
		addch(ACS_VLINE);
	}
	
	// Draw top and bottom border
	for (int i = 1; i < COLS - 1; i++) {
		move(0, i);
		addch(ACS_HLINE);
		move(LINES - 1, i);
		addch(ACS_HLINE);
	}
	
	if (color_supported) {
		attron(COLOR_PAIR(WHITE));
	}
}

// Moves cursor out of way and refreshes screen
void draw() {
	move(LINES - 1, 0);
	refresh();
	drawing = 0;
}

// Sets the pixel at the given point with the given color and character
// Note: character is an int because curses has special characters larger than a byte
void set_pixel(const Vec2D point, int character, int color) {
	if (color_supported) {
		attron(COLOR_PAIR(color));
	}
	
	move(point.y, point.x);
	addch(character);
	
	if (color_supported) {
		attroff(COLOR_PAIR(color));
	}
}

// Sets main loop function and starts timer with interval time
void loop(void (*f)(), int ms) {
	game_loop = f;
	signal(SIGALRM, loop_wrapper);
	start_timer(ms);
}

// (Private) Used to avoid passed function needing signum param.
void loop_wrapper(int signum) {
	if (game_loop) {
		game_loop();
	}
}

// Stops main loop
int stop_timer() {
	return start_timer(0);
}

/********* Below functions are from in class examples ********/

// Starts main loop or updates interval
int start_timer(int ms) {
	long s = ms / 1000 ;
	long us = ( ms % 1000 ) * 1000L ;

	struct itimerval timer = {
		it_interval: {
			tv_sec: s,
			tv_usec: us
		},
		it_value:{
			tv_sec: s,
			tv_usec: us
		}
	};

	return setitimer(ITIMER_REAL, &timer, NULL);
}

// Saves (how = 0) or restores (how = 1) tty mode
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
		curs_set(1);
	} else {
		curs_set(1);
	}
}

/* 
 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *  method: use bits in termios
 */
void set_cr_noecho_mode()
{
	struct	termios	ttystate;

	tcgetattr( 0, &ttystate);		/* read curr. setting	*/
	ttystate.c_lflag    	&= ~ICANON;	/* no buffering		*/
	ttystate.c_lflag    	&= ~ECHO;	/* no echo either	*/
	ttystate.c_cc[VMIN]  	=  1;		/* get 1 char at a time	*/
	tcsetattr( 0 , TCSANOW, &ttystate);	/* install settings	*/
}

/*
 * purpose: put file descriptor 0 into no-delay mode
 *  method: use fcntl to set bits
 *   notes: tcsetattr() will do something similar, but it is complicated
 */
void set_nodelay_mode()
{
	int	termflags;

	termflags = fcntl(0, F_GETFL);		/* read curr. settings	*/
	termflags |= O_NDELAY;			/* flip on nodelay bit	*/
	fcntl(0, F_SETFL, termflags);		/* and install 'em	*/
}
