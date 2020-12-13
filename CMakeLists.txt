cmake_minimum_required(VERSION 3.5)

project(blsd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

file( GLOB headers "${CMAKE_SOURCE_DIR}/*.h")
file( GLOB sources "${CMAKE_SOURCE_DIR}/*.cpp")

set( BLSD_INCLUDES ${headers} )
set( BLSD_SOURCES ${sources} )

#file( GLOB_RECURSE REMOVE_CMAKE "${CMAKE_SOURCE_DIR}/build/*.cpp" )
#list( REMOVE_ITEM sources "${REMOVE_CMAKE}" )

find_package(ZLIB)
if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME} ${BLSD_INCLUDES} ${BLSD_SOURCES})

target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
