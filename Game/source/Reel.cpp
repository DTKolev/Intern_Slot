#include "../headers/Grid.hpp"
#include <algorithm>
#include <iostream>

Reel::Reel(float x_pos, GridData grid_data, CellContent starting_content) 
: reel_x_pos{x_pos}, reel_y_pos{-grid_data.cell_size}, distance_travelled{0.0f}, animation_finished{true} 
{

    cells.reserve(grid_data.rows + 1);

    for (int i = 0; i < grid_data.rows + 1; i++) {

        float cell_y_pos = reel_y_pos + (float)i * grid_data.cell_size;
        single::Rect cell_location {reel_x_pos, cell_y_pos, grid_data.cell_size, grid_data.cell_size};

        cells.push_back((Cell){i - 1, cell_location, starting_content});
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



void Reel::ResetCell(single::Engine& eng, GridData grid_data, Cell& cell) {

    cell.location.y -= (float)(grid_data.rows + 1) * grid_data.cell_size;

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
        if (cell.location.y >= (float)grid_data.rows * grid_data.cell_size - 10.0f) {
            ResetCell(eng, grid_data, cell);
            SetCellRow(grid_data, cell);
            break;
        }
    }
}



void Reel::SetCellRow(GridData grid_data, Cell& cell) {

    if (cell.location.y < 0.0) cell.row = -1;
    else cell.row = (int)SDL_floorf(cell.location.y) / (int)grid_data.cell_size; 
}



void Reel::SpinReel(single::Engine& eng, GridData grid_data, double speed, double delta_time, bool reeling) {

    float distance_to_travel = speed * delta_time;

    if (distance_travelled >= grid_data.cell_size) {

        float diff = distance_travelled - grid_data.cell_size;

        if (reeling) {

            for (Cell& cell : cells) {
                if (cell.location.y >= (float)grid_data.rows * grid_data.cell_size) {
                    ResetCell(eng, grid_data, cell);
                    SetCellRow(grid_data, cell);
                    break;
                }
            }

            distance_travelled = diff;
        }
        else {

            for (Cell& cell : cells) {
                cell.location.y -= diff;
                cell.location.y = SDL_roundf(cell.location.y);
                SetCellRow(grid_data, cell);
            }
            PruneCells(grid_data);
            animation_finished = true;
            distance_travelled = 0.0;
        }
    }

    if (!animation_finished) {
        for (Cell& cell : cells) {
            cell.location.y += distance_to_travel;
            SetCellRow(grid_data, cell);
        }
        distance_travelled += distance_to_travel;
    }
}



void Reel::PruneCells(GridData grid_data) {

    float cell_size = grid_data.cell_size;
    std::vector<float> y_positions {0.0f, cell_size, 2.0f * cell_size, 3.0f * cell_size};
    std::vector<int> cell_indecies {0, 1, 2, 3};

    constexpr float reel_is_aligned = -1.0f;
    float missing_position = reel_is_aligned;

    // Align X
    for (Cell& cell : cells) {
        if (cell.location.x != reel_x_pos) cell.location.x = reel_x_pos;
    }

    // Align Y
    for (float y_pos : y_positions) {

        bool pos_found = false;
        for (int i = 0; i < cells.size(); i++) {
            const Cell& cell = cells[i];
            if (cell.location.y == y_pos) {
                
                pos_found = true;
                auto it = std::find(cell_indecies.begin(), cell_indecies.end(), i);
                if (it != cell_indecies.end()) cell_indecies.erase(it);
            }
        }

        if (!pos_found) {
            missing_position = y_pos;
            break;
        }
    }

    if (!cell_indecies.empty()) {
        std::cout << "Set the Y of cell " << cell_indecies[0] << " from " << cells.at(cell_indecies[0]).location.y << " to " << missing_position << '\n';
        cells.at(cell_indecies[0]).location.y = missing_position;
    }
}



const Cell& Reel::GetCellAt(GridData grid_data, int row) const {

    for (const Cell& cell : cells) {

        if (cell.row == row) return cell;
    }

    return cells[0];
}



void Reel::RenderCells(single::Engine& eng, std::vector<single::Sprite>& source_sprites) const {

    for (const Cell& cell : cells) {

        int sprite_idx = static_cast<int>(cell.content);
        eng.RenderSprite(source_sprites[sprite_idx], &cell.location);
    }
}