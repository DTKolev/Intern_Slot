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

void DebugPickCell::Update(single::Engine& eng, double delta_t) {}

void DebugPickCell::Render() {

	Grid& debug_grid = DebugManager::GetInstance().GetGrid();
	debug_grid.RenderGrid(vis);
	debug_manager.RenderCursor(vis);

}

void DebugPickCell::OnExit() {}
