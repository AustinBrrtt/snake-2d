#include "client.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "vec2d.h"
#include "host.h"

int parse_int(char* str, int digits) {
	int n = 0;
	for (int i = 0; i < digits; i++) {
		n *= 10;
		n += str[i] - '0';
	}
	return n;
}

void from_int(int number, char *output, int digits) {
	for (int i = digits - 1; i >= 0; i--) {
		output[i] = (char) number % 10 + '0';
		number /= 10;
	}
}

void cleanup_client() {
	close(client_sockid);
}

void join() {
	int client_client_sockid;
    struct sockaddr_in addrport;
    struct sockaddr_storage storage;
    int port_size = sizeof addrport;
    int addr_size = sizeof storage;

    if ((client_sockid = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        abort_game("failed to create socket", 1);
    }

    addrport.sin_family = AF_INET;
    addrport.sin_port = htons(PORT);
    addrport.sin_addr.s_addr = htonl(INADDR_ANY);

    if (inet_pton(AF_INET, "127.0.0.1", &addrport.sin_addr) <= 0) {
        abort_game("invalid address", 1);
    }

    if (connect(client_sockid, (struct sockaddr *)&addrport, sizeof(addrport)) < 0) {
        abort_game("failed to connect", 1);
    }
}

void client_sync(int input, Snake *snake, Food *food) {
    char buffer[MESSAGE_BUFFER_SIZE];
    memset(buffer, '\0', MESSAGE_BUFFER_SIZE);
	char msg[5];
	from_int(input, msg, 5);
	send(client_sockid, msg, 5, 0);
    read(client_sockid, buffer, MESSAGE_BUFFER_SIZE);
	snake = parse_snake(buffer, strlen(buffer));
	int offset = 1;
	while (buffer[offset] != 'F') {
		offset += 6;
	}
	food = parse_food(buffer + offset, strlen(buffer + offset));
}


Snake *parse_snake(char *message, int message_length) {
	if (message_length < 7) {
		abort_game("invalid snake received", 1);
	}
	Snake *snake = malloc(sizeof(Snake));
	snake->head = malloc(sizeof(SnakeNode));
	snake->growing = parse_int(message[1], 3);
	snake->direction = parse_int(message[4], 3);
	
	SnakeNode *current = snake->head;
    for (int i = 0; i < message_length; i += 6) {
		current->pos = malloc(sizeof(Vec2D));
		current->pos->x = parse_int(message + 7 + i, 3);
		current->pos->x = parse_int(message + 10 + i, 3);
		if (message[13+i] != 'F') {
			current->next = malloc(sizeof(SnakeNode));
			current = current->next;
		}
	}
	
	return snake;
}

Food *parse_food(char *message, int message_length) {
    if (message_length < 10 || message[0] != 'F') {
		abort_game("invalid food received", 1);
	}
	Food *food = malloc(sizeof(Food));
	food->pos = malloc(sizeof(Vec2D));
	food->pos->x = parse_int(message + 1, 3);
	food->pos->y = parse_int(message + 4, 3);
	food->life = parse_int(message + 7, 3);
	return food;
}
