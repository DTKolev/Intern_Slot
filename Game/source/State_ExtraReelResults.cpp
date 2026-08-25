#include "../headers/Mode_ExtraReel.hpp"
#include "../headers/CommonManager.hpp"
#include "../headers/InputManager.hpp"
#include <string>

void ExtraReelResults::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();
    Grid& grid = common_manager.GetGrid();
    Reel& extra_reel = grid.GetReel(grid.GetGridData().columns - 1);

    if (extra_reel.GetScatters(grid.GetGridData()) == 1) {
        common_manager.free_spins_mode = true;
        common_manager.free_spins += 10;
        scatter_found = true;
    }
    else scatter_found = false;

    if (scatter_found) outcome = eng.CreateText("You found the third scatter!", 32.0f, (single::Color){0, 255, 0, 255});
    else outcome = eng.CreateText("No scatter found, back to regular mode!", 32.0f, (single::Color){255, 255, 255, 255});
    
    frame_color = eng.RandomColor();
    frame_timer = 0.3;
    show_frames = true;
}

void ExtraReelResults::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::enter)) {
        eng.StateChange<ExtraReelTransitionOut>();
    }
}

void ExtraReelResults::Update(single::Engine& eng, double delta_t) {

    if (scatter_found) {
        frame_timer -= delta_t;

        if (frame_timer <= 0.0) {
            show_frames = !show_frames;
            frame_timer = 0.3;
            frame_color = eng.RandomColor();
        }
    }
}

void ExtraReelResults::Render(single::Engine& eng) {

    Grid& grid = CommonManager::GetInstance().GetGrid();

    grid.RenderGrid(eng);

    if (scatter_found) {
        for (const Cell& cell : grid.ExportCells()) {
            if (cell.content == CellContent::scatter && show_frames) DrawCellFrame(eng, cell, frame_color);
        }
    }

    eng.RenderText(outcome, 260.0f, 650.0f);
}

void ExtraReelResults::OnExit() {}



void ExtraReelResults::DrawCellFrame(single::Engine& eng, const Cell& cell, single::Color color) const {

    float cell_x = (float)cell.location.x;
    float cell_y = (float)cell.location.y;
    float cell_size = (float)cell.location.h;

    eng.RenderLine(cell_x, cell_y, cell_x, cell_y + cell_size, 10.0f, color);
    eng.RenderLine(cell_x, cell_y, cell_x + cell_size, cell_y, 10.0f, color);
    eng.RenderLine(cell_x, cell_y + cell_size, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
    eng.RenderLine(cell_x + cell_size, cell_y, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
}