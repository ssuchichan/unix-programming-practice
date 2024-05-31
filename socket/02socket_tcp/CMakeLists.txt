cmake_minimum_required(VERSION 3.2)
project(02socket_tcp C)

set(CMAKE_C_STANDARD 11)

add_executable(client client.c)
add_executable(server_multi_process server_multi_process.c)
add_executable(server_multi_thread server_multi_thread.c)
add_executable(server_select server_select.c)
add_executable(server_epoll server_epoll.c)

