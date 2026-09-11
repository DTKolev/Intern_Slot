#include "GameModes/Mode_Regular.hpp"
#include "GameModes/Mode_FreeSpins.hpp"
#include "GameObjects/InputManager.hpp"
#include "GameObjects/CommonManager.hpp"

void MainMenu::OnEntry(const single::Engine& eng) {

    title = vis.CreateText("Slot Game", 48.0);
}

void MainMenu::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    CommonManager& common_manager = CommonManager::GetInstance();
    InputManager& input_manager = InputManager::GetInstance();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::enter)) {
        if (common_manager.free_spins_mode) eng.StateChange<FreeSpinsEntry>();
        else eng.StateChange<Betting>();
    }
    else if (input_manager.IsReleased(Key::escape)) eng.Quit();
}

void MainMenu::Update(single::Engine& eng, double delta_t) {}

void MainMenu::Render() {

    float title_x = 500.0f - title.GetWidth() / 2.0f;

    vis.RenderText(title, title_x, 200.0);
}

void MainMenu::OnExit() {};
