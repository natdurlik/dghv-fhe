project(test)

add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(Google_Tests_run rounded_quot_mod_test.cpp she_test.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main)
target_link_libraries(Google_Tests_run gmp gmpxx)
target_link_libraries(Google_Tests_run src)
