cmake_minimum_required(VERSION 3.0.0)
project(ssolar VERSION 0.1.0)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    #set(CMAKE_C_FLAGS "-s STANDALONE_WASM" )
endif()

include(CTest)
enable_testing()

add_executable(ssolar src/main.cpp)

target_precompile_headers(ssolar PUBLIC src/pch.h)

set(copy_source_dir "${CMAKE_SOURCE_DIR}/assets")
    set(copy_dest_dir "${CMAKE_BINARY_DIR}/assets")
    set(copy_file_name "*.png")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${copy_dest_dir}
    )
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${copy_source_dir}" "${copy_dest_dir}"
        COMMENT "Copying ${copy_file_name} to build directory"
    )

if (EMSCRIPTEN)
    set_target_properties(
        ssolar
        PROPERTIES
        LINK_FLAGS
        "-s USE_GLFW=3 -s USE_WEBGL2=1  --preload-file assets"
    )
else(EMSCRIPTEN)
    target_link_libraries(ssolar glfw GL GLEW)
    
endif(EMSCRIPTEN)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
