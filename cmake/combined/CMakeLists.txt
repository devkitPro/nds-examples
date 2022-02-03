cmake_minimum_required(VERSION 3.7)
project(cmake-example LANGUAGES C)

nds_build_arm7(cmake-example-arm7
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/arm7
)

add_subdirectory(arm9)

nds_create_rom(cmake-example ARM9 cmake-example-arm9 ARM7 cmake-example-arm7)
