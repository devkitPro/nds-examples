cmake_minimum_required(VERSION 3.7)
project(cmake-example-arm7 LANGUAGES C)

add_executable(cmake-example-arm7 source/template.c)
target_link_libraries(cmake-example-arm7 dswifi7 mm7)
