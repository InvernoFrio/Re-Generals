cmake_minimum_required(VERSION 3.16)
project(Re-Generals VERSION 0.9.1)

set(RAYLIB_DIR ${CMAKE_SOURCE_DIR}/lib/raylib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


set(CLIENT_SRC
    src/client.cpp
    src/map.cpp
    src/render.cpp)
set(SERVER_SRC
    src/server.cpp
    src/map.cpp)
    
include_directories(include)
add_executable(client ${CLIENT_SRC})
target_link_libraries(client ws2_32 ${RAYLIB_DIR}/libraylib.a opengl32 gdi32 winmm)


include_directories(include)
add_executable(server ${SERVER_SRC})
target_link_libraries(server ws2_32 ${RAYLIB_DIR}/libraylib.a opengl32 gdi32 winmm)


