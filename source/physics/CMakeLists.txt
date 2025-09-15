# CMakeLists.txt for physics module

add_library(physics STATIC
    PhysicsEngine.cpp
)

target_include_directories(physics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)