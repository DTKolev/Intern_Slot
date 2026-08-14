#pragma once

#include "Grid.hpp"
#include <vector>

struct Combination {
    std::vector<CellContent> symbols;
    int multiplier;
};

class Analyzer {

    using Line = std::vector<int>;

    private:
    std::vector<Line> lines;
    std::vector<Combination> winning_combinations;

    std::vector<CellContent> LineSymbols(const Line& ln, const Grid& game_grid) const;
    int CombinationMultiplier(const std::vector<CellContent>& ln_symbols) const;

    public:
    Analyzer();

    int CalculateMultiplier(const Grid& game_grid) const;
};