#include "GameModes/Mode_Regular.hpp"
#include "GameObjects/InputManager.hpp"
#include "GameObjects/CommonManager.hpp"

void Betting::OnEntry(const single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();

    credits = vis.CreateText("Credits: " + std::to_string(common_manager.credits), 32.0);
    bet = vis.CreateText("Bet: ", 32.0);
    title = vis.CreateText("BETTING", 32.0);
}

void Betting::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();
    CommonManager& common_manager = CommonManager::GetInstance();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::escape)) eng.StateChange<MainMenu>();

    if (input_manager.IsReleased(Key::up)) {
        if (common_manager.bet + 1 <= common_manager.credits) common_manager.bet++;
    }
    if (input_manager.IsReleased(Key::down)) {
        if (common_manager.bet - 1 >= 1) common_manager.bet--;
    }

    if (input_manager.IsReleased(Key::enter)) {
        eng.StateChange<Reeling>();
    }
}

void Betting::Update(single::Engine& eng, double delta_t) {}

void Betting::Render() {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.GetGrid().RenderGrid(vis);

    bet.Update(vis, "Bet: " + std::to_string(common_manager.bet), 32.0);

    vis.RenderText(credits, 50.0, 610.0);
    vis.RenderText(bet, 50.0, 650.0);

    vis.RenderText(title, 260.0, 690.0);
}

void Betting::OnExit() {}