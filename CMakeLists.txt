cmake_minimum_required(VERSION 3.25)
project(dghv_fhe)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

include_directories(src)

add_subdirectory(test)
add_subdirectory(src)

#find_package(GMP REQUIRED)
#find_package(NTL REQUIRED)

add_executable(dghv_fhe main.cpp ${SOURCE_FILES})
target_link_libraries(dghv_fhe ntl)
target_link_libraries(dghv_fhe gmp gmpxx)
target_link_libraries(dghv_fhe src)
