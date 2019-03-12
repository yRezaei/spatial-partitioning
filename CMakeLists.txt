cmake_minimum_required(VERSION 3.0)

SET( PROJ_NAME      "spatial-partitioning" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -Wl,-subsystem,windows -std=c++17")

SET( PROJ_SOURCES
    "src/quadtree.cpp"
    "src/main.cpp"
    )

SET( PROJ_HEADERS
    "include/point.hpp"
    "include/quadtree.hpp"
    "include/aabb.hpp"
    )

SET( PROJ_INCLUDES
        ${CMAKE_SOURCE_DIR}/external_lib/glm/include
        ${CMAKE_SOURCE_DIR}/external_lib/sfml/include
    )

if (WIN32)
    link_directories(
            ${CMAKE_SOURCE_DIR}/external_lib/sfml/lib
    )
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    SET( PROJ_LIBRARIES "opengl32" "sfml-main" "sfml-graphics" "sfml-window" "sfml-system" "stdc++fs")
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
    SET( PROJ_LIBRARIES "opengl32" "sfml-main-d" "sfml-graphics-d" "sfml-window-d" "sfml-system-d" "stdc++fs")
endif()

PROJECT( ${PROJ_NAME} )

INCLUDE_DIRECTORIES( ${PROJ_INCLUDES} )
ADD_EXECUTABLE( ${PROJ_NAME} ${PROJ_SOURCES} )
TARGET_LINK_LIBRARIES( ${PROJ_NAME} ${PROJ_LIBRARIES} )

set(CMAKE_BINARY_DIR "bin/${CMAKE_BUILD_TYPE}")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/${CMAKE_BINARY_DIR})