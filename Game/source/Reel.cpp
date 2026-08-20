#include "../headers/Grid.hpp"
#include <bitset>

Reel::Reel(int x_pos, GridData grid_data, CellContent starting_content) 
: reel_x_pos{x_pos}, reel_y_pos{-grid_data.cell_size}, animation_finished{true} 
{

    cells.reserve(grid_data.rows + 1);

    for (int i = 0; i < grid_data.rows + 1; i++) {

        int cell_y_pos = reel_y_pos + i * grid_data.cell_size;
        single::Rect cell_location {reel_x_pos, cell_y_pos, grid_data.cell_size, grid_data.cell_size};

        cells.push_back((Cell){i - 1, cell_location, cell_location.y + grid_data.cell_size, starting_content});
    }
}



CellContent Reel::RandomContent(single::Engine& eng, int last_idx) const {

    // Values have been calculated externally to acheive wheighted randomness when picking cell content
    constexpr float scalar = 0.035f;

    last_idx++;
    float highest_base = SDL_sqrtf((float)last_idx / scalar);
    int rng_high = (int)SDL_floorf(highest_base * 10.0);
    
    float random_base = (float)eng.RandomNumber(rng_high) / 10.0f;
    float product = scalar * (random_base * random_base);

    return static_cast<CellContent>((int)(SDL_floorf(product)));
}



void Reel::ResetCell(single::Engine& eng, Cell& cell) {

    cell.location.y = -cell.location.h;
    cell.next_y_pos = cell.location.y + cell.location.h;

    bool reel_has_scatter = false;
    for (const Cell& cell : cells) {
        if (cell.content == CellContent::scatter) {
            reel_has_scatter = true;
            break;
        }
    }

    bool correct_symbol_found;
    do {
        correct_symbol_found = true;

        int last_idx = static_cast<int>(CellContent::wild);
        if (reel_x_pos == 0) last_idx--;

        cell.content = RandomContent(eng, last_idx);
        if (reel_has_scatter && cell.content == CellContent::scatter) correct_symbol_found = false;
    } while (!correct_symbol_found);
}



void Reel::StartReelSpin(single::Engine& eng, GridData grid_data) {

    animation_finished = false;
    
    for (Cell& cell : cells) {
        cell.next_y_pos = cell.location.y + cell.location.h;
        if (cell.location.y >= grid_data.cell_size * grid_data.rows) ResetCell(eng, cell);
    }
}



void Reel::SetCellRow(GridData grid_data, Cell& cell) {

    if (cell.location.y < 0) cell.row = -1;
    else cell.row = cell.location.y / grid_data.cell_size; 
}



void Reel::SpinReel(single::Engine& eng, GridData grid_data, bool reeling) {

    std::bitset<4> cell_alingment_state;
    int set_idx = 0;

    for (Cell& cell : cells) {

        if ((cell.location.y == grid_data.cell_size * grid_data.rows) && reeling) {
            ResetCell(eng, cell);
        }
        
        if (!animation_finished) cell.location.y++;

        SetCellRow(grid_data, cell);

        if (cell.location.y == cell.next_y_pos) {
            if (reeling) {
                cell.next_y_pos += grid_data.cell_size;
            }
            else {
                cell.location.y = cell.row * grid_data.cell_size;
                cell_alingment_state[set_idx] = true;
            }
        }

        set_idx++;
    }

    if (cell_alingment_state.all()) animation_finished = true;
}



const Cell& Reel::GetCellAt(GridData grid_data, int row) const {

    for (const Cell& cell : cells) {

        if (cell.row == row) return cell;
    }

    return cells[0];
}