cmake_minimum_required(VERSION 2.8)

# give a name to the project
project(focus)

# find opencv
find_package(OpenCV REQUIRED)

# check opencv version
if(${OpenCV_VERSION} VERSION_LESS 2.0.0)
message(FATAL_ERROR “OpenCV version is not compatible : ${OpenCV_VERSION}”)
endif()

# compilation flags
set(CMAKE_CXX_FLAGS "-Wall -g -O2 -std=c++14")

# put the binary files in this directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# create a directory called 'output' in the project binary directory
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# the files to compile
add_executable(focus src/Context.cpp src/Context.hpp src/focus.cpp src/Commands.cpp)
target_link_libraries(focus ${OpenCV_LIBS})
