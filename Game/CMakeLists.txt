cmake_minimum_required(VERSION 4.0.0)

file(GLOB GAME_SRC
    "source/GameMode_ExtraReel/*.cpp"
    "source/GameMode_FreeSpins/*.cpp"
    "source/GameMode_Regular/*.cpp"
    "source/GameObjects/*.cpp"
    "source/MasterStates/*.cpp"
)

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
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../Singleton
)

target_link_libraries(
    Game
    PUBLIC Singleton
)