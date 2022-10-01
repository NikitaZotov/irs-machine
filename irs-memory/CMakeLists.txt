add_subdirectory(irs-storage)

if(${IRS_BUILD_TESTS})
    include(tests/CMakeLists.txt)
endif()
