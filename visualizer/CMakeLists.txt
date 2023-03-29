cmake_minimum_required(VERSION 3.0)


project(Visualizer)

set(SOURCES
	sources/geometry/basics.cpp
	sources/geometry/vector_field.cpp
	sources/geometry/frame.cpp
	sources/geometry/circle.cpp
	sources/plotter/plotter.cpp
	sources/viewer/application.cpp
	sources/viewer/viewer_window.cpp
	sources/viewer/carousel.cpp
	sources/viewer/text_input.cpp
	sources/viewer/color_input.cpp
	sources/tracker/tracker.cpp
	sources/tracker/visual_preparator.cpp
	sources/tracker/neighbours_base.cpp
	sources/formula/formula_xy.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CUSTOM_FLAGS}")

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})

message("${ROOT_LIBRATIES}\n")

find_package(FLTK REQUIRED NO_MODULE)
include_directories(${FLTK_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
include_directories(sources)

add_executable(visualizer sources/main.cpp ${SOURCES})
# ${ROOT_LIBRARIES} can be used
target_link_libraries(visualizer Core Hist Graf Gpad fltk fltk_images matheval)