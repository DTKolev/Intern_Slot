#include "../headers/Analyzer.hpp"

Analyzer::Analyzer() : scatters{0} {

    lines.assign({
        {0, 1, 2, 3, 4},
        {5, 6, 7, 8, 9},
        {10, 11, 12, 13, 14},
        {0, 6, 12, 8, 4},
        {10, 6, 2, 8, 14},
        {0, 6, 7, 8, 14},
        {10, 6, 7, 8, 4}
    });

    pay_table[CellContent::cherry] = {1, 2, 10};
    pay_table[CellContent::lemon] = {1, 3, 10};
    pay_table[CellContent::orange] = {2, 5, 20};
    pay_table[CellContent::bell] = {2, 10, 50};
    pay_table[CellContent::seven] = {1, 3, 20, 100};
    pay_table[CellContent::diamond] = {1, 5, 20, 180};
}



Combination Analyzer::LineCombination(const Line& ln, const Grid& game_grid) {

    std::vector<CellContent> grid_state = game_grid.ExportState();
    CellContent first_cell_content = grid_state[ln[0]];

    if (first_cell_content == CellContent::scatter) {
        return (Combination){CellContent::cherry, 0};
    }

    Combination new_combination {
        .type = first_cell_content,
        .matching_symbols = 0
    };

    for (const int idx : ln) {
        if (grid_state[idx] == first_cell_content || grid_state[idx] == CellContent::wild) new_combination.matching_symbols++;
        else break;
    }

    return new_combination;
}



int Analyzer::CombinationMultiplier(const Combination& combination) const {

    constexpr int max_matching = 5;
    int diff = max_matching - pay_table.at(combination.type).size() + 1;

    int pay_table_idx = combination.matching_symbols - diff;

    if (pay_table_idx < 0) return 0;

    return pay_table.at(combination.type)[pay_table_idx];
}



int Analyzer::CalculateMultiplier(const Grid& game_grid) {

    int multiplier = 0;
    winning_lines.clear();

    for (const Line& ln : lines) {
        Combination sample_combination = LineCombination(ln, game_grid);
        int line_multiplier = CombinationMultiplier(sample_combination);

        multiplier += line_multiplier;
        if (line_multiplier > 0) winning_lines.push_back(ln);
    }

    return multiplier;
}



const std::vector<Analyzer::Line>& Analyzer::GetWinningLines() const {

    return winning_lines;
}