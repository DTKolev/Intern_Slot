#include "../../headers/MasterStates/MasterState_Reeling.hpp"

void MasterReeling::OnEntry(single::Engine& eng) {

    timer = 1.5;
    reeling = true;

    common_manager.GetGrid().PrepareReelSpin(eng);
}

void MasterReeling::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    int total_columns = common_manager.GetGrid().GetGridData().columns;
    int active_reels = common_manager.GetGrid().GetActiveReels();

    input_manager.ProcessInput(input_event);

    if ((input_manager.IsReleased(Key::enter) || input_manager.IsReleased(Key::escape)) && timer <= 1.0) {
        if (active_reels == total_columns) reeling = false;
    }   
}

void MasterReeling::Update(single::Engine& eng, double delta_t) {

    if (timer >= 0.0) timer -= delta_t;
    else reeling = false;

    common_manager.GetGrid().SpinReels(eng, delta_t, reeling);
}

void MasterReeling::Render(single::Engine& eng) {

    common_manager.GetGrid().RenderGrid(eng);
}

void MasterReeling::OnExit() {};