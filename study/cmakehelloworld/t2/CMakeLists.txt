PROJECT(HELLO)
ADD_SUBDIRECTORY(src bin)


INSTALL(FILES COPYRIGHT README DESTINATION share/doc/cmake/t2)
INSTALL(PROGRAMS build/bin/hello runhello.sh DESTINATION bin)
INSTALL(DIRECTORY doc/ DESTINATION share/cmake/t2)


