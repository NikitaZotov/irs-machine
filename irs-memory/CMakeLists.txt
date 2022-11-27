add_subdirectory(irs-engine)
add_subdirectory(irs-storage)

add_library(irs-memory SHARED "irs_memory.c" "irs_memory.h")

target_include_directories(irs-memory PUBLIC ${IRS_STORAGE_SRC})
target_link_libraries(irs-memory irs-storage)

if(${IRS_BUILD_TESTS})
    include(tests/CMakeLists.txt)
endif()
