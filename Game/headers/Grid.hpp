#pragma once

#include "../../Singleton/Singleton.hpp"
#include <vector>

enum class CellContent {
    cherry,
    lemon,
    orange,
    bell,
    seven,
    diamond,
    wild,
    empty
};

struct Cell {
    int row;
    SDL_FRect location;
    int next_y_pos;
    CellContent content;
};

struct GridData {
    int rows;
    int columns;
    int cell_size;
};

class Reel {

    private:
    std::vector<Cell> cells;
    int reel_x_pos;
    int reel_y_pos;

    bool animation_finished;

    void ResetCell(single::Engine& eng, Cell& cell);
    CellContent RandomContent(single::Engine& eng, int last_idx) const;

    void SetCellRow(GridData grid_data, Cell& cell);
    
    public:
    Reel(int x_pos, GridData grid_data, CellContent starting_content = CellContent::empty);

    void StartReelSpin(single::Engine& eng,GridData grid_data);
    void SpinReel(single::Engine& eng, GridData grid_data, double speed, double delta_time, bool reeling);

    const Cell& GetCellAt(GridData grid_data, int row) const;
    bool AnimationFinished() const {return animation_finished;}
};

class Grid {

    private:
    std::vector<Reel> reels;
    std::vector<single::Sprite> sprites;

    GridData data;

    double animation_delay;
    int active_reels;

    public:
    Grid(int rows, int columns, int cell_size);

    void PrepareReelSpin(single::Engine& eng);
    void SpinReels(single::Engine& eng, double speed, double delata_time, bool reeling);

    void RenderGrid(single::Engine& eng);
    const std::vector<CellContent> ExportState() const;

    const GridData& GetGridData() const {return data;}
    bool ReelingFinished() const;
};