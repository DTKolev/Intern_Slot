#pragma once

#include "GameObjects/Grid.hpp"
#include "Singleton.hpp"
#include "GameObjects/DebugManager.hpp"
#include "GameObjects/CommonManager.hpp"
#include "GameObjects/InputManager.hpp"
#include "Singleton_Engine.hpp"
#include "Singleton_GameState.hpp"



class DebugPickCell : public single::GameState {

private:
	CommonManager& common_manager = CommonManager::GetInstance();
	InputManager& input_manager = InputManager::GetInstance();
	DebugManager& debug_manager = DebugManager::GetInstance();

public:
	void OnEntry(const single::Engine& eng) override;
	void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
	void Update(single::Engine& eng, double delta_t) override;
	void Render() override;
	void OnExit() override;
};

class DebugSetCell : public single::OverlayState {

private:
	CommonManager& common_manager = CommonManager::GetInstance();
	InputManager& input_manager = InputManager::GetInstance();
	DebugManager& debug_manager = DebugManager::GetInstance();

	CellContent sample_cell_content;
	bool reel_has_scatter;

	auto SetContent(CellContent start, bool increment) const -> CellContent;

public:
	void OnEntry(const single::Engine& eng) override;
	void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
	void Update(single::Engine& eng, double delta_t) override;
	void Render() override;
	void OnExit() override;
};
