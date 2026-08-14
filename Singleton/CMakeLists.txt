cmake_minimum_required(VERSION 4.0.0)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_library(Singleton STATIC)

file(GLOB SINGLETON_SOURCES "source/*.cpp")

target_sources(
    Singleton
    PRIVATE ${SINGLETON_SOURCES}
)

target_include_directories(
    Singleton
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(
    Singleton
    PUBLIC SDL3::SDL3
    PUBLIC SDL3_ttf::SDL3_ttf
    PUBLIC SDL3_image::SDL3_image
)