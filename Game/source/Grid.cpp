#include "../headers/Grid.hpp"

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
            eng.LoadSprite("../src/wood.png")
        });
    }

    for (const Reel& reel : reels) {
        reel.RenderCells(eng, sprites);
    }    

    single::Rect bottom_pannel_rect {0.0f, 600.0f, 1000.0f, 200.0f};
    int bottom_pannel_idx = sprites.size() - 1;
    eng.RenderSprite(sprites[bottom_pannel_idx], &bottom_pannel_rect);

    single::Color border_color {238, 188, 29, 255}; // golden color
    float grid_x_size = (float)(data.cell_size * data.columns);
    float grid_y_size = (float)(data.cell_size * data.rows);
    
    //eng.RenderLine(x_pos, y_pos, x_pos + grid_x_size, y_pos, 10.0f, border_color);
    //eng.RenderLine(x_pos, y_pos, x_pos, y_pos + grid_y_size, 10.0f, border_color);
    //eng.RenderLine(x_pos, y_pos + grid_y_size, x_pos + grid_x_size, y_pos + grid_y_size, 5.0f, border_color);
    //eng.RenderLine(x_pos + grid_x_size, y_pos, x_pos + grid_x_size, y_pos + grid_y_size, 10.0f, border_color);
}



const std::vector<CellContent> Grid::ExportState() const {

    std::vector<CellContent> grid_state;
    grid_state.reserve(data.rows * data.columns);

    for (int row = 0; row < data.rows; row++) {
        for (const Reel& reel : reels) {

            const Cell& cell = reel.GetCellAt(data, row);
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