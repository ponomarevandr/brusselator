cmake_minimum_required(VERSION 3.0)


project(Visualizer)

set(SOURCES
	
)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})

find_package(FLTK REQUIRED NO_MODULE)
include_directories(${FLTK_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
include_directories(sources)

add_executable(visualizer sources/main.cpp ${SOURCES})
target_link_libraries(visualizer ${ROOT_LIBRARIES} fltk)