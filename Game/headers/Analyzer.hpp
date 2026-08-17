#pragma once

#include "Grid.hpp"
#include <vector>
#include <unordered_map>

struct Combination {
    CellContent type;
    int matching_symbols;
};

class Analyzer {

    using Line = std::vector<int>;

    private:
    std::unordered_map<CellContent, std::vector<int>> pay_table;

    std::vector<Line> lines;

    Combination LineCombination(const Line& ln, const Grid& game_grid) const;

    int CombinationMultiplier(const Combination& combination) const;

    public:
    Analyzer();

    int CalculateMultiplier(const Grid& game_grid) const;
};