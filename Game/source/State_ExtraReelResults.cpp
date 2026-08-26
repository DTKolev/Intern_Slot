#include "../headers/Mode_ExtraReel.hpp"
#include "../headers/CommonManager.hpp"
#include "../headers/InputManager.hpp"
#include <string>

void ExtraReelResults::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();
    Analyzer& analyzer = common_manager.GetAnalyzer();
    Grid& grid = common_manager.GetGrid();
    Reel& extra_reel = grid.GetReel(grid.GetGridData().columns - 1);

    int multiplier = analyzer.CalculateMultiplier(grid, common_manager.reverse_lines);
    win_amount = common_manager.bet * multiplier;

    if (extra_reel.GetScatters(grid.GetGridData()) == 1) {
        common_manager.free_spins_mode = true;
        common_manager.free_spins += 10;
        scatter_found = true;
    }
    else scatter_found = false;

    if (scatter_found) outcome = eng.CreateText("You found the third scatter!", 32.0f, (single::Color){0, 255, 0, 255});
    else outcome = eng.CreateText("No scatter found!", 32.0f, (single::Color){255, 255, 255, 255});

    win = eng.CreateText("Reversed lines win: " + std::to_string(win_amount), 32.0f);
    bet = eng.CreateText("Bet: " + std::to_string(common_manager.bet), 32.0f);
    
    frame_color = eng.RandomColor();
    frame_timer = 0.3;
    show_frames = true;

    line_color = eng.RandomColor();
    line_display_timer = 1.5;
    display_line_id = 0;
    winning_lines_amount = analyzer.GetWinningLines().size();
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

    if (winning_lines_amount > 0) {
        line_display_timer -= delta_t;

        if (line_display_timer <= 0.0) {

            line_color = eng.RandomColor();
            display_line_id++;
            display_line_id %= winning_lines_amount;

            line_display_timer = 1.5;
        }
    }
}

void ExtraReelResults::Render(single::Engine& eng) {

    Grid& grid = CommonManager::GetInstance().GetGrid();
    Analyzer& analyzer = CommonManager::GetInstance().GetAnalyzer();

    grid.RenderGrid(eng);

    if (scatter_found) {
        for (const Cell& cell : grid.ExportCells()) {
            if (cell.content == CellContent::scatter && show_frames) DrawCellFrame(eng, cell, frame_color);
        }
    }

    if (winning_lines_amount > 0) {
        DrawLine(eng, analyzer.GetWinningLines()[display_line_id], line_color);
    }

    eng.RenderText(outcome, 260.0f, 650.0f);

    eng.RenderText(win, 50.0f, 610.0f);
    eng.RenderText(bet, 50.0f, 650.0f);
}

void ExtraReelResults::OnExit() {

    CommonManager& common_manager = CommonManager::GetInstance();
    common_manager.credits += win_amount;
}



float ExtraReelResults::CalculateX(int cell_id, GridData grid_data, bool reverse) const {

    if (!reverse) {
        return grid_data.grid_x + (float)(cell_id % grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
    }
    else {
        float grid_x_border = grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size;
        return grid_x_border - (float)(cell_id % grid_data.columns) * grid_data.cell_size - grid_data.cell_size / 2.0f;
    }
}

float ExtraReelResults::CalculateY(int cell_id, GridData grid_data, bool reverse) const {

    return grid_data.grid_y + (float)(cell_id / grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
}



void ExtraReelResults::DrawLine(single::Engine& eng, const Line& ln, single::Color color) const {

    CommonManager& common_manager = CommonManager::GetInstance();
    GridData grid_data = common_manager.GetGrid().GetGridData();

    int current_cell = ln.line_begin_row * grid_data.columns;
    int dir_multiplier = 0;

    float current_x = CalculateX(current_cell, grid_data, common_manager.reverse_lines);
    float current_y = CalculateY(current_cell, grid_data, common_manager.reverse_lines);

    float next_x;
    float next_y;

    if (!common_manager.reverse_lines) {
        eng.RenderLine(grid_data.grid_x, current_y, current_x, current_y, 10.0f, color);
        dir_multiplier = 1;
    }
    else {
        float grid_x_border = grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size;
        eng.RenderLine(grid_x_border, current_y, current_x, current_y, 10.0f, color);
        dir_multiplier = -1;
    }

    for (const Direction& dir : ln.line_directions) {

        int next_cell = 0;
        switch (dir) {
            case Direction::forward:
                next_cell = current_cell + 1;
                break;
            case Direction::down:
                next_cell = current_cell + 1 + grid_data.columns;
                break;
            case Direction::up:
                next_cell = current_cell + 1 - grid_data.columns;
                break;
        }
        
        current_x = CalculateX(current_cell, grid_data, common_manager.reverse_lines);
        current_y = CalculateY(current_cell, grid_data, common_manager.reverse_lines);

        next_x = CalculateX(next_cell, grid_data, common_manager.reverse_lines);
        next_y = CalculateY(next_cell, grid_data, common_manager.reverse_lines);

        eng.RenderLine(current_x, current_y, next_x, next_y, 10.0f, color);

        current_cell = next_cell;
    }

    if (!common_manager.reverse_lines) {
        float grid_x_border = grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size;
        eng.RenderLine(next_x, next_y, grid_x_border, next_y, 10.0f, color);
    }
    else eng.RenderLine(next_x, next_y, grid_data.grid_x, next_y, 10.0f, color);
}



void ExtraReelResults::DrawCellFrame(single::Engine& eng, const Cell& cell, single::Color color) const {

    float cell_x = (float)cell.location.x;
    float cell_y = (float)cell.location.y;
    float cell_size = (float)cell.location.h;

    eng.RenderLine(cell_x, cell_y, cell_x, cell_y + cell_size, 10.0f, color);
    eng.RenderLine(cell_x, cell_y, cell_x + cell_size, cell_y, 10.0f, color);
    eng.RenderLine(cell_x, cell_y + cell_size, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
    eng.RenderLine(cell_x + cell_size, cell_y, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
}