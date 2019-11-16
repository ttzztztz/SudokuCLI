cmake_minimum_required(VERSION 3.15)
project(Sudoku)

set(CMAKE_CXX_STANDARD 17)

add_executable(Sudoku main.cpp command_color.h game_control.cpp game_control.h game_alg.cpp game_alg.h game_state.cpp game_state.h)