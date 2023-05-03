cmake_minimum_required(VERSION 3.25)
project(dghv_fhe)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-lgmp")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(GMP REQUIRED)
add_executable(dghv_fhe main.cpp)
target_link_libraries(dghv_fhe gmp gmpxx)
