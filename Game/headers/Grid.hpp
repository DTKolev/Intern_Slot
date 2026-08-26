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
    float grid_x;
    float grid_y;
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
    
    public:
    Reel(float x_pos, GridData grid_data, CellContent starting_content = CellContent::empty);

    void StartReelSpin(single::Engine& eng,GridData grid_data);
    void SpinReel(single::Engine& eng, GridData grid_data, double speed, double delta_time, bool reeling);

    const Cell& GetCellAt(GridData grid_data, int row) const;
    int GetScatters(GridData grid_data) const;
    bool AnimationFinished() const {return animation_finished;}

    void RenderCells(single::Engine& eng, GridData grid_data, std::vector<single::Sprite>& source_Sprites) const;

    void RelocateReel(float new_x, GridData grid_data);
    float GetPosX() const {return reel_x_pos;}
};

class Grid {

    private:
    std::vector<Reel> reels;
    std::vector<single::Sprite> sprites;
    single::Sprite bottom_pannel;
    single::Sprite background;

    float x_pos;
    float y_pos;
    GridData data;

    double animation_delay;
    int active_reels;

    public:
    Grid(float x, float y, int rows, int columns, float cell_size);

    void PrepareReelSpin(single::Engine& eng);
    void SpinReels(single::Engine& eng, double delta_time, bool reeling);

    void RenderGrid(single::Engine& eng);
    const std::vector<CellContent> ExportState() const;
    const std::vector<CellContent> ExportStateReverse() const;
    const std::vector<Cell> ExportCells() const;

    const GridData& GetGridData() const {return data;}
    int ScatterAmount() const;
    bool ReelingFinished() const;
    int GetActiveReels() const {return active_reels;}

    void RelocateGrid(float new_x, float new_y, float new_cell_size);
    void AlignReels();

    void AddExtraReel(float reel_x, CellContent starting_content = CellContent::empty);
    void RemoveExtraReel();
    Reel& GetReel(int column);
};