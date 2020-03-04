
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_COMPILER "g++")

INCLUDE_DIRECTORIES(../libdb)

AUX_SOURCE_DIRECTORY(. APP_SRC)

ADD_DEFINITIONS( "-Wall -std=c++11" )

LINK_DIRECTORIES(/usr/lib/mysql)
LINK_DIRECTORIES(../lib)

ADD_EXECUTABLE(testdblib ${APP_SRC})

TARGET_LINK_LIBRARIES(testdblib uniqsdb mysqlclient)

