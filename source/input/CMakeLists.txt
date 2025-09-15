# CMakeLists.txt for input module

add_library(input STATIC
    InputManager.cpp
)

target_include_directories(input PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(input
    ${WIIUSE_LIBRARIES}
)