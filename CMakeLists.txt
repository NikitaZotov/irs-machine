cmake_minimum_required(VERSION 2.8)
project(irs-machine)
cmake_policy(SET CMP0048 NEW)

set(IRS_MACHINE_ROOT ${CMAKE_CURRENT_LIST_DIR})

set(IRS_BIN_PATH "${CMAKE_BINARY_DIR}/../bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${IRS_BIN_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${IRS_BIN_PATH})

option(IRS_BUILD_TESTS "Flag to build tests" ON)

set(IRS_MEMORY_SRC "${IRS_MACHINE_ROOT}/irs-memory")

if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -std=c++17")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s") ## Strip binary
endif()

# dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB2 REQUIRED glib-2.0)
pkg_search_module(GLIB2_MODULE REQUIRED gmodule-2.0)

set(GLIB2_LIBRARIES ${GLIB_LDFLAGS} ${GLIB2_MODULE_LDFLAGS})

add_subdirectory(${IRS_MACHINE_ROOT}/irs-memory)
add_subdirectory(${IRS_MACHINE_ROOT}/irs-server)
