file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

add_library(irs-server-impl SHARED ${SOURCES})

include_directories(${IRS_MEMORY_SRC})
target_link_libraries(irs-server-impl PUBLIC irs-memory)
