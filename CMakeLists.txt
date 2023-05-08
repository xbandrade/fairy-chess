cmake_minimum_required(VERSION 3.10)
project(fairy-chess)

add_executable(fairy-chess
    src/board.cpp
    src/main.cpp
    src/player.cpp
    src/piece.cpp
    src/game.cpp
)

include_directories(headers)

add_compile_options(-Wall)
