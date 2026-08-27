#include "../headers/GameModes/Mode_ExtraReel.hpp"
#include "../headers/GameObjects/CommonManager.hpp"
#include "../headers/GameObjects/InputManager.hpp"

void ExtraReelTransitionIn::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();
    GridData grid_data = common_manager.GetGrid().GetGridData();

    target_cell_size = ((float)grid_data.columns * grid_data.cell_size) / (float)(grid_data.columns + 1);
    target_grid_y = 300.0f - (float)grid_data.rows * target_cell_size / 2.0f;

    current_cell_size = grid_data.cell_size;
    current_grid_y = grid_data.grid_y;

    transition_timer = 0.5;
    grid_relocation_finished = false;

    size_transition_step = (target_cell_size - current_cell_size) / transition_timer;
    y_transition_step = (target_grid_y - current_grid_y) / transition_timer;

    extra_reel_created = false;
    extra_reel_timer = 0.5;

    transition_finished = false;
}

void ExtraReelTransitionIn::HandleInput(single::Engine& eng, SDL_Event& input_event) {}

void ExtraReelTransitionIn::Update(single::Engine& eng, double delta_t) {

    Grid& grid = CommonManager::GetInstance().GetGrid();
    GridData grid_data = grid.GetGridData();

    if (!grid_relocation_finished) {

        transition_timer -= delta_t;

        if (transition_timer >= 0.0) {

            current_cell_size += size_transition_step * delta_t;
            current_grid_y += y_transition_step * delta_t;
        }
        else {
            current_cell_size = target_cell_size;
            current_grid_y = target_grid_y;

            grid_relocation_finished = true;
        }

        grid.RelocateGrid(grid_data.grid_x, current_grid_y, current_cell_size);
    }

    if (grid_relocation_finished && !extra_reel_created) {

        grid.AddExtraReel(1000.0f);
        grid_data = grid.GetGridData();
        extra_reel_x = 1000.0f;
        x_transition_step = ((grid_data.grid_x + (float)(grid_data.columns - 1) * grid_data.cell_size) - 1000.0f) / extra_reel_timer;
        extra_reel_created = true;
    }

    if (extra_reel_created) {
        extra_reel_timer -= delta_t;

        if (extra_reel_timer >= 0.0) {

            extra_reel_x += x_transition_step * delta_t;
        }
        else {
            extra_reel_x = grid_data.grid_x + grid_data.cell_size * (float)(grid_data.columns - 1);
            transition_finished = true;
        }

        grid.GetReel(grid_data.columns - 1).RelocateReel(extra_reel_x, grid_data);
    }

    if (transition_finished) {
        grid.AlignReels();
        eng.StateChange<ExtraReelReeling>();
    }
}

void ExtraReelTransitionIn::Render(single::Engine& eng) {

    Grid& grid = CommonManager::GetInstance().GetGrid();

    grid.RenderGrid(eng);
}

void ExtraReelTransitionIn::OnExit() {

    CommonManager& common_manager = CommonManager::GetInstance();
    common_manager.reverse_lines = true;
}