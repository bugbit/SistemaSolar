cmake_minimum_required(VERSION 3.0.0)
project(ssolar VERSION 0.1.0)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    #set(CMAKE_C_FLAGS "-s STANDALONE_WASM" )
endif()

include(CTest)
enable_testing()

add_executable(ssolar src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
