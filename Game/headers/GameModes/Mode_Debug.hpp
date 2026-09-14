#pragma once

#include "Singleton.hpp"
#include "GameObjects/Grid.hpp"
#include "Singleton_Engine.hpp"
#include "Singleton_GameState.hpp"

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
	auto ExportGridData() -> GridData const;
};

class DebugPickCell : public single::GameState {

public:
	void OnEntry(const single::Engine& eng) override;
	void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
	void Update(single::Engine& eng, double delta_t) override;
	void Render() override;
	void OnExit() override;
};

class DebugSetCell : public single::OverlayState {

public:
	void OnEntry(const single::Engine& eng) override;
	void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
	void Update(single::Engine& eng, double delta_t) override;
	void Render() override;
	void OnExit() override;
};
