#pragma once

#include "Singleton.hpp"
#include "Singleton_Engine.hpp"
#include "Singleton_Visualizer.hpp"
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

    void ResetCell(const single::Engine& eng, const GridData& grid_data, Cell& cell);
    auto RandomContent(const single::Engine& eng, int last_idx) const -> CellContent;
    void SetCellRow(const GridData& grid_data, Cell& cell);

    void RenderFrame(const single::Visualizer& vis, const GridData& grid_data) const;

    double acceleration_timer;
    auto AccelerationCurve(double min, double max, double time) const -> double;
    
    public:
    Reel(float x_pos, const GridData& grid_data, CellContent starting_content = CellContent::empty);

    void StartReelSpin(const single::Engine& eng, const GridData& grid_data);
    void SpinReel(const single::Engine& eng, const GridData& grid_data, double speed, double delta_time, bool reeling);

    auto GetCellAt(const GridData& grid_data, int row) const -> const Cell&;
    auto GetScatters(const GridData& grid_data) const -> int;
    auto AnimationFinished() const -> bool {return animation_finished;}

    void RenderCells(const single::Visualizer& vis, const GridData& grid_data, std::vector<single::Sprite>& source_Sprites) const;

    void RelocateReel(float new_x, const GridData& grid_data);
    auto GetPosX() const -> float {return reel_x_pos;}
};

class Grid {

    private:
    std::vector<Reel> reels;
    std::vector<single::Sprite> sprites;
    single::Sprite bottom_pannel;
    single::Sprite background;

    GridData data;

    double animation_delay;
    double acceleration_timer;
    int active_reels;

    std::vector<CellContent> grid_state;
    std::vector<CellContent> grid_state_reverse;
    std::vector<Cell> cells;
    bool reeling_finished;

    public:
    Grid(float x, float y, int rows, int columns, float cell_size);

    void PrepareReelSpin(const single::Engine& eng);
    void SpinReels(const single::Engine& eng, double delta_time, bool reeling);

    void RenderGrid(const single::Visualizer& vis);

    void UpdateGridState();
    auto ExportState() const -> const std::vector<CellContent>& {return grid_state;}
    auto ExportStateReverse() const -> const std::vector<CellContent>& {return grid_state_reverse;}
    auto ExportCells() const -> const std::vector<Cell>& {return cells;}

    auto GetGridData() const -> const GridData& {return data;}
    auto ScatterAmount() const -> int;
    auto ReelingFinished() const -> bool {return reeling_finished;}
    auto GetActiveReels() const -> int {return active_reels;}

    void RelocateGrid(float new_x, float new_y, float new_cell_size);
    void AlignReels();

    void AddExtraReel(float reel_x, CellContent starting_content = CellContent::empty);
    void RemoveExtraReel();
    auto GetReel(int column) -> Reel&;
};