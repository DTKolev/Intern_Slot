#pragma once

#include "Singleton.hpp"
#include "GameObjects/Grid.hpp"

struct Cursor {
	int row;
	int column;
};

/**
 * @brief Stores and exports the data from the debug mode
 *
 * Manages a separate cell grid that is used as an interface
 * for debugging (picking the outcome for the next reeling)
 *
 * Stores the debug configuration and is able to export it
 * for usage in other parts of the program
 * Sets the flags for the availability of a debug configuration
 * to their appropriate values automatically
 *
 * Manages and displays a cursor for picking cells in the
 * debug grid
 */
class DebugManager {

private:
	Grid debug_grid;
	DebugManager();

public:
	static auto GetInstance() -> DebugManager&;
	auto GetGrid() -> Grid& {return debug_grid;}

	/**
	 * @brief Exports the states of the regular 5 reels of the grid
	 *
	 * If the debug configuration includes an extra reel, it will be
	 * exported separately
	 */
	auto ExportState() -> std::vector<CellContent>;
	void ClearDebugInput();

	void RenderCursor(const single::Visualizer& vis) const;
	Cursor cursor;
	single::Color cursor_color;
	
	bool debug_config_available;

	int scatters_set;
	bool extra_reel_added;
	bool extra_config_available;
	std::vector<CellContent> extra_reel_outcome;
	/**
	 * @brief Exports the debug configuration for the additional
	 * reel (if available)
	 */
	auto ExportExtraReelState() -> std::vector<CellContent>;
};
