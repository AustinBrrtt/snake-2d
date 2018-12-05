#include "host.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "cursescontroller.h"
#include "game.h"

void cleanup_host() {
	if (sockid) {
		close(sockid);
	}
}

void start_hosting()
{
    host_lock = 1;
    loop(host_loop, START_SPEED);
    host_lock = 0;
    while (1) {
        sockid = 0;
        port_size = sizeof(addrport);
        addr_size = sizeof(storage);

        if ((sockid = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
            abort_game("Failed to create socket", 1);
        }

        addrport.sin_family = AF_INET;
        addrport.sin_port = htons(PORT);
        addrport.sin_addr.s_addr = htonl(INADDR_ANY);
        
        // int flags = fcntl(sockid, F_GETFL, 0);
        // if (flags == -1) {
        //     abort_game("Failed to get flags", 1);
        // }
        // flags =  flags | O_NONBLOCK;
        // fcntl(sockid, F_SETFL, flags);
        if (bind(sockid, (struct sockaddr *) &addrport, port_size) != -1) {
            if (listen(sockid, 5) == 0) {
                asockid = accept(sockid, (struct sockaddr *) &storage, &addr_size);
                fcntl(asockid, __F_SETOWN, getpid());
                signal(SIGIO, host_sync);
                host_lock = 1;
                const Snake snaker = *snake;
                const Food fooder = *food;
                if (!sockid || !asockid) {
                    abort_game("Socket closed", 1);
                }
                
                char buffer[MESSAGE_BUFFER_SIZE];
                memset(buffer, '\0', MESSAGE_BUFFER_SIZE);
                read(asockid, buffer, MESSAGE_BUFFER_SIZE);
                char response[MESSAGE_BUFFER_SIZE];
                memset(response, '\0', MESSAGE_BUFFER_SIZE);
                int offset = serialize_snake(snaker, response, MESSAGE_BUFFER_SIZE);
                serialize_food(fooder, response + offset, MESSAGE_BUFFER_SIZE - offset);
                send(asockid, response, MESSAGE_BUFFER_SIZE, 0);
                
                host_lock = 0;
                client_input = atoi(buffer);
            } else {
                close(sockid);
                abort_game("Failed to listen.", 1);
            }
        } else {
            abort_game("Failed to bind to port.", 1);
        }
        close(sockid);
        sleep(1);
    }
}

void host_sync (signum) {
    if (host_lock) {
        return;
    }
    host_lock = 1;
    const Snake snaker = *snake;
    const Food fooder = *food;
    if (!sockid || !asockid) {
        abort_game("Socket closed", 1);
    }
    
    char buffer[MESSAGE_BUFFER_SIZE];
    memset(buffer, '\0', MESSAGE_BUFFER_SIZE);
    read(asockid, buffer, MESSAGE_BUFFER_SIZE);
    char response[MESSAGE_BUFFER_SIZE];
    memset(response, '\0', MESSAGE_BUFFER_SIZE);
    int offset = serialize_snake(snaker, response, MESSAGE_BUFFER_SIZE);
    serialize_food(fooder, response + offset, MESSAGE_BUFFER_SIZE - offset);
    send(asockid, response, MESSAGE_BUFFER_SIZE, 0);
    
    host_lock = 0;
    client_input = atoi(buffer);
}

int serialize_snake(const Snake snake, char *buffer, int buffer_size) {
    sprintf(buffer, "S%03d%03d", snake.direction, snake.growing);
    SnakeNode *cur = snake.head;
    int offset = 7;
    while (cur != NULL && offset < MESSAGE_BUFFER_SIZE - 6) {
        sprintf(buffer + offset, "%03d%03d", cur->pos->x, cur->pos->y);
        offset += 6;
        cur = cur->next;
    }
    return offset;
}

int serialize_food(const Food food, char *buffer, int buffer_size) {
    if (buffer_size < 10) {
        char message[1000];
        sprintf(message, "Not enough room for food in the buffer of size %d.", buffer_size);
        abort_game(message, 1);
    }
    sprintf(buffer, "F%03d%03d%03d", food.pos->x, food.pos->y, food.life);
    return 10;
}
