SET(LIBHELLO_SRC hello.c)

# dynamic library
ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})
# static library
ADD_LIBRARY(hello_static STATIC ${LIBHELLO_SRC})

# set dynamic library version
SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1)
# set static library name
SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")

# install libraries
INSTALL(TARGETS hello hello_static
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

INSTALL(FILES hello.h DESTINATION include/hello)

