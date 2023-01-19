cmake_minimum_required(VERSION 3.24)
project(cahoot C)

set(CMAKE_C_STANDARD 11)

add_executable(cahoot
        client.c
        main.c
        server.c lib/packet.h lib/packet.c)
