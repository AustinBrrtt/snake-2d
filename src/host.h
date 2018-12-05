#ifndef HOST_H
#define HOST_H

#include "food.h"
#include "snake.h"

#define MESSAGE_BUFFER_SIZE 8192
#define PORT 49153

int sockid;
int asockid;
	
void cleanup_host();
void start_hosting();
int host_sync(const Snake snake, const Food food);
int serialize_snake(const Snake snake, char *buffer, int buffer_size);
int serialize_food(const Food food, char *buffer, int buffer_size);

#endif // HOST_H
