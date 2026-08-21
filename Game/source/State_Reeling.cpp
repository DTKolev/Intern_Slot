#include "../headers/State_Reeling.hpp"
#include "../headers/State_Results.hpp"
#include "../headers/InputManager.hpp"
#include "../headers/CommonManager.hpp"

void Reeling::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();

    target_credits = common_manager.credits - common_manager.bet;

    timer = 4.0;
    reeling = true;

    credits = eng.CreateText("Credits: " + std::to_string(common_manager.credits), 32.0);
    title = eng.CreateText("REELING", 32.0);

    common_manager.GetGrid().PrepareReelSpin(eng);
}

void Reeling::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();
    CommonManager& common_manager = CommonManager::GetInstance();
    
    int total_columns = common_manager.GetGrid().GetGridData().columns;
    int active_reels = common_manager.GetGrid().GetActiveReels();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::enter) || input_manager.IsReleased(Key::escape)) {
        if (active_reels == total_columns) reeling = false;
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

        grid.SpinReels(eng, delta_t, reeling); 
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