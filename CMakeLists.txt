cmake_minimum_required(VERSION 3.10)
project(sudoku2 C)

set(CMAKE_C_STANDARD 90)

add_executable(sudoku2 main.c solver.c solver.h parser.c parser.h game.c game.h)