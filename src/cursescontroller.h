#ifndef CURSES_CONTROLLER_H
#define CURSES_CONTROLLER_H

#include <curses.h> // included in header to share functions

#include "vec2d.h"

#define WHITE 0
#define GREEN 1
#define BLUE 2
#define RED 3
#define YELLOW 4


/***** Variables *****/
// Flag to tell whether color can be used
int color_supported;

// Main game logic loop
void (*game_loop)();

// Cleanup for game logic
void (*game_cleanup)();

// Lock to avoid frames being drawn simultaneously
int drawing;


/***** Functions *****/
// Sets up terminal for curses and sets capability flags
void init_curses(void (*cleanup)());

// Restores terminal to normal state
void cleanup_curses(int signum);

// Cleans up and kills process
void abort_game(const char *message, int status);

// Resets to plain window with border
void clear_and_border();

// Moves cursor out of way and refreshes screen
void draw();

// Sets the pixel at the given point with the given color and character
void set_pixel(const Vec2D point, int character, int color);

// Sets game loop function and starts timer with interval time
void loop(void (*f)(), int ms);

// (Private) Used to avoid passed function needing signum param.
void loop_wrapper(int signum);

// Stops main loop
int stop_timer();

/********* Below functions are from in class examples ********/

// Starts main loop or updates interval
int start_timer(int ms);

// Saves (how = 0) or restores (how = 1) tty mode
void tty_mode(int how);

// Disable echo
void set_cr_noecho_mode();

// Enables realtime keyboard input
void set_nodelay_mode();

#endif // CURSES_CONTROLLER_H
