cmake_minimum_required(VERSION 3.15)
project(icount)

# Enable solution folder support
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include Intel PIN targets (this should use find_package but this is easier sometimes)
include(cmake/CPM.cmake)
include(cmake/IntelPIN.cmake)

# Instruction counter example
add_pintool(icount
        ${PIN_DIR}/source/tools/InstLibExamples/icount.cpp
        )