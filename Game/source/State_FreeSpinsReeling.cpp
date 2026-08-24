#include "../headers/Mode_FreeSpins.hpp"
#include "../headers/CommonManager.hpp"
#include "../headers/InputManager.hpp"
#include <string>

void FreeSpinsReeling::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();

    timer = 1.5;
    reeling = true;

    common_manager.free_spins--;

    winnings = eng.CreateText("Free Spin Winnings: " + std::to_string(common_manager.free_spins_winnings), 32.0f);
    free_spins = eng.CreateText("Free Spins Remaining: " + std::to_string(common_manager.free_spins), 32.0f, (single::Color){0, 255, 0, 255});

    common_manager.GetGrid().PrepareReelSpin(eng);
}

void FreeSpinsReeling::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();
    CommonManager& common_manager = CommonManager::GetInstance();

    int total_columns = common_manager.GetGrid().GetGridData().columns;
    int active_reels = common_manager.GetGrid().GetActiveReels();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::enter) || input_manager.IsReleased(Key::escape)) {
        if (active_reels == total_columns) reeling = false;
    }
}

void FreeSpinsReeling::Update(single::Engine& eng, double delta_t) {

    CommonManager& common_manager = CommonManager::GetInstance();
    Grid& grid = common_manager.GetGrid();

    timer -= delta_t;

    if (timer <= 0.0) reeling = false;

    grid.SpinReels(eng, delta_t, reeling);
    if (grid.ReelingFinished()) eng.StateChange<FreeSpinsResults>();
}

void FreeSpinsReeling::Render(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.GetGrid().RenderGrid(eng);

    eng.RenderText(winnings, 50.0f, 610.0f);
    eng.RenderText(free_spins, 260.0f, 650.0f);
}

void FreeSpinsReeling::OnExit() {}