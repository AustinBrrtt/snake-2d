#ifndef HOST_H
#define HOST_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "food.h"
#include "snake.h"

#define MESSAGE_BUFFER_SIZE 8192
#define PORT 49153

int sockid;
int asockid;
struct sockaddr_in addrport;
struct sockaddr_storage storage;
int port_size;
int addr_size;
int host_lock;
	
void cleanup_host();
void start_hosting();
void host_sync(int signum);
int serialize_snake(const Snake snake, char *buffer, int buffer_size);
int serialize_food(const Food food, char *buffer, int buffer_size);

#endif // HOST_H
