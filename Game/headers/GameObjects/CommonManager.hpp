#pragma once

#include "Grid.hpp"
#include "Analyzer.hpp"
#include <string>

// Static manager class for the Slot Game
//
// Functions as a public container for storing game data
// All game states can read and write to/from it
//
// Handles the savig of the current game data after quitting the game and 
// loading saved data if available on launch
class CommonManager {

    private:
    Grid game_grid;
    Analyzer analyzer;

    CommonManager();

    std::string save_file_path;
    auto GetBinaryPath() const -> std::string;

    public:
    ~CommonManager();

    CommonManager(const CommonManager& copy_src) = delete;
    CommonManager& operator=(const CommonManager& copy_src) = delete;

    static auto GetInstance() -> CommonManager&;

    auto GetGrid() -> Grid& {return game_grid;}
    auto GetAnalyzer() -> Analyzer& {return analyzer;}

    int credits;
    int bet;

    int free_spins;
    int free_spins_winnings;
    bool free_spins_mode;

    bool extra_reel_mode;
    bool reverse_lines;
};
