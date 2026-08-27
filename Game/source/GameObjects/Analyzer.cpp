#include "GameObjects/Analyzer.hpp"
#include <algorithm>
#include <cmath>

Analyzer::Analyzer() : scatters{0} {

    lines.assign({
        {0, {Direction::forward, Direction::forward, Direction::forward, Direction::forward}},
        {1, {Direction::forward, Direction::forward, Direction::forward, Direction::forward}},
        {2, {Direction::forward, Direction::forward, Direction::forward, Direction::forward}},
        {0, {Direction::down, Direction::down, Direction::up, Direction::up}},
        {2, {Direction::up, Direction::up, Direction::down, Direction::down}},
        {0, {Direction::down, Direction::forward, Direction::forward, Direction::down}},
        {2, {Direction::up, Direction::forward, Direction::forward, Direction::up}}
    });

    pay_table[CellContent::cherry] = {0, 0, 0, 1, 2, 5};
    pay_table[CellContent::lemon] = {0, 0, 0,1, 3, 10};
    pay_table[CellContent::orange] = {0, 0,0,2, 5, 20};
    pay_table[CellContent::bell] = {0, 0, 0, 2, 10, 50};
    pay_table[CellContent::seven] = {0, 0, 1, 3, 15, 100};
    pay_table[CellContent::diamond] = {0, 0, 1, 5, 20, 200};
}



bool Analyzer::CellHasBeenUsed(int cell_id) {

    if (std::find(used_cell_indecies.begin(), used_cell_indecies.end(), cell_id) != used_cell_indecies.end()) {
        return true;
    }
    else {
        used_cell_indecies.push_back(cell_id);
        return false;
    }
}



Combination Analyzer::LineCombination(const Line& ln, const Grid& game_grid, bool reverse_lines) {

    GridData grid_data = game_grid.GetGridData();

    std::vector<CellContent> grid_state;

    if (!reverse_lines) grid_state = game_grid.ExportState();
    else grid_state = game_grid.ExportStateReverse();

    int starting_cell = ln.line_begin_row * grid_data.columns;
    CellContent first_cell_content = grid_state[starting_cell];

    if (first_cell_content == CellContent::scatter) {
        return (Combination){CellContent::cherry, 0};
    }

    Combination new_combination {
        .type = first_cell_content,
        .matching_symbols = 1
    };

    bool all_cells_used = CellHasBeenUsed(starting_cell);
    int current_cell = starting_cell;
    for (const Direction& dir : ln.line_directions) {
        
        switch (dir) {
            case Direction::forward:
                current_cell++;
                break; 
            case Direction::down:
                current_cell += 1 + grid_data.columns;
                break;
            case Direction::up:
                current_cell += 1 - grid_data.columns;
                break;
        }

        if (grid_state[current_cell] == new_combination.type || grid_state[current_cell] == CellContent::wild) {
            new_combination.matching_symbols++;

            if (all_cells_used) {
                all_cells_used = CellHasBeenUsed(current_cell);
            }
        }
        else break;
    }

    if (all_cells_used) new_combination.matching_symbols = 1;

    return new_combination;
}



int Analyzer::CombinationMultiplier(const Combination& combination) const {

    if (combination.matching_symbols > 5) return pay_table.at(combination.type)[5];

    return pay_table.at(combination.type)[combination.matching_symbols];
}



int Analyzer::CalculateMultiplier(const Grid& game_grid, bool reverse_lines) {

    int multiplier = 0;
    winning_lines.clear();
    used_cell_indecies.clear();

    for (const Line& ln : lines) {
        Combination sample_combination = LineCombination(ln, game_grid, reverse_lines);
        int line_multiplier = CombinationMultiplier(sample_combination);

        multiplier += line_multiplier;
        if (line_multiplier > 0) winning_lines.push_back(ln);
    }

    return multiplier;
}



const std::vector<Line>& Analyzer::GetWinningLines() const {

    return winning_lines;
}