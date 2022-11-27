set(IRS_ENGINE_SRC "${IRS_MACHINE_ROOT}/irs-engine")
file(GLOB_RECURSE SOURCES "*.c" "*.h")

add_library(irs-engine SHARED ${SOURCES})

target_include_directories(irs-engine PUBLIC ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-engine ${GLIB2_LIBRARIES})
