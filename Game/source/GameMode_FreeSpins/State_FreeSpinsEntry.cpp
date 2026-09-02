#include "GameModes/Mode_FreeSpins.hpp"
#include "GameObjects/CommonManager.hpp"
#include "GameObjects/InputManager.hpp"
#include <string>

void FreeSpinsEntry::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();

    prompt = vis.CreateText("PRESS ENTER TO START REELING", 32.0f, vis.RandomColor());
    bet = vis.CreateText("Bet: " + std::to_string(common_manager.bet), 32.0f);
    free_spins = vis.CreateText("Free Spins: " + std::to_string(common_manager.free_spins), 32.0f, (single::Color){0, 255, 0, 255});
} 

void FreeSpinsEntry::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::enter)) eng.StateChange<FreeSpinsReeling>();
}

void FreeSpinsEntry::Update(single::Engine& eng, double delta_t) {}

void FreeSpinsEntry::Render() {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.GetGrid().RenderGrid(vis);

    vis.RenderText(prompt, 50.0f, 610.0f);
    vis.RenderText(bet, 50.0f, 650.0f);
    vis.RenderText(free_spins, 260.0f, 650.0f);
}

void FreeSpinsEntry::OnExit() {}