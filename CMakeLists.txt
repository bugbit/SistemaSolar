cmake_minimum_required(VERSION 3.0.0)
project(ssolar VERSION 0.1.0)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    #set(CMAKE_C_FLAGS "-s STANDALONE_WASM" )
endif()

include_directories(extern)

include(CTest)
enable_testing()

add_executable(ssolar src/main.cpp src/shaders.cpp src/texture.cpp src/vao.cpp src/astro.cpp)

target_precompile_headers(ssolar PUBLIC src/pch.h)

    #set(copy_source_dir "${CMAKE_SOURCE_DIR}/assets")
    #set(copy_dest_dir "${CMAKE_BINARY_DIR}/assets")
    #set(copy_file_name "*.png")
    #add_custom_command(
    #    TARGET ${PROJECT_NAME} POST_BUILD
    #    COMMAND ${CMAKE_COMMAND} -E make_directory ${copy_dest_dir}
    #)
    #add_custom_command(
    #    TARGET ${PROJECT_NAME} POST_BUILD 
    #    COMMAND ${CMAKE_COMMAND} -E copy_directory "${copy_source_dir}" "${copy_dest_dir}"
    #    COMMENT "Copying ${copy_file_name} to build directory"
    #)

if (EMSCRIPTEN)
    set_target_properties(
        ssolar
        PROPERTIES
        LINK_FLAGS
        "-s TOTAL_MEMORY=33554432 -s USE_GLFW=3 -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"jpg\"]' --preload-file ../assets@assets"
    )
else(EMSCRIPTEN)
    target_link_libraries(ssolar glfw GL GLEW SDL2 SDL2main SDL2_image)    
endif(EMSCRIPTEN)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
