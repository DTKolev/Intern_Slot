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

class Analyzer {

    private:
    std::unordered_map<CellContent, std::vector<int>> pay_table;

    std::vector<Line> lines;
    std::vector<Line> winning_lines;

    int scatters;

    Combination LineCombination(const Line& ln, const Grid& game_grid, bool reverse_lines);

    int CombinationMultiplier(const Combination& combination) const;

    public:
    Analyzer();

    int CalculateMultiplier(const Grid& game_grid, bool reverse_lines);
    const std::vector<Line>& GetWinningLines() const;
};