project(src)

file(GLOB SOURCES *.cpp *.h)

add_library(src STATIC ${SOURCES})
target_include_directories(src PUBLIC .)