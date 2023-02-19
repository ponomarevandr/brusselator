cmake_minimum_required(VERSION 3.0)


project(Checkers)

set(SOURCES
	
)

add_compile_options(-std=c++17)
include_directories(sources)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(visualizer sources/main.cpp ${SOURCES})