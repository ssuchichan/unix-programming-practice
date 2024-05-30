cmake_minimum_required(VERSION 3.2)
project(02socket_tcp C)

set(CMAKE_C_STANDARD 11)

add_executable(client client.c)

add_executable(server server.c)



