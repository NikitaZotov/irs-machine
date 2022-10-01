add_executable(irs-storage-tests ${CMAKE_CURRENT_LIST_DIR}/irs_storage_tests.c)

target_include_directories(irs-storage-tests PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-storage-tests irs-storage ${GLIB2_LIBRARIES})
