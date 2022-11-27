set(IRS_ENGINE_SRC "${IRS_MEMORY_SRC}/irs-engine")
file(GLOB_RECURSE SOURCES "*.c" "*.h")

add_library(irs-engine SHARED ${SOURCES})

target_include_directories(irs-engine PUBLIC ${GLIB2_INCLUDE_DIRS} ${IRS_MEMORY_SRC}/irs-data)
target_link_libraries(irs-engine ${GLIB2_LIBRARIES} irs-data)
