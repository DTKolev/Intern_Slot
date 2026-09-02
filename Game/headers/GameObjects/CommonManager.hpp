#pragma once

#include "Grid.hpp"
#include "Analyzer.hpp"

class CommonManager {

    private:
    Grid game_grid;
    Analyzer analyzer;

    CommonManager();

    public:
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