cmake_minimum_required(VERSION 3.2)
project(01socket_udp C)

set(CMAKE_C_STANDARD 11)

add_executable(client client.c)
add_executable(server server.c)
