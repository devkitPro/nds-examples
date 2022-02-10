cmake_minimum_required(VERSION 3.7)
project(cmake-example LANGUAGES C)

add_executable(cmake-example source/main.c)
nds_create_rom(cmake-example)
