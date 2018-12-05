#ifndef CLIENT_H
#define CLIENT_H

#include "snake.h"
#include "food.h"


int client_sockid;

void from_int(int number, char *output, int digits);
void cleanup_client();
void join();
void client_sync(int input, Snake *snake, Food *food);

Snake *parse_snake(char *message, int message_length);

Food *parse_food(char *message, int message_length);

#endif // CLIENT_H