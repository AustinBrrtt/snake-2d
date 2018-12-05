#include "menu.h"

#include <stdlib.h>
#include <string.h>

#include "cursescontroller.h"
#include "vec2d.h"
#include "game.h"

void pause() {
	stop_timer();
	
	Menu *pause = malloc(sizeof(Menu));
	pause->title_width = 54;
	pause->title_height = 7;
	pause->title = malloc(pause->title_width * pause->title_height * sizeof(char));
	strcpy(pause->title, "======         ^        |      |    =======   ========|     |       / \\       |      |   /          |       |     |      /   \\      |      |   \\          |       |=====      /_____\\     |      |    =======   |====   |          /       \\    |      |           \\  |       |         /         \\   |      |           /  |       |        /           \\  \\______/    =======   ========");
	pause->title_alt = malloc(5 * sizeof(char));
	strcpy(pause->title_alt, "Pause");
	pause->title_color = YELLOW;
	
	pause->num_options = 3;
	pause->options = malloc(sizeof(MenuOption) * pause->num_options);
	
	pause->options[0].length = 6;
	pause->options[0].name = malloc(pause->options[0].length * sizeof(char));
	strcpy(pause->options[0].name, "Resume");
	pause->options[0].action = resume_wrapper;
	
	pause->options[1].length = 8;
	pause->options[1].name = malloc(pause->options[1].length * sizeof(char));
	strcpy(pause->options[1].name, "New Game");
	pause->options[1].action = main_menu;
	
	pause->options[2].length = 4;
	pause->options[2].name = malloc(pause->options[2].length * sizeof(char));
	strcpy(pause->options[2].name, "Exit");
	pause->options[2].action = exit_wrapper;
	
	pause->selected_option = 0;
	pause->back_option = 0;
	
	current_menu = pause;
	
	loop(run_menu, START_SPEED);
}

void main_menu() {
	stop_timer();
	
	Menu *main_menu = malloc(sizeof(Menu));
	main_menu->title_width = 54;
	main_menu->title_height = 7;
	main_menu->title = malloc(main_menu->title_width * main_menu->title_height * sizeof(char));
	strcpy(main_menu->title, " =======   |\\      |        ^        |     /  ========/          | \\     |       / \\       |    /   |       \\          |  \\    |      /   \\      |   /    |        =======   |   \\   |     /_____\\     |===     |====           \\  |    \\  |    /       \\    |   \\    |               /  |     \\ |   /         \\   |    \\   |        =======   |      \\|  /           \\  |     \\  ========");
	main_menu->title_alt = malloc(5 * sizeof(char));
	strcpy(main_menu->title_alt, "Snake");
	main_menu->title_color = GREEN;
	
	main_menu->num_options = 4;
	main_menu->options = malloc(sizeof(MenuOption) * main_menu->num_options);
	
	main_menu->options[0].length = 13;
	main_menu->options[0].name = malloc(main_menu->options[0].length * sizeof(char));
	strcpy(main_menu->options[0].name, "Single Player");
	main_menu->options[0].action = sp_wrapper;
	
	main_menu->options[1].length = 18;
	main_menu->options[1].name = malloc(main_menu->options[1].length * sizeof(char));
	strcpy(main_menu->options[1].name, "Multiplayer (Host)");
	main_menu->options[1].action = mp_wrapper_host;
	
	main_menu->options[2].length = 18;
	main_menu->options[2].name = malloc(main_menu->options[2].length * sizeof(char));
	strcpy(main_menu->options[2].name, "Multiplayer (Join)");
	main_menu->options[2].action = mp_wrapper_join;
	
	main_menu->options[3].length = 4;
	main_menu->options[3].name = malloc(main_menu->options[3].length * sizeof(char));
	strcpy(main_menu->options[3].name, "Exit");
	main_menu->options[3].action = exit_wrapper;
	
	main_menu->selected_option = 0;
	main_menu->back_option = 0;
	
	current_menu = main_menu;
	
	loop(run_menu, START_SPEED);
}

void exit_wrapper() {
	abort_game("",0);
}

void sp_wrapper() {
	start_game(SINGLE_PLAYER);
}

void mp_wrapper_host() {
	start_game(MP_HOST);
}

void mp_wrapper_join() {
	start_game(MP_JOIN);
}

void resume_wrapper() {
	switch(game_mode) {
		case SINGLE_PLAYER: 
			loop(main_loop, speed);
			return;
		case MP_HOST:
			loop(host_loop, speed);
			return;
		case MP_JOIN:
			loop(client_loop, speed);
	}
}

void run_menu() {
	switch (getch()) {
		case 'w':
		case 0x103: // up
		case 'a':
		case 0x104: // left
			current_menu->selected_option = (current_menu->num_options + current_menu->selected_option - 1) % current_menu->num_options; // Add num_options in extra time to avoid negative mod
			break;
		case 's':
		case 0x102: // down
		case 'd':
		case 0x105: // right
			current_menu->selected_option = (current_menu->selected_option + 1) % current_menu->num_options;
			break;
		case 0x1b: // escape
			current_menu->selected_option = current_menu->back_option;
		case 0x20: // space
		case 0xa: // enter
			current_menu->options[current_menu->selected_option].action();
			return;
	}
	if (!drawing) {
		draw_menu(current_menu);
	}
}

void draw_menu(Menu *menu) {
	clear_and_border();
	draw_title(menu);
	draw_options(menu);
	draw();
}

void draw_title(Menu *menu) {
	if (COLS - 2 > menu->title_width && 
		LINES - 8 - (2 * menu->num_options) > menu->title_height) {
		// Fancy title
		int row = LINES - 6 - (2 * menu->num_options) - menu->title_height;
		int col = (COLS - menu->title_width) / 2;
		
		for (int i = 0; i < menu->title_height; i++) {
			for (int j = 0; j < menu->title_width; j++) {
				set_pixel(*(new_vec2d(col + j, row + i)), menu->title[i * menu->title_width + j], menu->title_color);
			}
		}
	} else if (COLS - 2 > menu->title_alt_len && LINES > 3 + menu->num_options) {
		// Plain text title in border
		for (int i = 0; i < menu->title_alt_len; i++) {
			set_pixel(*(new_vec2d(1 + i, LINES - 1 - menu->num_options)), menu->title_alt[i], menu->title_color);
		}
	} else if (COLS > menu->title_alt_len && LINES > menu->num_options) {
		// Plain text title, no border
		for (int i = 0; i < menu->title_alt_len; i++) {
			set_pixel(*(new_vec2d(i, 0)), menu->title_alt[i], menu->title_color);
		}
	}
}


void draw_options(Menu *menu) {
	if (COLS - 2 > menu->title_width && 
		LINES - 8 - (2 * menu->num_options) > menu->title_height) {
		// Below fancy title with space between options
		int row = LINES - 4 - (2 * menu->num_options);
		
		for (int i = 0; i < menu->num_options; i++) {
			int color = WHITE;
			if (i == menu->selected_option) {
				color = RED;
			}
			int col = (COLS - menu->options[i].length) / 2;
			for (int j = 0; j < menu->options[i].length; j++) {
				set_pixel(*(new_vec2d(col + j, row + 2*i)), menu->options[i].name[j], color);
			}
		}
	} else if (COLS - 2 > menu->title_alt_len && LINES > 3 + menu->num_options) {
		// Below plain title inside border, no space between
		int row = LINES - menu->num_options - 1;
		
		for (int i = 0; i < menu->num_options; i++) {
			int color = WHITE;
			if (i == menu->selected_option) {
				color = RED;
			}
			int col = (COLS - menu->options[i].length) / 2;
			for (int j = 0; j < menu->options[i].length; j++) {
				set_pixel(*(new_vec2d(col + j, row + i)), menu->options[i].name[j], color);
			}
		}
	} else if (COLS > menu->title_alt_len && LINES > menu->num_options) {
		// Below plain title no border, no space
		int row = LINES - menu->num_options;
		
		for (int i = 0; i < menu->num_options; i++) {
			int color = WHITE;
			if (i == menu->selected_option) {
				color = RED;
			}
			int col = 0;
			for (int j = 0; j < menu->options[i].length; j++) {
				set_pixel(*(new_vec2d(col + j, row + i)), menu->options[i].name[j], color);
			}
		}
	} else {
		// No title, no border, no space
		int row = 0;
		
		for (int i = 0; i < menu->num_options; i++) {
			int color = WHITE;
			if (i == menu->selected_option) {
				color = RED;
			}
			int col = 0;
			for (int j = 0; j < menu->options[i].length; j++) {
				set_pixel(*(new_vec2d(col + j, row + i)), menu->options[i].name[j], color);
			}
		}
	}
}
