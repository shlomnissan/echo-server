// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "shared.h"
#include "config.h"

void get_local_address(const char* port, struct addrinfo** address) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", errno);
        exit(EXIT_FAILURE);
    }
}

int create_socket(const struct addrinfo* address) {
    int fd_server = socket(
        address->ai_family,
        address->ai_socktype,
        address->ai_protocol
    );
    if (!fd_server) {
        fprintf(stderr, "socket() failed. (%d)\n", errno);
        exit(EXIT_FAILURE);
    }

    if (bind(fd_server, address->ai_addr, address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", errno);
        exit(EXIT_FAILURE);
    } 
    return fd_server;
}

void start_listening(int fd_server) {
    printf("Listening to new connections...\n");
    if (listen(fd_server, MAX_BACKLOG) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", errno);
        exit(EXIT_FAILURE);
    }
}

int accept_connection(int fd_server) {
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    int fd_client = accept(fd_server, (struct sockaddr*)&client_address, &client_len);
    if (!fd_client) {
        fprintf(stderr, "accept() failed. (%d)\n", errno);
        exit(EXIT_FAILURE);
    }
    char address_buffer[100];
    getnameinfo(
        (struct sockaddr*)&client_address,
        client_len,
        address_buffer, sizeof(address_buffer), 0, 0,
        NI_NUMERICHOST
    );
    printf("Accepted a new connection from %s\n", address_buffer);
    return fd_client;
}

int read_and_echo(int fd_client) {
    char read_buffer[MAX_READ_BUFFER];
    long bytes = recv(fd_client, read_buffer, sizeof(read_buffer), 0);
    if (bytes <= 0) {
        printf("Client (%d) disconnected\n", fd_client);
        return 1;
    } 
    send(fd_client, read_buffer, bytes, 0);
    return 0;
}