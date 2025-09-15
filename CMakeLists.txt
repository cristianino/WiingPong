# CMakeLists.txt for WiingPong - Wii Homebrew Game
# This build system uses devkitPPC toolchain. Run cmake with:
# cmake -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/powerpc-eabi/cmake/powerpc-eabi.toolchain.cmake ..

cmake_minimum_required(VERSION 3.12)
project(WiingPong CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wii-specific settings (assumes devkitPPC environment)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Find required packages (devkitPPC provides these)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GRRLIB REQUIRED grrlib)
pkg_check_modules(LIBOGC REQUIRED libogc)
pkg_check_modules(WIIUSE REQUIRED wiiuse)

# Global include directories
include_directories(${CMAKE_SOURCE_DIR}/include
                   ${GRRLIB_INCLUDE_DIRS}
                   ${LIBOGC_INCLUDE_DIRS}
                   ${WIIUSE_INCLUDE_DIRS})

# Add subdirectories for modules (as they are created)
add_subdirectory(source/core)
add_subdirectory(source/input)
add_subdirectory(source/physics)
add_subdirectory(source/rendering)
add_subdirectory(source/audio)
add_subdirectory(source/ui)
add_subdirectory(source/gamestate)
add_subdirectory(source/assets)
add_subdirectory(source/events)

# Main executable
add_executable(WiingPong
    source/main.cpp  # Temporary: will refactor to core/GameEngine.cpp
)

# Link libraries
target_link_libraries(WiingPong
    core
    input
    physics
    rendering
    audio
    ui
    gamestate
    assets
    events
    ${GRRLIB_LIBRARIES}
    ${LIBOGC_LIBRARIES}
    ${WIIUSE_LIBRARIES}
    fat
    asnd
    dvd
    es
    wiiuse
    wiimote
    ntfs
    ogc
    sdhc
)

# Create DOL file for Wii (custom post-build)
add_custom_command(TARGET WiingPong POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary WiingPong.elf WiingPong.dol
    COMMENT "Generating WiingPong.dol"
)

# For release: Strip and pack
add_custom_target(release
    DEPENDS WiingPong
    COMMAND ${CMAKE_STRIP} WiingPong.elf -g
    COMMAND tar -czf WiingPong-${CMAKE_PROJECT_VERSION}.tar.gz WiingPong.dol meta.xml
)

# Install (for development)
install(TARGETS WiingPong RUNTIME DESTINATION bin)