// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#include "shared.h"
#include "multiplexing.h"

void using_select(int fd_server) {
    printf("Using select() for synchronous multiplexing.\n");

    fd_set master;
    FD_ZERO(&master);
    FD_SET(fd_server, &master);
    int nfds = fd_server;
    while (1) {
        fd_set reads;
        reads = master;
        if (select(nfds + 1, &reads, NULL, NULL, /* timeout = */ NULL) < 0) {
            fprintf(stderr, "getaddrinfo() failed. (%d)\n", errno);
            exit(EXIT_FAILURE);
        }
        for (int i = 1; i <= nfds; ++i) {
            if (FD_ISSET(i, &reads)) {
                if (i == fd_server) {
                    // accept a new client socket
                    int fd_client = accept_connection(fd_server);
                    FD_SET(fd_client, &master);
                    if (fd_client > nfds) nfds = fd_client;
                } else {
                    // read from a client socket
                    if (read_and_echo(i)) {
                        // client disconnected
                        FD_CLR(i, &master);
                        close(i);
                    }
                }
            }
        }
    }
}