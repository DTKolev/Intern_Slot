#pragma once

#include "Singleton.hpp"
#include "Grid.hpp"
#include "Analyzer.hpp"
#include <string>

/**
 * @brief Static manager class for the Slot Game
 *
 * Functions as a public container for storing game data
 * All game states can read/write to it
 *
 * Handles the saving of the current game data after quitting the game
 * Handles the loading of saved data if available on launch
 */
class CommonManager {

    private:
    Grid game_grid;
    Analyzer analyzer;

    CommonManager();

    std::string save_file_path;
    /**
     * @param eng required for providing the absolute path to the
     * compiled binary
     */
    auto GetSaveFilePath(const single::Engine& eng) -> std::string;
    void WriteToSaveFile();

    public:
    ~CommonManager();

    CommonManager(const CommonManager& copy_src) = delete;
    CommonManager& operator=(const CommonManager& copy_src) = delete;

    static auto GetInstance() -> CommonManager&;

    auto GetGrid() -> Grid& {return game_grid;}
    auto GetAnalyzer() -> Analyzer& {return analyzer;}

    void LoadSavedData(const single::Engine& eng);

    int credits;
    int bet;

    int free_spins;
    int free_spins_winnings;
    bool free_spins_mode;

    bool extra_reel_mode;
    bool reverse_lines;
};
