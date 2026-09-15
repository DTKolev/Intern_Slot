#pragma once

#include "Singleton.hpp"
#include "Singleton_Engine.hpp"
#include "Singleton_Visualizer.hpp"
#include <vector>

// All possible cell contents
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

// Represents a single cell in memory
struct Cell {
    int row;
    single::Rect location;
    CellContent content;
};

// Contains data about the dimensions of the grid
struct GridData {
    int rows;
    int columns;
    float grid_x;
    float grid_y;
    float cell_size;
};



// *******************************************************************
// Reel class - contains 'rows + 1' amount of cells and manages them
// *******************************************************************

class Reel {

private:
    // Reel contents
    std::vector<Cell> cells;

    // Reel location
    float reel_x_pos;
    float reel_y_pos;

    // Reel spin variables
    float distance_travelled;
    bool animation_finished;
    double acceleration_timer;
    auto AccelerationCurve(double min, double max, double time) const -> double;

    // Reeling outcome control
    std::vector<CellContent> spin_outcome;
    int set_cells;
    void SetOutcome(const single::Engine& eng, bool config_available, const std::vector<CellContent>& target_outcome, const GridData& grid_data);

    // Cell manipulation
    void ResetCell(const single::Engine& eng, const GridData& grid_data, Cell& cell);
    auto RandomContent(const single::Engine& eng, const GridData& grid_data) const -> CellContent;
    void SetCellRow(const GridData& grid_data, Cell& cell);

    // Rendering utilities
    void RenderFrame(const single::Visualizer& vis, const GridData& grid_data) const;

public:
    // Class constructor
    Reel(float x_pos, const GridData& grid_data, CellContent starting_content = CellContent::empty);

    // Reel spin controls
    void StartReelSpin(const single::Engine& eng, const GridData& grid_data, bool config_available, const std::vector<CellContent>& target_outcome);
    void SpinReel(const single::Engine& eng, const GridData& grid_data, double speed, double delta_time, bool reeling);

    // Getters
    auto GetCellAt(const GridData& grid_data, int row) -> Cell&;
    auto GetScatters(const GridData& grid_data) const -> int;
    auto AnimationFinished() const -> bool {return animation_finished;}
    auto GetPosX() const -> float {return reel_x_pos;}

    // Rendering
    void RenderCells(const single::Visualizer& vis, const GridData& grid_data, std::vector<single::Sprite>& source_Sprites) const;

    // Reel re-location
    void RelocateReel(float new_x, const GridData& grid_data);
};



// *******************************************************************
// Grid class - contains 'columns' amount of reels and manages them
// *******************************************************************

class Grid {

private:
    // Grid contents
    std::vector<Reel> reels;

    // Visual elements
    std::vector<single::Sprite> sprites;
    single::Sprite bottom_pannel;
    single::Sprite background;

    // Reeling animation variables
    double animation_delay;
    double acceleration_timer;
    int active_reels;
    bool reeling_finished;

    // Exportable data
    GridData data;
    std::vector<CellContent> grid_state;
    std::vector<CellContent> grid_state_reverse;
    std::vector<Cell> cells;

    // Reeling outcome manipulation
    std::vector<CellContent> target_outcome;

    public:
    // Class constructor
    Grid(float x, float y, int rows, int columns, float cell_size);

    // Animation controls
    void PrepareReelSpin(const single::Engine& eng, bool config_available, const std::vector<CellContent>& target_state);
    void SpinReels(const single::Engine& eng, double delta_time, bool reeling);

    // Rendering
    void RenderGrid(const single::Visualizer& vis);

    // Update and export of grid data
    auto GetGridData() const -> const GridData& {return data;}
    void UpdateGridState();
    auto ExportState() const -> const std::vector<CellContent>& {return grid_state;}
    auto ExportStateReverse() const -> const std::vector<CellContent>& {return grid_state_reverse;}
    auto ExportCells() const -> const std::vector<Cell>& {return cells;}

    // Getters
    auto ScatterAmount() const -> int;
    auto ReelingFinished() const -> bool {return reeling_finished;}
    auto GetActiveReels() const -> int {return active_reels;}
    auto GetReel(int column) -> Reel&;
    auto GetCellAt(int row, int column) -> Cell&;

    // Grid re-location
    void RelocateGrid(float new_x, float new_y, float new_cell_size);
    void AlignReels();

    // Functionality for 'Extra Reel' mode
    void AddExtraReel(float reel_x, CellContent starting_content = CellContent::empty);
    void RemoveExtraReel();
};
