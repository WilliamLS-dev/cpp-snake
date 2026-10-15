cmake_minimum_required(VERSION 3.28)
project(MyProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(SFML_USE_STATIC_STD_LIBS ON)
FetchContent_MakeAvailable(SFML)

add_executable(main WIN32 src/main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)
if(WIN32)
    target_link_libraries(main PRIVATE SFML::Main)
endif()
target_link_libraries(main PRIVATE SFML::Audio)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Pixel Digivolve.otf"
        "${CMAKE_SOURCE_DIR}/hit.wav"
        "${CMAKE_SOURCE_DIR}/pickup.wav"
        $<TARGET_FILE_DIR:main>)
