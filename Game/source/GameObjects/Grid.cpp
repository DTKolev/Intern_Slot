#include "GameObjects/Grid.hpp"
#include "Singleton_Visualizer.hpp"

Grid::Grid(float x, float y, int rows, int columns, float cell_size) :
    data{rows, columns, x, y, cell_size}, reeling_finished{false} 
{
    reels.reserve(columns);

    for (int i = 0; i < columns; i++) {

        float reel_x_pos = data.grid_x + (float)i * cell_size;
        Reel new_reel {reel_x_pos, data};
        reels.push_back(new_reel);
    }
}



void Grid::PrepareReelSpin(const single::Engine& eng) {

    animation_delay = 0.0;
    active_reels = reels.size();
    for (Reel& reel : reels) reel.StartReelSpin(eng, data);
}

void Grid::SpinReels(const single::Engine& eng, double delta_time, bool reeling) {

    if (!reeling) {
        animation_delay -= delta_time;
    
        if (animation_delay <= 0.0 && active_reels > 0) {
            active_reels--;
            animation_delay = (double)eng.RandomNumber(4, 2) / 10.0;
        }
    }

    for (int i = 0; i < reels.size(); i++) {

        constexpr double reeling_speed = 13.0;

        if (i >= reels.size() - active_reels) reels[i].SpinReel(eng, data, reeling_speed, delta_time, true);
        else reels[i].SpinReel(eng, data, reeling_speed, delta_time, false);
    }

    for (const Reel& reel : reels) {
        if (!reel.AnimationFinished()) {
            reeling_finished = false;
            break;
        }

        reeling_finished = true;
    }
}



void Grid::UpdateGridState() {

    grid_state.clear();
    grid_state_reverse.clear();
    cells.clear();

    for (int row = 0; row < data.rows; row++) {

        for (auto it = reels.begin(); it != reels.end(); it++) {

            const Cell& cell = it->GetCellAt(data, row);
            grid_state.push_back(cell.content);
            cells.push_back(cell);
        }

        for (auto rit = reels.rbegin(); rit != reels.rend(); rit++) {

            const Cell& cell = rit->GetCellAt(data, row);
            grid_state_reverse.push_back(cell.content);
        }
    }
}



void Grid::RenderGrid(const single::Visualizer& vis) {

    if (sprites.empty()) {
        sprites.assign({
            vis.LoadSprite("../src/cherry.png"),
            vis.LoadSprite("../src/lemon.png"),
            vis.LoadSprite("../src/orange.png"),
            vis.LoadSprite("../src/bell.png"),
            vis.LoadSprite("../src/seven.png"),
            vis.LoadSprite("../src/diamond.png"),
            vis.LoadSprite("../src/scatter.png"),
            vis.LoadSprite("../src/wild.png"),
            vis.LoadSprite("../src/empty.png"),
        });
    }

    if (bottom_pannel.Empty()) bottom_pannel = vis.LoadSprite("../src/wood.png");
    if (background.Empty()) background = vis.LoadSprite("../src/background.png");

    single::Rect background_rect {0.0f, 0.0f, 1000.0f, 600.0f};
    vis.RenderSprite(background, background_rect);

    for (const Reel& reel : reels) {
        reel.RenderCells(vis, data, sprites);
    }

    single::Rect bottom_pannel_rect {0.0f, 600.0f, 1000.0f, 200.0f};
    vis.RenderSprite(bottom_pannel, bottom_pannel_rect);

    single::Color border_color {238, 188, 29, 255}; // golden color
    float grid_x_size = (float)(data.cell_size * data.columns);
    float grid_y_size = (float)(data.cell_size * data.rows);
}



auto Grid::ScatterAmount() const -> int {

    int total_scatters = 0;
    for (const Reel& reel : reels) {
        total_scatters += reel.GetScatters(data);
    }

    return total_scatters;
}



void Grid::RelocateGrid(float new_x, float new_y, float new_cell_size) {

    float x_diff = new_x - data.grid_x;
    float size_diff = new_cell_size - data.cell_size;

    data.grid_x = new_x;
    data.grid_y = new_y;
    data.cell_size = new_cell_size;

    for (int i = 0; i < reels.size(); i++) {

        float prev_x = reels[i].GetPosX();
        float new_reel_x = prev_x + (float)i * size_diff + x_diff;
        reels[i].RelocateReel(new_reel_x, data);
    }
}

void Grid::AlignReels() {

    for (int i = 0; i < reels.size(); i++) {

        float reel_x = data.grid_x + (float)i * data.cell_size;
        reels[i].RelocateReel(reel_x, data);
    }
}



void Grid::AddExtraReel(float reel_x, CellContent starting_content) {

    reels.emplace_back(reel_x, data, starting_content);
    data.columns++;
}

void Grid::RemoveExtraReel() {

    reels.pop_back();
    data.columns--;
}

auto Grid::GetReel(int column) -> Reel& {

    return reels.at(column);
}