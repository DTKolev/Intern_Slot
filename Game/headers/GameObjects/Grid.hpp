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



/**
 * @brief Contains and manages the cells of a single column of the grid
 *
 * Handles the reeling animation
 * Assigns the cell content of the cells that it contains
 *
 * Handles assigning of random values during the reeling animation
 * Guarantees the desired reeling outcome (if provided)
 */
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
    /**
     * @brief Accepts input for the desired reeling outcome (if available) and
     * translates it into a usable symbol combination
     *
     * The input may contain empty cells, this function will replace them with
     * randomy-generated content and keep the other intentionally-provided
     * symbols
     *
     * @param eng provides RNG number
     * @param config_available the function will only look at the input if this
     * is toggled on; It will generate random content in all cells otherwise
     * @param target_outcome desired reeling outcome input
     */
    void SetOutcome(const single::Engine& eng, bool config_available, const std::vector<CellContent>& target_outcome, const GridData& grid_data);

    /**
     * @brief Move a cell back to the top of the reel and assign
     * random content to it
     */
    void ResetCell(const single::Engine& eng, const GridData& grid_data, Cell& cell);
    /**
     * @brief Draws a random cell content based on a weighted destribution
     *
     * Takes a purely random number from the engine and converts it into a valid
     * cell content index
     *
     * @param grid_data provides the information about the location of the reel
     * (if the reel if first, it may not contain a 'Wild' symbol)
     */
    auto RandomContent(const single::Engine& eng, const GridData& grid_data) const -> CellContent;
    void SetCellRow(const GridData& grid_data, Cell& cell);

    // Rendering utilities
    void RenderFrame(const single::Visualizer& vis, const GridData& grid_data) const;

public:
    // Class constructor
    Reel(float x_pos, const GridData& grid_data, CellContent starting_content = CellContent::empty);

    /**
     * @brief Assign starting values to the reeling variables
     *
     * Sets all animation variables to their starting values
     * Determines the outcome of the reeling
     */
    void StartReelSpin(const single::Engine& eng, const GridData& grid_data, bool config_available, const std::vector<CellContent>& target_outcome);
    /**
     * @brief Runs the reeling animation until reeling is finished and ensures that
     * the desired reeling outcome is acheived
     *
     * When reeling is finished, the reel performs 3 additional cell resets before
     * stopping, so that the 3 cells tha will be visible when the reeling is over
     * contain the cell contents from the desired reeling outcome
     *
     * @param speed the reel rotation speed measured in cell heights per second
     * @param reeling the reel will start its finishig sequence when this is
     * toggled off
     */
    void SpinReel(const single::Engine& eng, const GridData& grid_data, double speed, double delta_time, bool reeling);

    // Getters
    auto GetCellAt(const GridData& grid_data, int row) -> Cell&;
    auto GetScatters(const GridData& grid_data) const -> int;
    /**
     * @return returns true when the reel has stopped completely
     * after completing its finishing sequence
     */
    auto AnimationFinished() const -> bool {return animation_finished;}
    auto GetPosX() const -> float {return reel_x_pos;}

    // Rendering
    void RenderCells(const single::Visualizer& vis, const GridData& grid_data, std::vector<single::Sprite>& source_Sprites) const;

    // Reel re-location
    void RelocateReel(float new_x, const GridData& grid_data);
};

/**
 * @brief Contains and manages multiple reels
 *
 * Orders the rendering of the reels and stores the sprites for the
 * different cell contents
 * Controls the start/end of the reeling animation
 *
 * Tracks its state and is able to export it for usage in other parts of the program
 *
 * Handles the desired reeling outcome by separating it column-by-column and
 * feeding it to the appropriate reel
 */
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

    /**
     * @brief Orders all reels to prepare for the reeling animation
     */
    void PrepareReelSpin(const single::Engine& eng, bool config_available, const std::vector<CellContent>& target_state);
    /**
     * @brief Starts and stops the reeling animation at the appropriate time
     *
     * All reels start spinnin simoultaniously and get stopped one-by-one
     * when the grid receives the signal that the reeling should end
     *
     * @param reeling the grid will start stopping the reels when this is
     * toggled off
     */
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
    /**
     * @return returns true only when all reels have signaled
     * that they have stopped completely
     */
    auto ReelingFinished() const -> bool {return reeling_finished;}
    /**
     * @return returns the amound of reels that are currently spinning
     */
    auto GetActiveReels() const -> int {return active_reels;}
    auto GetReel(int column) -> Reel&;
    auto GetCellAt(int row, int column) -> Cell&;

    // Grid re-location
    void RelocateGrid(float new_x, float new_y, float new_cell_size);
    void AlignReels();

    /**
     * @brief Constructs an additional reel and updates its
     * grid data, so that it accounts for the extra reel as well
     *
     * @param reel_x the x-location where the reel should be
     * displayed at (the y-location is the same as the other reels)
     */
    void AddExtraReel(float reel_x, CellContent starting_content = CellContent::empty);
    void RemoveExtraReel();
};
