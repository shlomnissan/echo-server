// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <unistd.h>

#include "config.h"
#include "shared.h"
#include "multiplexing.h"

void add_to_epfd(int epfd, int fd, int events) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) == -1) {
        fprintf(stderr, "epoll_ctl() EPOLL_CTL_ADD failed. (%d)\n", errno);
        exit(EXIT_FAILURE);
    }
}

void remove_from_epfd(int epfd, int fd) {
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        fprintf(stderr, "epoll_ctl() EPOLL_CTL_DEL failed. (%d)\n", errno);
        exit(EXIT_FAILURE);
    }
}

void using_epoll(int fd_server) {
    printf("Using epoll() for synchronous multiplexing.\n");

    int epfd = epoll_create(1);
    add_to_epfd(epfd, fd_server, EPOLLIN);

    struct epoll_event events[MAX_EVENTS];
    while (1) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == fd_server) {
                int fd_client = accept_connection(fd_server);
                add_to_epfd(epfd, fd_client, EPOLLIN | EPOLLET);
            } else {
               // read from a client socket
                int fd_client = events[i].data.fd;
                if (read_and_echo(fd_client)) {
                    // client disconnected
                    remove_from_epfd(epfd, fd_client);
                    close(fd_client);
                } 
            }
        }
    }
}