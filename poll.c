// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <unistd.h>

#include "shared.h"
#include "multiplexing.h"

void add_to_pfds(struct pollfd *pfds[], int fd_client, int *fd_count, int *fd_size) {
    if (*fd_count == *fd_size) {
        (*fd_size) *= 2;
        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[(*fd_count)].fd = fd_client;
    (*pfds)[(*fd_count)].events = POLLIN;
    (*fd_count)++;
}

void remove_from_pfds(struct pollfd pfds[], int i, int *fd_count) {
    pfds[i] = pfds[(*fd_count)-1];
    (*fd_count)--;
}

void using_poll(int fd_server) {
    printf("Using poll() for synchronous multiplexing.\n");

    int fd_count = 1;
    int fd_size = 1;
    struct pollfd *pfds = malloc(sizeof(*pfds) * fd_size);

    pfds[0].fd = fd_server;
    pfds[0].events = POLLIN; 

    while(1) {
        int poll_count = poll(pfds, fd_count, -1);
        if (poll_count == -1) {
            fprintf(stderr, "poll() failed. (%d)\n", errno);
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < fd_count; ++i) {
            if (pfds[i].revents & POLLIN) {
                if (pfds[i].fd == fd_server) {
                    int fd_client = accept_connection(fd_server);
                    add_to_pfds(&pfds, fd_client, &fd_count, &fd_size);
                } else {
                    // read from a client socket
                    int fd_client = pfds[i].fd;
                    if (read_and_echo(fd_client)) {
                        // client disconnected
                        close(fd_client);
                        remove_from_pfds(pfds, i, &fd_count);
                    }
                }
            }
        }
    }
}