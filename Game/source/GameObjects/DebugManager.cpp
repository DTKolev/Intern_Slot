#include "GameObjects/DebugManager.hpp"
#include "GameObjects/Grid.hpp"
#include <iostream>

DebugManager::DebugManager() :
	debug_grid{0.0f, 0.0f, 3, 5, 200.0f}, cursor {0, 0} , cursor_color{255, 0, 0, 255}
{

	GridData grid_data = debug_grid.GetGridData();

	float new_cell_size = 175.0f;

	float pos_x = 500.0f - ((float)grid_data.columns * new_cell_size) / 2.0f;
	float pos_y = 300.0f - ((float)grid_data.rows * new_cell_size) / 2.0f;
	debug_grid.RelocateGrid(pos_x, pos_x, new_cell_size);

	std::cout << pos_x << ' ' << pos_y << '\n';
}

auto DebugManager::GetInstance() -> DebugManager& {

	static DebugManager debug_manager;
	return debug_manager;
}

auto DebugManager::ExportState() -> std::vector<CellContent> {

	std::vector<CellContent> to_export = debug_grid.ExportState();
	ClearDebugInput();

	return to_export;
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

	GridData grid_data = debug_grid.GetGridData();

	for (int row = 0; row < grid_data.rows; row++) {
		for (int col = 0; col < grid_data.columns; col++) {
			debug_grid.GetCellAt(row, col).content = CellContent::empty;
		}
	}

	debug_grid.UpdateGridState();
}
