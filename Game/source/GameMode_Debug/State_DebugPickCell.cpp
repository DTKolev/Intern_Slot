#include "GameModes/Mode_Debug.hpp"
#include "GameModes/Mode_FreeSpins.hpp"
#include "GameModes/Mode_Regular.hpp"
#include "GameObjects/CommonManager.hpp"
#include "GameObjects/DebugManager.hpp"
#include "GameObjects/Grid.hpp"
#include "GameObjects/InputManager.hpp"
#include <cmath>

void DebugPickCell::OnEntry(const single::Engine& eng) {

	Grid& debug_grid = debug_manager.GetGrid();
	GridData grid_data = debug_grid.GetGridData();

	debug_manager.ClearDebugInput();
	debug_manager.GetGrid().UpdateGridState();

	debug_manager.debug_config_available = false;
	debug_manager.extra_config_available = false;

	debug_manager.cursor = {0, 0};
}

void DebugPickCell::HandleInput(single::Engine& eng, SDL_Event& input_event) {

	GridData grid_data = debug_manager.GetGrid().GetGridData();

	input_manager.ProcessInput(input_event);

	if (input_manager.IsPressed(Key::up)) {
		debug_manager.cursor.row--;
		debug_manager.cursor.row = std::max(0, debug_manager.cursor.row);
	}
	else if (input_manager.IsPressed(Key::down)) {
		debug_manager.cursor.row++;
		debug_manager.cursor.row = std::min(grid_data.rows - 1, debug_manager.cursor.row);
	}
	else if (input_manager.IsPressed(Key::left)) {
		debug_manager.cursor.column--;
		debug_manager.cursor.column = std::max(0, debug_manager.cursor.column);
	}
	else if (input_manager.IsPressed(Key::right)) {
		debug_manager.cursor.column++;
		debug_manager.cursor.column = std::min(grid_data.columns - 1, debug_manager.cursor.column);
	}

	if (input_manager.IsReleased(Key::enter)) eng.AddOverlayState<DebugSetCell>();

	if (input_manager.IsReleased(Key::escape) || input_manager.IsReleased(Key::d)) {
		if (common_manager.free_spins_mode) eng.StateChange<FreeSpinsEntry>();
		else eng.StateChange<Betting>();
	}
}

void DebugPickCell::Update(single::Engine& eng, double delta_t) {

	if (debug_manager.scatters_set == 2 && !debug_manager.extra_reel_added && !common_manager.free_spins_mode) {

		GridData grid_data = debug_manager.GetGrid().GetGridData();
		debug_manager.GetGrid().AddExtraReel(grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size);

		debug_manager.extra_reel_added = true;
	}
	else if (debug_manager.scatters_set != 2 && debug_manager.extra_reel_added) {

		Grid& debug_grid = debug_manager.GetGrid();
		GridData grid_data = debug_grid.GetGridData();

		if (debug_grid.GetReel(grid_data.columns - 1).GetScatters(grid_data) == 0) {
			debug_manager.GetGrid().RemoveExtraReel();
			debug_manager.extra_reel_added = false;
		}
	}

}

void DebugPickCell::Render() {

	Grid& debug_grid = DebugManager::GetInstance().GetGrid();
	debug_grid.RenderGrid(vis);
	debug_manager.RenderCursor(vis);

}

void DebugPickCell::OnExit() {

	for (const CellContent& sample : debug_manager.GetGrid().ExportState()) {
		if (sample != CellContent::empty) {
			debug_manager.debug_config_available = true;
			break;
		}
	}

	if (debug_manager.extra_reel_added) {
		for (const CellContent& sample : debug_manager.ExportExtraReelState()) {
			if (sample != CellContent::empty) {
				debug_manager.extra_config_available = true;
				break;
			}
		}
	}
}
