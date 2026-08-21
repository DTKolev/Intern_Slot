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
    scatter,
    wild,
    empty
};

struct Cell {
    int row;
    single::Rect location;
    CellContent content;
};

struct GridData {
    int rows;
    int columns;
    float cell_size;
};

class Reel {

    private:
    std::vector<Cell> cells;
    float reel_x_pos;
    float reel_y_pos;

    float distance_travelled;

    bool animation_finished;

    void ResetCell(single::Engine& eng, GridData grid_data, Cell& cell);
    CellContent RandomContent(single::Engine& eng, int last_idx) const;

    void SetCellRow(GridData grid_data, Cell& cell);
    void PruneCells(GridData grid_data);
    
    public:
    Reel(float x_pos, GridData grid_data, CellContent starting_content = CellContent::empty);

    void StartReelSpin(single::Engine& eng,GridData grid_data);
    void SpinReel(single::Engine& eng, GridData grid_data, double speed, double delta_time, bool reeling);

    const Cell& GetCellAt(GridData grid_data, int row) const;
    bool AnimationFinished() const {return animation_finished;}

    void RenderCells(single::Engine& eng, std::vector<single::Sprite>& source_Sprites) const;
};

class Grid {

    private:
    std::vector<Reel> reels;
    std::vector<single::Sprite> sprites;

    GridData data;

    double animation_delay;
    int active_reels;

    public:
    Grid(int rows, int columns, float cell_size);

    void PrepareReelSpin(single::Engine& eng);
    void SpinReels(single::Engine& eng, double delta_time, bool reeling);

    void RenderGrid(single::Engine& eng);
    const std::vector<CellContent> ExportState() const;
    const std::vector<Cell> ExportCells() const;

    const GridData& GetGridData() const {return data;}
    bool ReelingFinished() const;
    int GetActiveReels() const {return active_reels;}
};