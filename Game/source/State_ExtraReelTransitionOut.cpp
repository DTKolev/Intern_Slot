#include "../headers/Mode_ExtraReel.hpp"
#include "../headers/Mode_Regular.hpp"
#include "../headers/Mode_FreeSpins.hpp"
#include "../headers/CommonManager.hpp"

void ExtraReelTransitionOut::OnEntry(single::Engine& eng) {

    Grid& grid = CommonManager::GetInstance().GetGrid();
    GridData grid_data = grid.GetGridData();

    target_cell_size = 175.0f;
    target_grid_y = 300.0f - ((float)grid_data.rows * target_cell_size) / 2.0f;
    current_cell_size = grid_data.cell_size;
    current_grid_y = grid_data.grid_y;

    transition_timer = 0.5;

    size_transition_step = (target_cell_size - current_cell_size) / transition_timer;
    y_transition_step = (target_grid_y - current_grid_y) / transition_timer;

    current_reel_x = grid_data.grid_x + (float)(grid_data.columns - 1) * grid_data.cell_size;
    extra_reel_timer = 0.5;
    x_transition_step = (1000.0f - current_reel_x) / extra_reel_timer;
    extra_reel_relocated = false;

    transition_finished = false;
}

void ExtraReelTransitionOut::HandleInput(single::Engine& eng, SDL_Event& input_event) {}

void ExtraReelTransitionOut::Update(single::Engine& eng, double delta_t) {

    CommonManager& common_manager = CommonManager::GetInstance();
    Grid& grid = common_manager.GetGrid();

    if (!extra_reel_relocated) {
        extra_reel_timer -= delta_t;

        if (extra_reel_timer >= 0.0) {
            current_reel_x += x_transition_step * delta_t;
        }
        else {
            current_reel_x = 1000.0f;
            extra_reel_relocated = true;
        }

        grid.GetReel(grid.GetGridData().columns - 1).RelocateReel(current_reel_x, grid.GetGridData());
    }
    else {

        transition_timer -= delta_t;

        if (transition_timer >= 0.0) {

            current_cell_size += size_transition_step * delta_t;
            current_grid_y += y_transition_step * delta_t;
        }
        else {
            
            current_cell_size = target_cell_size;
            current_grid_y = target_grid_y;

            transition_finished = true;
        }

        grid.RelocateGrid(grid.GetGridData().grid_x, current_grid_y, current_cell_size);
    }

    if (transition_finished) {
        grid.AlignReels();
        if (common_manager.free_spins_mode) eng.OverlayState<FreeSpinsTransitionIn>();
        else eng.StateChange<Betting>();
    }
}

void ExtraReelTransitionOut::Render(single::Engine& eng) {

    Grid& grid = CommonManager::GetInstance().GetGrid();
    
    grid.RenderGrid(eng);
}

void ExtraReelTransitionOut::OnExit() {

    CommonManager& common_manager = CommonManager::GetInstance();
    Grid& grid = common_manager.GetGrid();

    common_manager.extra_reel_mode = false;
    common_manager.reverse_lines = false;
    grid.RemoveExtraReel();
}