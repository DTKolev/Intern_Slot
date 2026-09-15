#include "GameObjects/DebugManager.hpp"
#include "GameObjects/Grid.hpp"
#include <vector>

DebugManager::DebugManager() :
	debug_grid{20.0f, 0.0f, 3, 5, 160.0f}, 
	cursor {0, 0} , cursor_color{255, 0, 0, 255}, 
	debug_config_available{false}, 
	scatters_set{0}, extra_reel_added{false}
{
	GridData grid_data = debug_grid.GetGridData();

	float pos_y = 300.0f - ((float)grid_data.rows * grid_data.cell_size) / 2.0f;

	debug_grid.RelocateGrid(grid_data.grid_x, pos_y, grid_data.cell_size);
}

auto DebugManager::GetInstance() -> DebugManager& {

	static DebugManager debug_manager;
	return debug_manager;
}

auto DebugManager::ExportState() -> std::vector<CellContent> {

	if (!extra_reel_added) {
		return debug_grid.ExportState();
	}
	else {
		GridData grid_data = debug_grid.GetGridData();
		std::vector<Cell>cells = debug_grid.ExportCells();
		std::vector<CellContent> output;

		for (int row = 0; row < grid_data.rows; row++) {
			for (int col = 0; col < grid_data.columns - 1; col++) {
				output.push_back(debug_grid.GetCellAt(row, col).content);
			}
		}

		return output;
	}
}

auto DebugManager::ExportExtraReelState() -> std::vector<CellContent> {

	extra_reel_outcome.clear();

	if (extra_reel_added) {
		GridData grid_data = debug_grid.GetGridData();
		Reel& extra_reel = debug_grid.GetReel(grid_data.columns - 1);

		for (int row = 0; row < grid_data.rows; row++) {
			extra_reel_outcome.push_back(extra_reel.GetCellAt(grid_data, row).content);
		}
	}

	return extra_reel_outcome;
}

void DebugManager::RenderCursor(const single::Visualizer& vis) const {

	GridData grid_data = debug_grid.GetGridData();

	float cursor_x = grid_data.grid_x + (float)cursor.column * grid_data.cell_size;
	float cursor_y = grid_data.grid_y + (float)cursor.row * grid_data.cell_size;

	vis.RenderLine(cursor_x, cursor_y, cursor_x + grid_data.cell_size, cursor_y, 10.0f, cursor_color);
	vis.RenderLine(cursor_x, cursor_y, cursor_x, cursor_y + grid_data.cell_size, 10.0f, cursor_color);
	vis.RenderLine(cursor_x, cursor_y + grid_data.cell_size, cursor_x + grid_data.cell_size, cursor_y + grid_data.cell_size, 10.0f, cursor_color);
	vis.RenderLine(cursor_x + grid_data.cell_size, cursor_y, cursor_x + grid_data.cell_size, cursor_y + grid_data.cell_size, 10.0f, cursor_color);
}

void DebugManager::ClearDebugInput() {

	if (extra_reel_added) {
		debug_grid.RemoveExtraReel();
		scatters_set = 0;
		extra_reel_added = false;
	}

	GridData grid_data = debug_grid.GetGridData();

	for (int row = 0; row < grid_data.rows; row++) {
		for (int col = 0; col < grid_data.columns; col++) {
			debug_grid.GetCellAt(row, col).content = CellContent::empty;
		}
	}

	debug_grid.UpdateGridState();
}
