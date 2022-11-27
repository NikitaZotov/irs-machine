add_executable(irs-storage-tests ${CMAKE_CURRENT_LIST_DIR}/irs_storage_tests.c)

target_include_directories(irs-storage-tests PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-storage-tests irs-storage irs-data ${GLIB2_LIBRARIES})

add_executable(irs-engine-tests ${CMAKE_CURRENT_LIST_DIR}/irs_engine_tests.c)

target_include_directories(irs-engine-tests PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-engine-tests irs-engine irs-data ${GLIB2_LIBRARIES})
