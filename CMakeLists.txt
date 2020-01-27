project(directory_test)

# Bring in headers
include_directories(include)
set(CMAKE_BUILD_TYPE Debug)
add_subdirectory("resources/portaudio/")
add_subdirectory("resources/portaudio/bindings/cpp")

# Option 1: Add sources manually:
set(SOURCES 
    src/file1.cpp
    src/waveform.cpp
    src/SineWave.cpp
)

# Option 2: Add by * with file(GLOB ... )
#file(GLOB SOURCES "src/file*.cpp")

set(portaudio "portaudio_static")
set(portaudiocpp "portaudiocpp_static")

add_executable(testApp
            ${SOURCES}
)

target_link_libraries(testApp   
                    ${portaudio}
                    ${portaudiocpp}
)