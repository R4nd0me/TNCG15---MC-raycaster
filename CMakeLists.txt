cmake_minimum_required(VERSION 3.20)

project(MonteCarloRenderer
    VERSION 0.1.0
    DESCRIPTION "A Monte Carlo path tracer written in C++"
    LANGUAGES CXX
)

add_executable(renderer
    src/main.cpp
    src/vec3.h
    src/color.h
    src/ray.h
)

target_compile_features(renderer PRIVATE cxx_std_20)

target_include_directories(renderer PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

if(MSVC)
    target_compile_options(renderer PRIVATE /W4)
else()
    target_compile_options(renderer PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()