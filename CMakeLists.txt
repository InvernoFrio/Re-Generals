cmake_minimum_required(VERSION 3.16)
project(Re-Generals VERSION 0.0.1)

set(RAYLIB_DIR ${CMAKE_SOURCE_DIR}/lib/raylib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(./include)

aux_source_directory(./src src)

add_executable(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} ${RAYLIB_DIR}/libraylib.a opengl32 gdi32 winmm)