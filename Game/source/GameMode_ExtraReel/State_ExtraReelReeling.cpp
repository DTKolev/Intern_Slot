#include "GameModes/Mode_ExtraReel.hpp"

void ExtraReelReeling::OnEntry(const single::Engine& eng) {

    Grid& grid = common_manager.GetGrid();
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

    Grid& grid = common_manager.GetGrid();
    Reel& extra_reel = grid.GetReel(grid.GetGridData().columns - 1);

    timer -= delta_t;
    if (timer <= 0.0) reeling = false; 

    extra_reel.SpinReel(eng, grid.GetGridData(), 13.0, delta_t, reeling);

    if (extra_reel.AnimationFinished()) {
        grid.UpdateGridState();
        eng.StateChange<ExtraReelResults>();
    }
}