cmake_minimum_required(VERSION 4.0.0)

file(GLOB GAME_SRC "source/*.cpp")

add_library(
    Game STATIC
)

target_sources(
    Game
    PRIVATE ${GAME_SRC}
)

target_include_directories(
    Game
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(
    Game
    PUBLIC Singleton
)