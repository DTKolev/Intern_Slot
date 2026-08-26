#include "../headers/Mode_ExtraReel.hpp"
#include "../headers/CommonManager.hpp"
#include "../headers/InputManager.hpp"

void ExtraReelReeling::OnEntry(single::Engine& eng) {

    Grid& grid = CommonManager::GetInstance().GetGrid();
    grid.GetReel(grid.GetGridData().columns - 1).StartReelSpin(eng, grid.GetGridData());

    timer = 1.5;
    reeling = true;
}

void ExtraReelReeling::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();

    input_manager.ProcessInput(input_event);

    if ((input_manager.IsReleased(Key::enter) || input_manager.IsReleased(Key::escape)) && timer <= 1.0) {
        reeling = false;
    }
}

void ExtraReelReeling::Update(single::Engine& eng, double delta_t) {

    Grid& grid = CommonManager::GetInstance().GetGrid();
    Reel& extra_reel = grid.GetReel(grid.GetGridData().columns - 1);

    timer -= delta_t;
    if (timer <= 0.0) reeling = false; 

    extra_reel.SpinReel(eng, grid.GetGridData(), 12.0, delta_t, reeling);

    if (extra_reel.AnimationFinished()) eng.StateChange<ExtraReelResults>();
}

void ExtraReelReeling::Render(single::Engine& eng) {

    Grid& grid = CommonManager::GetInstance().GetGrid();

    grid.RenderGrid(eng);
}

void ExtraReelReeling::OnExit() {}