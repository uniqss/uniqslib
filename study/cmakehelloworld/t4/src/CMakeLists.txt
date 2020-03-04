
FILE(GLOB srcs "*.c")

# add external library search directory. must before ADD_EXECUTABLE!!!
LINK_DIRECTORIES(/tmp/t3/lib)

# add external header file directory
INCLUDE_DIRECTORIES(/tmp/t3/include/hello)

# executable
#ADD_EXECUTABLE(main main.c)
ADD_EXECUTABLE(main ${srcs})

#
#TARGET_LINK_LIBRARIES(main libhello.a)
TARGET_LINK_LIBRARIES(main libhello.so)


