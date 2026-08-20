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

    pay_table[CellContent::cherry] = {2, 3, 4};
    pay_table[CellContent::lemon] = {3, 5, 8};
    pay_table[CellContent::orange] = {4, 7, 10};
    pay_table[CellContent::bell] = {5, 10, 15};
    pay_table[CellContent::seven] = {10, 20, 30};
    pay_table[CellContent::diamond] = {20, 35, 50};
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

    int pay_table_idx = combination.matching_symbols - 3;
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



int Analyzer::ScatterAmount(const Grid& game_grid) {

    scatters = 0;

    std::vector<CellContent> grid_state = game_grid.ExportState();
    for (const CellContent& sample : grid_state) {
        if (sample == CellContent::scatter) scatters++;
    }

    return scatters;
}