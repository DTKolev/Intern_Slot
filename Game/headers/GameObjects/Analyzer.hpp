#pragma once

#include "Grid.hpp"
#include <vector>
#include <unordered_map>

enum class Direction {
    forward,
    up,
    down
};

struct Line {
    int line_begin_row;
    std::vector<Direction> line_directions;
};

struct Combination {
    CellContent type;
    int matching_symbols;
};

// Calculates the win multiplier based on the current contents of the game grid
//
// Defines the paylines of the game
// Contains the paytable for the different symbol combinations
class Analyzer {

    private:
    std::unordered_map<CellContent, std::vector<int>> pay_table;

    std::vector<Line> lines;
    std::vector<Line> winning_lines;

    std::vector<int> used_cell_indecies;

    int scatters;

    auto LineCombination(const Line& ln, const Grid& game_grid, bool reverse_lines) -> Combination;
    auto CombinationMultiplier(const Combination& combination) const -> int;

    public:
    Analyzer();

    auto CalculateMultiplier(const Grid& game_grid, bool reverse_lines) -> int;
    auto GetWinningLines() const -> const std::vector<Line>&;
};
