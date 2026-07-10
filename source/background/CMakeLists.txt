# CMakeLists.txt for Background Module
# Parallax background system with multiple layers and effects

add_library(background
    BackgroundManager.cpp
)

# Include directories for this module
target_include_directories(background PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Link with dependencies
target_link_libraries(background
    ${GRRLIB_LIBRARIES}
    ${LIBOGC_LIBRARIES}
)