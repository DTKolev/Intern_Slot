#include "../headers/Analyzer.hpp"

Analyzer::Analyzer() {

    lines.assign({
        {0, 1, 2}, // top line
        {3, 4, 5}, // middle line (horizontal)
        {6, 7, 8}, // bottom line
        {0, 3, 6}, // left line
        {1, 4, 7}, // middle line (vertical)
        {2, 5, 8}, // right line
        {0, 4, 8}, // diagonal 1
        {6, 4, 2}  // diagonal 2
    });

    winning_combinations.assign({
        {{CellContent::cherry, CellContent::cherry, CellContent::cherry}, 2},   // 3x cherry
        {{CellContent::lemon, CellContent::lemon, CellContent::lemon}, 3},      // 3x lemon
        {{CellContent::orange, CellContent::orange, CellContent::orange}, 4},   // 3x orange
        {{CellContent::bell, CellContent::bell, CellContent::bell}, 5},         // 3x bell
        {{CellContent::seven, CellContent::seven, CellContent::seven}, 10},     // 3x seven
        {{CellContent::diamond, CellContent::diamond, CellContent::diamond}, 20}// 3x diamond
    });
}



std::vector<CellContent> Analyzer::LineSymbols(const Line& ln, const Grid& game_grid) const {

    std::vector<CellContent> new_combination;

    for (int idx : ln) {
        new_combination.push_back(game_grid.ExportState()[idx]);
    }

    return new_combination;
}

int Analyzer::CombinationMultiplier(const std::vector<CellContent>& ln_symbols) const {

    for (const Combination& comb : winning_combinations) {
        if (ln_symbols == comb.symbols) return comb.multiplier;
    }

    return 0;
}



int Analyzer::CalculateMultiplier(const Grid& game_grid) const {

    int multiplier = 0;

    for (const Line& ln : lines) {
        std::vector<CellContent> sample_combination = LineSymbols(ln, game_grid);
        multiplier += CombinationMultiplier(sample_combination);
    }

    return multiplier;
}