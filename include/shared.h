// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#ifndef ECHO_SERVER_SHARED_H
#define ECHO_SERVER_SHARED_H

#include <netdb.h>

void get_local_address(struct addrinfo** address);

int create_socket(const struct addrinfo* address);

void start_listening(int fd_server);

int accept_connection(int fd_server);

#endif  // ECHO_SERVER_SHARED_H