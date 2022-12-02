add_executable(irs-storage-tests ${CMAKE_CURRENT_LIST_DIR}/irs_storage_tests.c)
target_include_directories(irs-storage-tests PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-storage-tests irs-storage irs-data ${GLIB2_LIBRARIES})

add_executable(irs-storage-bench-tests ${CMAKE_CURRENT_LIST_DIR}/irs_storage_bench_tests.c)
target_include_directories(irs-storage-bench-tests PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-storage-bench-tests irs-storage irs-data ${GLIB2_LIBRARIES})

add_executable(irs-engine-tests ${CMAKE_CURRENT_LIST_DIR}/irs_engine_tests.c)
target_include_directories(irs-engine-tests PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-engine-tests irs-engine irs-data ${GLIB2_LIBRARIES})

add_executable(irs-engine-bench-tests ${CMAKE_CURRENT_LIST_DIR}/irs_engine_bench_tests.c)
target_include_directories(irs-engine-bench-tests PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(irs-engine-bench-tests irs-engine irs-data ${GLIB2_LIBRARIES})
