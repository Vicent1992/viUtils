project(cplus)
cmake_minimum_required(VERSION 2.8.12)

#SET(CMAKE_C_COMPILER   gcc)
#SET(CMAKE_CXX_COMPILER g++)
#SET(CMAKE_CXX_FLAGS -std=c++11)


set(CPLUS_SRC_FILES
    socket/socketServer.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/log/
    ${CMAKE_CURRENT_SOURCE_DIR}/socket/
)

add_executable(socketServer ${CPLUS_SRC_FILES})

install(TARGETS socketServer DESTINATION bin)
