#include "../../headers/MasterStates/MasterState_Results.hpp"
#include <string>

void MasterResults::OnEntry(single::Engine& eng) {

    Analyzer& analyzer = common_manager.GetAnalyzer();
    Grid& grid = common_manager.GetGrid();

    int multiplier = analyzer.CalculateMultiplier(grid, common_manager.reverse_lines);
    win_amount = common_manager.bet * multiplier;
    display_win = 0;
    win_display_timer = 0.01;
    win = eng.CreateText("Win: " + std::to_string(display_win), 32.0f);

    if (grid.ScatterAmount() >= 3) {
        common_manager.free_spins += 10;
        common_manager.free_spins_mode = true;
        scatters_found = true;
    }
    else scatters_found = false;

    winning_lines_amount = analyzer.GetWinningLines().size();
    display_line_id = 0;
    line_display_timer = 1.0;
    line_color = eng.RandomColor();

    show_frames = true;
    frame_display_timer = 0.3;
    frame_color = eng.RandomColor();

    bet = eng.CreateText("Bet: " + std::to_string(common_manager.bet), 32.0f);
}

void MasterResults::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    input_manager.ProcessInput(input_event);
}

void MasterResults::Update(single::Engine& eng, double delta_t) {

    if (display_win < win_amount) {
        win_display_timer -= delta_t;

        if (win_display_timer <= 0.0) {
            display_win++;

            win_display_timer = 0.01;
        }
    }

    if (scatters_found) {
        frame_display_timer -= delta_t;

        if (frame_display_timer <= 0.0) {
            show_frames = !show_frames;
            frame_color = eng.RandomColor();

            frame_display_timer = 0.3;
        } 
    }

    if (winning_lines_amount > 0) {
        line_display_timer -= delta_t;

        if (line_display_timer <= 0.0) {
            display_line_id++;
            display_line_id %= winning_lines_amount;
            line_color = eng.RandomColor();

            line_display_timer = 1.0;
        }
    }
}

void MasterResults::Render(single::Engine& eng) {

    common_manager.GetGrid().RenderGrid(eng);

    if (winning_lines_amount > 0) {
        DrawLine(eng, common_manager.GetAnalyzer().GetWinningLines()[display_line_id], line_color);
    }

    if (scatters_found) {
        for (const Cell& cell : common_manager.GetGrid().ExportCells()) {
            if (cell.content == CellContent::scatter && show_frames) DrawCellFrame(eng, cell, frame_color);
        }
    }

    win.Update(eng, "Win: " + std::to_string(display_win), 32.0f);
    eng.RenderText(bet, 50.0f, 650.0f);
    eng.RenderText(win, 50.0f, 690.0f);
}

void MasterResults::OnExit() {

    common_manager.credits += win_amount;
}



float MasterResults::CalculateX(int cell_id, const GridData& grid_data, bool reverse_lines) const {

    if (!reverse_lines) {
        return grid_data.grid_x + (float)(cell_id % grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
    }
    else {
        float grid_x_border = grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size;
        return grid_x_border - (float)(cell_id % grid_data.columns) * grid_data.cell_size - grid_data.cell_size / 2.0f;
    }
}

float MasterResults::CalculateY(int cell_id, const GridData& grid_data, bool reverse_lines) const {

    return grid_data.grid_y + (float)(cell_id / grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
}



void MasterResults::DrawLine(single::Engine& eng, const Line& ln, const single::Color& color) const {

    GridData grid_data = common_manager.GetGrid().GetGridData();

    int current_cell = ln.line_begin_row * grid_data.columns;

    float current_x = CalculateX(current_cell, grid_data, common_manager.reverse_lines);
    float current_y = CalculateY(current_cell, grid_data, common_manager.reverse_lines);

    float next_x;
    float next_y;

    if (!common_manager.reverse_lines) {
        eng.RenderLine(grid_data.grid_x, current_y, current_x, current_y, 10.0f, color);
    }
    else {
        float grid_x_border = grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size;
        eng.RenderLine(grid_x_border, current_y, current_x, current_y, 10.0f, color);
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

void MasterResults::DrawCellFrame(single::Engine& eng, const Cell& cell, const single::Color& color) const {

    float cell_x = cell.location.x;
    float cell_y = cell.location.y;
    float cell_size = cell.location.h;

    eng.RenderLine(cell_x, cell_y, cell_x, cell_y + cell_size, 10.0f, color);
    eng.RenderLine(cell_x, cell_y, cell_x + cell_size, cell_y, 10.0f, color);
    eng.RenderLine(cell_x, cell_y + cell_size, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
    eng.RenderLine(cell_x + cell_size, cell_y, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
}