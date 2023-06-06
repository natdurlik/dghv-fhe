project(src)

set(HEADER_FILES rounded_quot_mod.h SomewhatScheme.h FullyScheme.h PublicKey.h)
set(SOURCE_FILES rounded_quot_mod.cpp SomewhatScheme.cpp FullyScheme.cpp PublicKey.cpp)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})