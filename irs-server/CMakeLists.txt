add_subdirectory(src)

file(GLOB_RECURSE SOURCES
    "irs_server_factory.cpp"
    "irs_server_factory.hpp"
    "irs_signal_handler.cpp"
    "irs_signal_handler.hpp"
)

add_executable(irs-server main.cpp ${SOURCES})
include_directories(${IRS_MEMORY_SRC})
target_link_libraries(irs-server irs-server-impl irs-memory)

#if(${IRS_BUILD_TESTS})
#    include(tests/CMakeLists.txt)
#endif()
