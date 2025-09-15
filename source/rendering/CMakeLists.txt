# CMakeLists.txt for rendering module

add_library(rendering STATIC
    Renderer.cpp
)

target_include_directories(rendering PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(rendering
    ${GRRLIB_LIBRARIES}
)