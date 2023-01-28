// Copyright 2023 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#ifndef ECHO_SERVER_MULTIPLEXING_H
#define ECHO_SERVER_MULTIPLEXING_H

void using_select(int fd_server);

// void using_poll(int fd_server);

// void using_epoll(int fd_server);

#endif  // ECHO_SERVER_MULTIPLEXING_H