#include "GameObjects/Grid.hpp"
#include "Singleton_Common.hpp"
#include "Singleton_Visualizer.hpp"
#include <cmath>

Reel::Reel(float x_pos, const GridData& grid_data, CellContent starting_content) 
: reel_x_pos{x_pos}, reel_y_pos{grid_data.grid_y -grid_data.cell_size}, distance_travelled{0.0f}, animation_finished{true} 
{

    cells.reserve(grid_data.rows + 1);

    for (int i = 0; i < grid_data.rows + 1; i++) {

        float cell_y_pos = reel_y_pos + (float)i * grid_data.cell_size;
        single::Rect cell_location {reel_x_pos, cell_y_pos, grid_data.cell_size, grid_data.cell_size};

        cells.push_back((Cell){i - 1, cell_location, starting_content});
    }
}



auto Reel::RandomContent(const single::Engine& eng, int last_idx) const -> CellContent {

    // Values have been calculated externally to acheive wheighted randomness when picking cell content
    last_idx++;

    float max_x = std::sqrt((float)last_idx) - 0.05;
    int rng_high = (int)std::floor(max_x * 10.0f);

    int rng_base = eng.RandomNumber(rng_high);
    float base_x = (float)rng_base / 10.0f;

    int content_idx = (int)std::floor(base_x * base_x);

    return static_cast<CellContent>(content_idx);
}



void Reel::ResetCell(const single::Engine& eng, const GridData& grid_data, Cell& cell) {

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
        
        float last_column_x = grid_data.grid_x + (float)(grid_data.columns - 1) * grid_data.cell_size;

        if (reel_x_pos == grid_data.grid_x || reel_x_pos == last_column_x) last_idx--;

        cell.content = RandomContent(eng, last_idx);
        if (reel_has_scatter && cell.content == CellContent::scatter) correct_symbol_found = false;
    } while (!correct_symbol_found);
}



auto Reel::AccelerationCurve(double min, double max, double time) const -> double {

    double fraction = 2.0 * (time * time * time) - (time * time);
    double result = min + (max - min) * fraction;

    if (result > max) return max;
    else return result;
}



void Reel::StartReelSpin(const single::Engine& eng, const GridData& grid_data) {

    animation_finished = false;
    acceleration_timer = 0.0;
    distance_travelled = 0.0f;
    
    for (Cell& cell : cells) {
        if (cell.location.y >= (grid_data.grid_y + (float)grid_data.rows * grid_data.cell_size) - grid_data.cell_size / 5.0f) {
            ResetCell(eng, grid_data, cell);
            SetCellRow(grid_data, cell);
            break;
        }
    }
}



void Reel::SetCellRow(const GridData& grid_data, Cell& cell) {

    if (cell.location.y < grid_data.grid_y) cell.row = -1;
    else cell.row = (int)std::ceil(cell.location.y - grid_data.grid_y) / (int)grid_data.cell_size; 
}



void Reel::SpinReel(const single::Engine& eng, const GridData& grid_data, double speed, double delta_time, bool reeling) {

    if (distance_travelled >= grid_data.cell_size) {

        float diff = distance_travelled - grid_data.cell_size;

        if (reeling) {

            for (Cell& cell : cells) {
                if (cell.location.y >= (grid_data.grid_y + (float)grid_data.rows * grid_data.cell_size) - grid_data.cell_size / 5.0f) {
                    ResetCell(eng, grid_data, cell);
                    SetCellRow(grid_data, cell);
                    break;
                }
            }

            distance_travelled = diff;
        }
        else {

            for (Cell& cell : cells) {
                cell.location.y = SDL_roundf(cell.location.y);
                cell.location.y = grid_data.grid_y + cell.row * grid_data.cell_size;
            }
            animation_finished = true;
            distance_travelled = 0.0;
        }
    }

    if (!animation_finished) {

        acceleration_timer += delta_time * 2.5;
        double real_speed = AccelerationCurve(0.0, speed, acceleration_timer);

        float distance_to_travel = grid_data.cell_size * (float)(real_speed * delta_time);

        for (Cell& cell : cells) {
            cell.location.y += distance_to_travel;
            SetCellRow(grid_data, cell);
        }
        distance_travelled += distance_to_travel;
    }
}




auto Reel::GetCellAt(const GridData& grid_data, int row) const -> const Cell& {

    for (const Cell& cell : cells) {

        if (cell.row == row) return cell;
    }

    return cells[0];
}



auto Reel::GetScatters(const GridData& grid_data) const -> int {

    int scatter_amount = 0;

    for (const Cell& cell : cells) {
        if (cell.row < grid_data.rows && cell.content == CellContent::scatter) scatter_amount++;
    }

    return scatter_amount;
}



void Reel::RenderFrame(const single::Visualizer& vis, const GridData& grid_data) const {

    constexpr single::Color golden_brown {153, 101, 21, 255};
    constexpr single::Color shadow{76, 50, 10, 255};

    constexpr float thickness = 8.0f;

    float visible_reel_y = reel_y_pos + grid_data.cell_size;

    vis.RenderLine(
        reel_x_pos, visible_reel_y, 
        reel_x_pos + grid_data.cell_size, visible_reel_y, 
        thickness + 2.0f, shadow
    );
    vis.RenderLine(
        reel_x_pos, visible_reel_y, 
        reel_x_pos, visible_reel_y + (float)grid_data.rows * grid_data.cell_size, 
        thickness, shadow
    );
    vis.RenderLine(
        reel_x_pos + grid_data.cell_size, visible_reel_y,
        reel_x_pos + grid_data.cell_size, visible_reel_y + (float)grid_data.rows * grid_data.cell_size,
        thickness, shadow
    );
    vis.RenderLine(
        reel_x_pos, visible_reel_y + (float)grid_data.rows * grid_data.cell_size,
        reel_x_pos + grid_data.cell_size, visible_reel_y + (float)grid_data.rows * grid_data.cell_size,
        thickness + 2.0f, shadow
    );

    vis.RenderLine(
        reel_x_pos + 2.0f, visible_reel_y,
        reel_x_pos + grid_data.cell_size - 2.0f, visible_reel_y,
        thickness - 2.0f, golden_brown
    );
    vis.RenderLine(
        reel_x_pos, visible_reel_y + 2.0f,
        reel_x_pos, visible_reel_y + (float)grid_data.rows * grid_data.cell_size - 2.0f,
        thickness - 4.0f, golden_brown
    );
    vis.RenderLine(
        reel_x_pos + grid_data.cell_size, visible_reel_y + 2.0f,
        reel_x_pos + grid_data.cell_size, visible_reel_y + (float)grid_data.rows * grid_data.cell_size - 2.0f,
        thickness -  4.0f, golden_brown
    );
    vis.RenderLine(
        reel_x_pos + 2.0f, visible_reel_y + (float)grid_data.rows * grid_data.cell_size,
        reel_x_pos + grid_data.cell_size - 2.0f, visible_reel_y + (float)grid_data.rows * grid_data.cell_size,
        thickness - 2.0f, golden_brown
    );
}



void Reel::RenderCells(const single::Visualizer& vis, const GridData& grid_data, std::vector<single::Sprite>& source_sprites) const {

    vis.EnableClippedRendering((single::Rect){0.0f, grid_data.grid_y, 1000.0f, (float)grid_data.rows * grid_data.cell_size});
    for (const Cell& cell : cells) {

        int sprite_idx = static_cast<int>(cell.content);
        vis.RenderSprite(source_sprites[sprite_idx], cell.location);
    }

    float reel_visible_y = reel_y_pos + grid_data.cell_size;
    float reel_height = (float)grid_data.rows * grid_data.cell_size;

    single::Rect top_gradient_rect {
        .x = reel_x_pos,
        .y = reel_visible_y,
        .w = grid_data.cell_size,
        .h = reel_height / 6.0f
    };
    single::Rect bottom_gradient_rect {
        .x = reel_x_pos,
        .y = reel_visible_y + reel_height - reel_height / 6.0f,
        .w = grid_data.cell_size,
        .h = reel_height / 6.0f
    };

    vis.RenderGradient(top_gradient_rect, 0.5f, 0.0, single::GradientType::linear);
    vis.RenderGradient(bottom_gradient_rect, 0.5f, 180.0f, single::GradientType::linear);

    vis.DisableClipping();

    RenderFrame(vis, grid_data);
}



void Reel::RelocateReel(float new_x, const GridData& grid_data) {

    reel_x_pos = new_x;
    reel_y_pos = grid_data.grid_y -grid_data.cell_size;

    for (Cell& cell : cells) {

        cell.location.x = new_x;
        cell.location.y = grid_data.grid_y + cell.row * grid_data.cell_size;
        cell.location.w = grid_data.cell_size;
        cell.location.h = grid_data.cell_size;
    }
}