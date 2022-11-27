set(IRS_STORAGE_SRC "${IRS_MACHINE_ROOT}/irs-data")
file(GLOB_RECURSE SOURCES "*.c" "*.h")

add_library(irs-data SHARED ${SOURCES})

target_include_directories(irs-data PUBLIC ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-data ${GLIB2_LIBRARIES})
