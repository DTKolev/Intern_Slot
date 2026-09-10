#include "MasterStates/MasterState_Results.hpp"
#include "GameObjects/Log.hpp"
#include <fstream>
#include <string>

void MasterResults::OnEntry(const single::Engine& eng) {

    Analyzer& analyzer = common_manager.GetAnalyzer();
    Grid& grid = common_manager.GetGrid();

    int multiplier = analyzer.CalculateMultiplier(grid, common_manager.reverse_lines);
    win_amount = common_manager.bet * multiplier;
    display_win = 0;
    win_display_timer = 0.01;
    win = vis.CreateText("Win: " + std::to_string(display_win), 32.0f);

    LogGameResults();

    if (grid.ScatterAmount() >= 3) {
        common_manager.free_spins += 10;
        common_manager.free_spins_mode = true;
        scatters_found = true;
    }
    else scatters_found = false;

    winning_lines_amount = analyzer.GetWinningLines().size();
    display_line_id = 0;
    line_display_timer = 1.0;
    line_color = vis.RandomColor();

    show_frames = true;
    frame_display_timer = 0.3;
    frame_color = vis.RandomColor();

    bet = vis.CreateText("Bet: " + std::to_string(common_manager.bet), 32.0f);
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
            frame_color = vis.RandomColor();

            frame_display_timer = 0.3;
        } 
    }

    if (winning_lines_amount > 0) {
        line_display_timer -= delta_t;

        if (line_display_timer <= 0.0) {
            display_line_id++;
            display_line_id %= winning_lines_amount;
            line_color = vis.RandomColor();

            line_display_timer = 1.0;
        }
    }
}

void MasterResults::Render() {

    common_manager.GetGrid().RenderGrid(vis);

    if (winning_lines_amount > 0) {
        DrawLine(common_manager.GetAnalyzer().GetWinningLines()[display_line_id], line_color);
    }

    if (scatters_found) {
        for (const Cell& cell : common_manager.GetGrid().ExportCells()) {
            if (cell.content == CellContent::scatter && show_frames) DrawCellFrame(cell, frame_color);
        }
    }

    win.Update(vis, "Win: " + std::to_string(display_win), 32.0f);
    vis.RenderText(bet, 50.0f, 650.0f);
    vis.RenderText(win, 50.0f, 690.0f);
}

void MasterResults::OnExit() {

    common_manager.credits += win_amount;
}



auto MasterResults::CalculateX(int cell_id, const GridData& grid_data, bool reverse_lines) const -> float {

    if (!reverse_lines) {
        return grid_data.grid_x + (float)(cell_id % grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
    }
    else {
        float grid_x_border = grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size;
        return grid_x_border - (float)(cell_id % grid_data.columns) * grid_data.cell_size - grid_data.cell_size / 2.0f;
    }
}

auto MasterResults::CalculateY(int cell_id, const GridData& grid_data, bool reverse_lines) const -> float {

    return grid_data.grid_y + (float)(cell_id / grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
}



void MasterResults::DrawLine(const Line& ln, const single::Color& color) const {

    const GridData& grid_data = common_manager.GetGrid().GetGridData();

    int current_cell = ln.line_begin_row * grid_data.columns;

    float current_x = CalculateX(current_cell, grid_data, common_manager.reverse_lines);
    float current_y = CalculateY(current_cell, grid_data, common_manager.reverse_lines);

    float next_x;
    float next_y;

    if (!common_manager.reverse_lines) {
        vis.RenderLine(grid_data.grid_x, current_y, current_x, current_y, 10.0f, color);
    }
    else {
        float grid_x_border = grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size;
        vis.RenderLine(grid_x_border, current_y, current_x, current_y, 10.0f, color);
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

        vis.RenderLine(current_x, current_y, next_x, next_y, 10.0f, color);

        current_cell = next_cell;
    }

    if (!common_manager.reverse_lines) {
        float grid_x_border = grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size;
        vis.RenderLine(next_x, next_y, grid_x_border, next_y, 10.0f, color);
    }
    else vis.RenderLine(next_x, next_y, grid_data.grid_x, next_y, 10.0f, color);
}

void MasterResults::DrawCellFrame(const Cell& cell, const single::Color& color) const {

    float cell_x = cell.location.x;
    float cell_y = cell.location.y;
    float cell_size = cell.location.h;

    vis.RenderLine(cell_x, cell_y, cell_x, cell_y + cell_size, 10.0f, color);
    vis.RenderLine(cell_x, cell_y, cell_x + cell_size, cell_y, 10.0f, color);
    vis.RenderLine(cell_x, cell_y + cell_size, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
    vis.RenderLine(cell_x + cell_size, cell_y, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
}



void MasterResults::LogGameResults() const {

    std::string log_file_name {"../src/game_log.txt"};

    std::ifstream input_str {log_file_name};
    if (!input_str) {
        std::cout << "Failed to open file for reading from: " << log_file_name << '\n';
        return;
    }

    // Count the number of rows that are already in the log file
    int row_count = 0;
    std::string line;

    while (std::getline(input_str, line)) row_count++;
    input_str.clear();
    input_str.seekg(0, std::ios::beg);

    // Log file should only contain record from the last 10 spins
    // Free a row if the file already contains 10 entrees by removing the first row of the file
    if (row_count >= 10) {

        std::vector<std::string> remaining_lines;

        // Skip the first line of the file and store the remaining ones
        std::getline(input_str, line); 
        while (std::getline(input_str, line)) {
            remaining_lines.push_back(line);
        }
        input_str.close();

        std::ofstream temp_out {log_file_name}; // Discard all of the existing file contents
        if (!temp_out) {
            std::cout << "Failed to open file for writing from: " << log_file_name << '\n';
            return;
        }

        // Re-write all lines from the original file except the first one
        for (const std::string& remaining_line : remaining_lines) {
            temp_out << remaining_line << '\n';
        }
        temp_out.close();
    }
    else input_str.close();

    std::ofstream output_str {log_file_name, std::ios::app};

    Log new_log;
    new_log.GetLogData(common_manager, win_amount);

    output_str << new_log << '\n';
}
