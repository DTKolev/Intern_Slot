#include "GameObjects/Grid.hpp"

Grid::Grid(float x, float y, int rows, int columns, float cell_size) :
    x_pos{x}, y_pos{y}, data{rows, columns, x, y, cell_size} 
{
    reels.reserve(columns);

    for (int i = 0; i < columns; i++) {

        float reel_x_pos = x_pos + (float)i * cell_size;
        Reel new_reel {reel_x_pos, data};
        reels.push_back(new_reel);
    }
}



void Grid::PrepareReelSpin(single::Engine& eng) {

    animation_delay = 0.0;
    active_reels = reels.size();
    for (Reel& reel : reels) reel.StartReelSpin(eng, data);
}

void Grid::SpinReels(single::Engine& eng, double delta_time, bool reeling) {

    if (!reeling) {
        animation_delay -= delta_time;
    
        if (animation_delay <= 0.0 && active_reels > 0) {
            active_reels--;
            animation_delay = (double)eng.RandomNumber(4, 2) / 10.0;
        }
    }

    for (int i = 0; i < reels.size(); i++) {

        constexpr double reeling_speed = 10.0;

        if (i >= reels.size() - active_reels) reels[i].SpinReel(eng, data, reeling_speed, delta_time, true);
        else reels[i].SpinReel(eng, data, reeling_speed, delta_time, false);
    }
}




void Grid::RenderGrid(single::Engine& eng) {

    if (sprites.empty()) {
        sprites.assign({
            eng.LoadSprite("../src/cherry.png"),
            eng.LoadSprite("../src/lemon.png"),
            eng.LoadSprite("../src/orange.png"),
            eng.LoadSprite("../src/bell.png"),
            eng.LoadSprite("../src/seven.png"),
            eng.LoadSprite("../src/diamond.png"),
            eng.LoadSprite("../src/scatter.png"),
            eng.LoadSprite("../src/wild.png"),
            eng.LoadSprite("../src/empty.png"),
        });
    }

    if (bottom_pannel.Empty()) bottom_pannel = eng.LoadSprite("../src/wood.png");
    if (background.Empty()) background = eng.LoadSprite("../src/background.png");

    single::Rect background_rect {0.0f, 0.0f, 1000.0f, 600.0f};
    eng.RenderSprite(background, &background_rect);

    eng.EnableClippedRendering((single::Rect){0.0f, data.grid_y, 1000.0f, (float)data.rows * data.cell_size});
    for (const Reel& reel : reels) {
        reel.RenderCells(eng, data, sprites);
    }
    eng.DisableClipping();

    single::Rect bottom_pannel_rect {0.0f, 600.0f, 1000.0f, 200.0f};
    eng.RenderSprite(bottom_pannel, &bottom_pannel_rect);

    single::Color border_color {238, 188, 29, 255}; // golden color
    float grid_x_size = (float)(data.cell_size * data.columns);
    float grid_y_size = (float)(data.cell_size * data.rows);
}



const std::vector<CellContent> Grid::ExportState() const {

    std::vector<CellContent> grid_state;
    grid_state.reserve(data.rows * data.columns);

    for (int row = 0; row < data.rows; row++) {
        for (auto it = reels.begin(); it != reels.end(); it++) {

            const Cell& cell = it->GetCellAt(data, row);
            grid_state.push_back(cell.content);
        }
    }

    return grid_state;
}

const std::vector<CellContent> Grid::ExportStateReverse() const {

    std::vector<CellContent> grid_state;
    grid_state.reserve(data.rows * data.columns);

    for (int row = 0; row < data.rows; row++) {
        for (auto it = reels.rbegin(); it != reels.rend(); it++) {
            
            const Cell& cell = it->GetCellAt(data, row);
            grid_state.push_back(cell.content);
        }
    }
    
    return grid_state;
}



const std::vector<Cell> Grid::ExportCells() const {

    std::vector<Cell> cells;
    cells.reserve(data.rows * data.columns);

    for (int row = 0; row < data.rows; row++) {
        for (const Reel& reel : reels) {

            const Cell& cell = reel.GetCellAt(data, row);
            cells.push_back(cell);
        }
    }

    return cells;
}



int Grid::ScatterAmount() const {

    int total_scatters = 0;
    for (const Reel& reel : reels) {
        total_scatters += reel.GetScatters(data);
    }

    return total_scatters;
}



bool Grid::ReelingFinished() const {

    for (const Reel& reel : reels) {
        if (!reel.AnimationFinished()) {
            return false;
        }
    }

    return true;
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

Reel& Grid::GetReel(int column) {

    return reels.at(column);
}