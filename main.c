// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "multiplexing.h"
#include "shared.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: echo_server [select|poll|epoll]");
        return 1;
    }

    const char* method = argv[1];
    if (strcmp(method, "select") && strcmp(method, "poll") && strcmp(method, "epoll")) {
        printf("Unknown multiplexing method. Use select, poll or epoll.\n");
        return 1;
    }

    // configure local address
    struct addrinfo* address;
    get_local_address(&address);

    // create socket and start listening
    int fd_server = create_socket(address);
    freeaddrinfo(address);
    start_listening(fd_server); 

    // select multiplexing method
    if (strcmp(method, "select") == 0) {
        printf("Using select() for synchronous multiplexing.\n");
        using_select(fd_server);
    }
    if (strcmp(method, "poll") == 0) {
        printf("Using poll() for synchronous multiplexing.\n");
        // TODO: using_poll(fd_server);
    }
    if (strcmp(method, "epoll") == 0) {
        printf("Using epoll() for synchronous multiplexing.\n");
        // TODO: using_epoll(fd_server);
    }

    close(fd_server);

    return 0;
}