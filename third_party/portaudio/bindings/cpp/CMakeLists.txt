file(GLOB sources "${CMAKE_CURRENT_LIST_DIR}/source/portaudiocpp/*")
file(GLOB headers "${CMAKE_CURRENT_LIST_DIR}/include/portaudiocpp/*")

add_library(portaudiocpp_static STATIC "${sources}" "${headers}")
target_include_directories(portaudiocpp_static PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(portaudiocpp_static PRIVATE portaudio_static)
