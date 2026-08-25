#include "../headers/Grid.hpp"
#include <iostream>

Reel::Reel(float x_pos, GridData grid_data, CellContent starting_content) 
: reel_x_pos{x_pos}, reel_y_pos{grid_data.grid_y -grid_data.cell_size}, distance_travelled{0.0f}, animation_finished{true} 
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
        if (cell.location.y >= (float)grid_data.rows * grid_data.cell_size - grid_data.cell_size / 10.0f) {
            ResetCell(eng, grid_data, cell);
            SetCellRow(grid_data, cell);
            break;
        }
    }
}



void Reel::SetCellRow(GridData grid_data, Cell& cell) {

    if (cell.location.y < grid_data.grid_y) cell.row = -1;
    else cell.row = (int)SDL_floorf(cell.location.y - grid_data.grid_y) / (int)grid_data.cell_size; 
}



void Reel::SpinReel(single::Engine& eng, GridData grid_data, double speed, double delta_time, bool reeling) {

    if (distance_travelled >= grid_data.cell_size) {

        float diff = distance_travelled - grid_data.cell_size;

        if (reeling) {

            for (Cell& cell : cells) {
                if (cell.location.y >= (float)grid_data.rows * grid_data.cell_size - grid_data.cell_size / 10.0f) {
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
            animation_finished = true;
            distance_travelled = 0.0;
        }
    }

    if (!animation_finished) {

        float distance_to_travel = grid_data.cell_size * (float)(speed * delta_time);

        for (Cell& cell : cells) {
            cell.location.y += distance_to_travel;
            SetCellRow(grid_data, cell);
        }
        distance_travelled += distance_to_travel;
    }
}




const Cell& Reel::GetCellAt(GridData grid_data, int row) const {

    for (const Cell& cell : cells) {

        if (cell.row == row) return cell;
    }

    return cells[0];
}



int Reel::GetScatters(GridData grid_data) const {

    int scatter_amount = 0;

    for (const Cell& cell : cells) {
        if (cell.row < grid_data.rows && cell.content == CellContent::scatter) scatter_amount++;
    }

    return scatter_amount;
}



void Reel::RenderCells(single::Engine& eng, GridData grid_data, std::vector<single::Sprite>& source_sprites) const {

    for (const Cell& cell : cells) {

        int sprite_idx = static_cast<int>(cell.content);
        eng.RenderSprite(source_sprites[sprite_idx], &cell.location);
    }

    single::Rect bottom_cover {
        .x = reel_x_pos,
        .y = grid_data.grid_y + (float)grid_data.rows * grid_data.cell_size,
        .w = grid_data.cell_size,
        .h = grid_data.cell_size
    };

    single::Rect top_cover {
        .x = reel_x_pos,
        .y = grid_data.grid_y - grid_data.cell_size,
        .w = grid_data.cell_size,
        .h = grid_data.cell_size
    };

    eng.RenderRect(top_cover, (single::Color){0, 0, 0, 255});
    eng.RenderRect(bottom_cover, (single::Color){0, 0, 0, 255});
}



void Reel::RelocateReel(float new_x, GridData grid_data) {

    reel_x_pos = new_x;
    reel_y_pos = grid_data.grid_y -grid_data.cell_size;

    for (Cell& cell : cells) {

        cell.location.x = new_x;
        cell.location.y = grid_data.grid_y + cell.row * grid_data.cell_size;
        cell.location.w = grid_data.cell_size;
        cell.location.h = grid_data.cell_size;
    }
}