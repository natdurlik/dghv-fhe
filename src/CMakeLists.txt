project(src)

set(HEADER_FILES utils.h SomewhatScheme.h FullyScheme.h PublicKey.h SecretKey.h)
set(SOURCE_FILES utils.cpp SomewhatScheme.cpp FullyScheme.cpp SecretKey.h)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})