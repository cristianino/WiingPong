# CMakeLists.txt for audio module

add_library(audio STATIC
    AudioManager.cpp
)

target_include_directories(audio PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(audio
    asnd  # Wii audio library stub
)