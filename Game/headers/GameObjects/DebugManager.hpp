#pragma once

#include "Singleton.hpp"
#include "GameObjects/Grid.hpp"

struct Cursor {
	int row;
	int column;
};

class DebugManager {

private:
	Grid debug_grid;
	DebugManager();

public:
	static auto GetInstance() -> DebugManager&;
	auto GetGrid() -> Grid& {return debug_grid;}

	auto ExportState() -> std::vector<CellContent>;
	void ClearDebugInput();

	void RenderCursor(const single::Visualizer& vis) const;
	Cursor cursor;
	single::Color cursor_color;
};
