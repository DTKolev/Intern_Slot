#include "../headers/Mode_ExtraReel.hpp"
#include "../headers/CommonManager.hpp"
#include "../headers/InputManager.hpp"
#include <iostream>

void ExtraReelTransitionIn::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();
    GridData grid_data = common_manager.GetGrid().GetGridData();

    target_cell_size = 1000.0f / (float)(grid_data.columns + 1);
    target_grid_y = 300.0f - (float)grid_data.rows * target_cell_size / 2.0f;

    std::cout << target_cell_size << ' ' << target_grid_y << '\n';

    current_cell_size = grid_data.cell_size;
    current_grid_y = grid_data.grid_y;

    transition_timer = 1.0;
    grid_relocation_finished = false;

    size_transition_step = (target_cell_size - current_cell_size) / transition_timer;
    y_transition_step = (target_grid_y - current_grid_y) / transition_timer;

    std::cout << size_transition_step << ' ' << y_transition_step << '\n';
}

void ExtraReelTransitionIn::HandleInput(single::Engine& eng, SDL_Event& input_event) {}

void ExtraReelTransitionIn::Update(single::Engine& eng, double delta_t) {

    Grid& grid = CommonManager::GetInstance().GetGrid();
    GridData grid_data = grid.GetGridData();

    if (transition_timer >= 0.0) {
        transition_timer -= delta_t;

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

void ExtraReelTransitionIn::Render(single::Engine& eng) {

    Grid& grid = CommonManager::GetInstance().GetGrid();

    grid.RenderGrid(eng);
}

void ExtraReelTransitionIn::OnExit() {

}