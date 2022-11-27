add_subdirectory(irs-data)
add_subdirectory(irs-storage)
add_subdirectory(irs-engine)

add_library(irs-memory SHARED "irs_memory.c" "irs_memory.h")

target_include_directories(irs-memory PUBLIC ${IRS_STORAGE_SRC} ${IRS_ENGINE_SRC})
target_link_libraries(irs-memory irs-storage irs-engine)

if(${IRS_BUILD_TESTS})
    include(tests/CMakeLists.txt)
endif()
