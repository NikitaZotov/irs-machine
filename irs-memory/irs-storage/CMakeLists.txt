set(IRS_STORAGE_SRC "${IRS_MACHINE_ROOT}/irs-storage")
file(GLOB_RECURSE SOURCES "*.c" "*.h")

add_library(irs-storage SHARED ${SOURCES})

target_include_directories(irs-storage PUBLIC ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-storage ${GLIB2_LIBRARIES})
