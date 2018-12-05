#ifndef MENU_H
#define MENU_H

typedef struct {
	char *name;
	int length;
	void (*action)();
} MenuOption;

typedef struct {
	char *title_alt;
	int title_alt_len;
	char *title;
	int title_width;
	int title_height;
	int title_color;
	MenuOption *options;
	int num_options;
	int selected_option;
	int back_option;
} Menu;

Menu *current_menu;

void pause();
void main_menu();

// Parameter-less functions to use as actions in menus
void exit_wrapper();
void sp_wrapper();
void mp_wrapper_host();
void mp_wrapper_join();
void resume_wrapper();

void run_menu();

void draw_menu(Menu *menu);
void draw_title(Menu *menu);
void draw_options(Menu *menu);

#endif // MENU_H
