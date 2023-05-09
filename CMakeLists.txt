cmake_minimum_required(VERSION 3.24)
project(SimplyTaint)

set(CMAKE_CXX_STANDARD 17)

#add_executable(SimplyTaint main.cpp)
# Enable solution folder support
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include Intel PIN targets (this should use find_package but this is easier sometimes)
include(cmake/CPM.cmake)
include(cmake/IntelPIN.cmake)

# Instruction counter example
add_pintool(SimplyTaint main.cpp)