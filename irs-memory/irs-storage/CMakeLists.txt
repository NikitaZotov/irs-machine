# dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB2 REQUIRED glib-2.0)
pkg_search_module(GLIB2_MODULE REQUIRED gmodule-2.0)

set(GLIB2_LIBRARIES ${GLIB_LDFLAGS} ${GLIB2_MODULE_LDFLAGS})

# library
file(GLOB_RECURSE SOURCES "*.c" "*.h")

add_library(irs-storage SHARED ${SOURCES})

target_include_directories(irs-storage PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-storage ${GLIB2_LIBRARIES})
