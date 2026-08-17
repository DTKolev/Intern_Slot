#include "../headers/State_MainMenu.hpp"
#include "../headers/State_Betting.hpp"
#include "InputManager.hpp"

void MainMenu::OnEntry(single::Engine& eng) {

    title = eng.CreateText("Slot Game", 48.0);
}

void MainMenu::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::enter)) eng.StateChange<Betting>();
    else if (input_manager.IsReleased(Key::escape)) eng.Quit();
}

void MainMenu::Update(single::Engine& eng, double delta_t) {}

void MainMenu::Render(single::Engine& eng) {

    float title_x = 500.0f - title.GetWidth() / 2.0f;

    eng.RenderText(title, title_x, 200.0);
}

void MainMenu::OnExit() {};



