cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 99)

project(echo_server)

include_directories(include)

set(SOURCES
    main.c
    shared.c
    select.c
    poll.c
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
