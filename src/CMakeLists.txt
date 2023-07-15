project(src)

set(HEADER_FILES utils.h SomewhatScheme.h FullyScheme.h Ciphertext.h PublicKey.h SecretKey.h)
set(SOURCE_FILES utils.cpp SomewhatScheme.cpp FullyScheme.cpp Ciphertext.cpp)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})