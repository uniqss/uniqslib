cmake_minimum_required(VERSION 3.2)

SET(PNAME "umkdir")

PROJECT(${PNAME})

INCLUDE(../cmake/ModuleImport.cmake)
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
INCLUDE(../cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()
ModuleSetWinCompilerFlags()

SET(UNIQS_VERSION_MAJOR "1")
SET(UNIQS_VERSION_MINOR "0")
SET(UNIQS_VERSION_PATCH "1")
SET(UNIQS_VERSION "${UNIQS_VERSION_MAJOR}.${UNIQS_VERSION_MINOR}.${UNIQS_VERSION_PATCH}")

MESSAGE(STATUS "VERSION: ${UNIQS_VERSION}")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include/uniqsnet/)

FILE(GLOB project_src_cpp "src/*.cpp")
FILE(GLOB project_src_h "src/*.h")
FILE(GLOB project_include "include/*.h")

IF (WIN32)
    LIST(APPEND project_src_cpp)
ENDIF(WIN32)

ADD_EXECUTABLE(${PNAME} ${project_src_cpp} ${project_src_h} ${project_include})

#SET(UNIQS_LIB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/)

#find_library(LIB_uniqsnet uniqsnet HINTS ${UNIQS_LIB_ROOT}/${CMAKE_BUILD_TYPE})
#message("find_library path: ${UNIQS_LIB_ROOT} find_result:${LIB_uniqsnet}")

#TARGET_LINK_LIBRARIES(${PNAME} PUBLIC ${LIB_uniqsnet})
