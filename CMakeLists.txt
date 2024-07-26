cmake_minimum_required(VERSION 3.28)

project(FXFrameTest)

set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    SET(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif(APPLE)

#add_executable(FXFrameTest main.cpp)
#find_package(Libevent CONFIG REQUIRED)
#find_package(protobuf CONFIG REQUIRED)
#find_package(VTK REQUIRED)


set(SolutionDir ${CMAKE_SOURCE_DIR})

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/Debug)
    set(FXOutPutDir ${CMAKE_BINARY_DIR})
else()
    set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/Release)
    set(FXOutPutDir ${CMAKE_BINARY_DIR})
endif()

link_directories(${FXOutPutDir})

include_directories(${CMAKE_SOURCE_DIR}/Base)
include_directories(${CMAKE_SOURCE_DIR}/FXComm/FXPublic)
include_directories(${CMAKE_SOURCE_DIR}/FXComm/FXCore)
include_directories(${CMAKE_SOURCE_DIR}/FXComm/FXLogicDefine)


add_subdirectory(FXTutorial)
add_subdirectory(FXComm)

file(GLOB INCLUDE_FILES
        ${CMAKE_SOURCE_DIR}/Base/*.h
        ${CMAKE_SOURCE_DIR}/Base/*.hpp
        ${CMAKE_SOURCE_DIR}/FXComm/FXPublic/*.h
        ${CMAKE_SOURCE_DIR}/FXComm/FXPublic/*.hpp
        ${CMAKE_SOURCE_DIR}/FXComm/FXCore/*.h
        ${CMAKE_SOURCE_DIR}/FXComm/FXCore/*.hpp
        ${CMAKE_SOURCE_DIR}/FXComm/FXLogicDefine/*.h

)


install(FILES ${INCLUDE_FILES}
        DESTINATION include
)
