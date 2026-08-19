#include "../headers/Grid.hpp"

Grid::Grid(int rows, int columns, int cell_size) :
    data{rows, columns, cell_size} 
{
    reels.reserve(columns);

    for (int i = 0; i < columns; i++) {

        int x_pos = i * cell_size;
        Reel new_reel {x_pos, data};
        reels.push_back(new_reel);
    }
}



void Grid::PrepareReelSpin(single::Engine& eng) {

    animation_delay = 0.25;
    active_reels = 1;
    for (Reel& reel : reels) reel.StartReelSpin(eng, data);
}

void Grid::SpinReels(single::Engine& eng, double speed, double delata_time, bool reeling) {

    animation_delay -= delata_time;
    if (animation_delay <= 0.0 && active_reels < reels.size()) {
        active_reels++;
        animation_delay = 0.25;
    }

    for (int i = 0; i < active_reels; i++) {
        reels[i].SpinReel(eng, data, speed, delata_time, reeling);
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
            eng.LoadSprite("../src/wild.png"),
            eng.LoadSprite("../src/empty.png"),
            eng.LoadSprite("../src/wood.png")
        });
    }

    for (const Reel& reel : reels) {
        for (int row = -1; row < data.rows; row++) {

            const Cell& cell = reel.GetCellAt(data, row);
            int sprite_idx = static_cast<int>(cell.content);

            eng.RenderSprite(sprites[sprite_idx], &cell.location);
        }
    }    

    SDL_FRect bottom_pannel_rect {0.0f, (float)(data.cell_size * data.rows), (float)(data.cell_size * data.columns), 200.0f};
    int bottom_pannel_idx = sprites.size() - 1;
    eng.RenderSprite(sprites[bottom_pannel_idx], &bottom_pannel_rect);

    single::Color border_color {238, 188, 29, 255}; // golden color
    float grid_x_size = (float)(data.cell_size * data.columns);
    float grid_y_size = (float)(data.cell_size * data.rows);
    
    eng.RenderLine(0.0f, 0.0f, grid_x_size, 0.0f, 10.0f, border_color);
    eng.RenderLine(0.0f, 0.0f, 0.0f, grid_y_size, 10.0f, border_color);
    eng.RenderLine(0.0f, grid_y_size, grid_x_size, grid_y_size, 5.0f, border_color);
    eng.RenderLine(grid_x_size, 0.0f, grid_x_size, grid_y_size, 10.0f, border_color);
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




bool Grid::ReelingFinished() const {

    for (const Reel& reel : reels) {
        if (!reel.AnimationFinished()) {
            return false;
        }
    }

    return true;
}