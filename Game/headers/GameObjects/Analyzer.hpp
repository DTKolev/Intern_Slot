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

/**
 * @brief Calculates the win multiplier based on the contents of the game grid
 *
 * Defines the paylines of the game
 * Contains the paytable for the symbol combinations
 */
class Analyzer {

    private:
    std::unordered_map<CellContent, std::vector<int>> pay_table;

    std::vector<Line> lines;
    std::vector<Line> winning_lines;

    std::vector<int> used_cell_indecies;

    int scatters;

    /**
     * @returns The combination type and the amount of matching symbols for
     * a certain line
     */
    auto LineCombination(const Line& ln, const Grid& game_grid, bool reverse_lines) -> Combination;
    auto CombinationMultiplier(const Combination& combination) const -> int;

    public:
    Analyzer();

    /**
     * @brief Calculates the combined multiplier from all paylines for the current
     * analyzer session
     *
     * Loops through all paylines, samples their combinations of symbols and sums up the multiplier
     *
     * @param reverse_lines if set to true, the analyzer will traverse the paylines
     * from right-to-left instead of left-to-right
     */
    auto CalculateMultiplier(const Grid& game_grid, bool reverse_lines) -> int;
    /**
     * @returns All lines that have acheived a winning
     * combination from the current analyzer session
     */
    auto GetWinningLines() const -> const std::vector<Line>&;
};
