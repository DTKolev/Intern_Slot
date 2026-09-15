#include "GameModes/Mode_Debug.hpp"
#include "GameObjects/Grid.hpp"
#include "Singleton_Engine.hpp"

void DebugSetCell::OnEntry(const single::Engine& eng) {

	Grid& debug_grid = debug_manager.GetGrid();

	sample_cell_content = debug_grid.GetCellAt(debug_manager.cursor.row, debug_manager.cursor.column).content;

	if (debug_grid.GetReel(debug_manager.cursor.column).GetScatters(debug_grid.GetGridData()) != 0) {
		reel_has_scatter = true;
	}
	else reel_has_scatter = false;

	debug_manager.cursor_color = {0, 255, 0, 255};
}

void DebugSetCell::HandleInput(single::Engine& eng, SDL_Event& input_event) {

	input_manager.ProcessInput(input_event);

	if (input_manager.IsPressed(Key::up)) {
		sample_cell_content = SetContent(sample_cell_content, true);
	}
	else if (input_manager.IsPressed(Key::down)) {
		sample_cell_content = SetContent(sample_cell_content, false);
	}

	debug_manager.GetGrid().GetCellAt(debug_manager.cursor.row, debug_manager.cursor.column).content = sample_cell_content;
	debug_manager.GetGrid().UpdateGridState();

	if (input_manager.IsReleased(Key::enter)) eng.RemoveOverlayState();
}

void DebugSetCell::Update(single::Engine& eng, double delta_t) {}

void DebugSetCell::Render() {}

void DebugSetCell::OnExit() {

	debug_manager.cursor_color = {255, 0, 0, 255};
}



auto DebugSetCell::SetContent(CellContent start, bool increment) const -> CellContent {

	GridData grid_data = debug_manager.GetGrid().GetGridData();

	int change;
	if (increment) change = 1;
	else change = -1;

	const int last_idx = static_cast<int>(CellContent::empty);
	int current_content_idx = static_cast<int>(sample_cell_content);

	current_content_idx += change;
	current_content_idx %= last_idx + 1;

	bool reel_first_or_last = debug_manager.cursor.column == 0 || debug_manager.cursor.column == grid_data.columns - 1;
	
	bool correct_content_found;

	do {
		correct_content_found = true;

		if (reel_first_or_last && current_content_idx == static_cast<int>(CellContent::wild)) {
			current_content_idx += change;
			current_content_idx %= last_idx + 1;
			
			correct_content_found = false;
		}

		if (reel_has_scatter && current_content_idx == static_cast<int>(CellContent::scatter)) {
			current_content_idx += change;
			current_content_idx %= last_idx + 1;

			correct_content_found = false;
		}
	} while (!correct_content_found);

	if (current_content_idx < 0) current_content_idx = last_idx;
	
	return static_cast<CellContent>(current_content_idx);
}
