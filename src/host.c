#include "host.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include "cursescontroller.h"

void cleanup_host() {
	if (sockid) {
		close(sockid);
	}
	exit(0);
}

void start_hosting()
{
    sockid = 0;
    struct sockaddr_in addrport;
    struct sockaddr_storage storage;
    int port_size = sizeof(addrport);
    int addr_size = sizeof(storage);

    if ((sockid = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        abort_game("Failed to create socket", 1);
    }

    addrport.sin_family = AF_INET;
    addrport.sin_port = htons(PORT);
    addrport.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockid, (struct sockaddr *) &addrport, port_size) != -1) {
        if (listen(sockid, 5) == 0) {
            int asockid = accept(sockid, &storage, &addr_size);
        } else {
            close(sockid);
            abort_game("Failed to listen.", 1);
        }
    } else {
        abort_game("Failed to bind to port.", 1);
    }
}

int host_sync (const Snake snake, const Food food) {
    if (!sockid || !asockid) {
        abort_game("Socket closed", 1);
    }
    
    char buffer[MESSAGE_BUFFER_SIZE];
    memset(buffer, '\0', MESSAGE_BUFFER_SIZE);
    read(asockid, buffer, MESSAGE_BUFFER_SIZE);
    char *response[MESSAGE_BUFFER_SIZE];
    int offset = serialize_snake(snake, response, MESSAGE_BUFFER_SIZE);
    serialize_food(food, response + offset, MESSAGE_BUFFER_SIZE - offset);
    memset(response, '\0', MESSAGE_BUFFER_SIZE);
    send(asockid, response, MESSAGE_BUFFER_SIZE, 0);
    
    return atoi(buffer);
}

int serialize_snake(const Snake snake, char *buffer, int buffer_size) {
    sprintf(buffer, "S%03d%03d", snake.direction, snake.growing);
    SnakeNode *cur = snake.head;
    int offset = 7;
    while (cur != NULL && offset < MESSAGE_BUFFER_SIZE - 6) {
        sprintf(buffer + offset, "%03d%03d", cur->pos->x, cur->pos->y);
        offset += 6;
    }
    return offset;
}

int serialize_food(const Food food, char *buffer, int buffer_size) {
    buffer[0] = 'F';
    sprintf(buffer, "%03d%03d%03d", food.pos->x, food.pos->y, food.life);
    return 9;
}
