# CMakeLists.txt for gamestate module

add_library(gamestate STATIC
    GameStateManager.cpp
)

target_include_directories(gamestate PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)