file(GLOB TEST_SOURCES "*.cpp")
add_executable(interpolate_test ${TEST_SOURCES})
target_link_libraries(interpolate_test PRIVATE interpolate)