cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(directory_test)

# Bring in headers
include_directories(include)
set(CMAKE_BUILD_TYPE Debug)

set(RTMIDI_BUILD_STATIC_LIBS 1)

add_subdirectory("third_party/portaudio/")
add_subdirectory("third_party/portaudio/bindings/cpp")
add_subdirectory("third_party/rtmidi/")

#target_include_directories(rtmidi PUBLIC "${CMAKE_CURRENT_LIST_DIR}/third_party/rtmidi")


# Option 1: Add sources manually:
set(SOURCES 
    src/waveform.cpp
    src/oscillator.cpp
    src/SineWave.cpp
    src/squarewave.cpp
    src/trianglewave.cpp
    src/main.cpp
    src/miditest.cpp
)

# Option 2: Add by * with file(GLOB ... )
#file(GLOB SOURCES "src/file*.cpp")

set(portaudio "portaudio_static")
set(portaudiocpp "portaudiocpp_static")
set(rtmidi "rtmidi")

add_executable(testApp
            ${SOURCES}
)

target_link_libraries(testApp   
                    ${portaudio}
                    ${portaudiocpp}
                    ${rtmidi}

)