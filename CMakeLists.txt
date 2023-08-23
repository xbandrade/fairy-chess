cmake_minimum_required(VERSION 3.10)
project(fairy-chess)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS
    src/board.hpp
    src/player.hpp
    src/piece.hpp
    src/game.hpp
)

set(SOURCES
    main.cpp
    src/board.cpp
    src/player.cpp
    src/piece.cpp
    src/game.cpp
)

add_executable(fairy-chess ${HEADERS} ${SOURCES})
