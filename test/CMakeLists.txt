project(test)

add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(Google_Tests_run utils_test.cpp swhe_test.cpp fhe_test.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main)
target_link_libraries(Google_Tests_run ntl)
target_link_libraries(Google_Tests_run gmp gmpxx)
target_link_libraries(Google_Tests_run src)
