cmake_minimum_required(VERSION 4.0.0)
project(InternSlot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/Singleton)
add_subdirectory(${CMAKE_SOURCE_DIR}/Game)

add_executable(
    InternSlot
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(
    InternSlot
    PRIVATE Game
)