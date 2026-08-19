#include "../headers/State_Reeling.hpp"
#include "../headers/State_Results.hpp"
#include "../headers/InputManager.hpp"
#include "../headers/CommonManager.hpp"

void Reeling::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();

    target_credits = common_manager.credits - common_manager.bet;
    timer = 2.5;
    reeling = true;

    credits = eng.CreateText("Credits: " + std::to_string(common_manager.credits), 32.0);
    title = eng.CreateText("REELING", 32.0);

    common_manager.GetGrid().PrepareReelSpin(eng);
}

void Reeling::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::enter) || input_manager.IsReleased(Key::escape)) {
        if (timer <= 1.5) reeling = false;
    }
}

void Reeling::Update(single::Engine& eng, double delta_t) {

    CommonManager& common_manager = CommonManager::GetInstance();
    Grid& grid = common_manager.GetGrid();

    if (common_manager.credits > target_credits) {
        common_manager.credits--;
        eng.Delay(15);
    }
    else {
        timer -= delta_t;
        if (timer <= 0.0f) reeling = false;

        grid.SpinReels(eng, 2000.0, delta_t, reeling); 
        if (grid.ReelingFinished()) eng.StateChange<Results>();
    }
}

void Reeling::Render(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.GetGrid().RenderGrid(eng);

    credits.Update(eng, "Credits: " + std::to_string(common_manager.credits), 32.0);

    eng.RenderText(credits, 50.0, 610.0);
    eng.RenderText(title, 260.0, 690.0);
}

void Reeling::OnExit() {};