set(IRS_CONFIG_SRC "${IRS_SERVER_SRC}/irs-config")
file(GLOB_RECURSE SOURCES "*.c" "*.cpp" "*.h" "*.hpp")

add_library(irs-config SHARED ${SOURCES})

target_include_directories(irs-config PUBLIC ${GLIB2_INCLUDE_DIRS} ${IRS_MEMORY_SRC}/irs-data)
target_link_libraries(irs-config ${GLIB2_LIBRARIES} irs-data)
