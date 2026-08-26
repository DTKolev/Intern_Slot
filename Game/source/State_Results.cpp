#include "../headers/Mode_Regular.hpp"
#include "../headers/Mode_FreeSpins.hpp"
#include "../headers/Mode_ExtraReel.hpp"
#include "../headers/InputManager.hpp"
#include "../headers/CommonManager.hpp"

void Results::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();
    Grid& grid = common_manager.GetGrid();
    Analyzer& analyzer = common_manager.GetAnalyzer();

    win_amount = common_manager.bet * analyzer.CalculateMultiplier(grid, common_manager.reverse_lines);

    if (grid.ScatterAmount() >= 3) {
        common_manager.free_spins += 10;
        common_manager.free_spins_mode = true;
    }
    else if (grid.ScatterAmount() == 2) {
        common_manager.extra_reel_mode = true;
    }

    credits = eng.CreateText("Credits: " + std::to_string(common_manager.credits), 32.0);
    bet = eng.CreateText("Bet: " + std::to_string(common_manager.bet), 32.0);
    win = eng.CreateText("Win: ", 32.0);
    title = eng.CreateText("RESULTS", 32.0);
    free_spins = eng.CreateText("FREE SPINS MODE ACHEIVED", 32.0f, (single::Color){0, 255, 0, 255});
    extra_reel = eng.CreateText("EXTRA REEL", 32.0f, (single::Color){0, 255, 0, 255});

    display_win = 0;

    win_line_amount = analyzer.GetWinningLines().size();
    current_win_line = 0;
    line_display_timer = 2.0;

    curren_win_line_color = eng.RandomColor();

    scatter_frame_timer = 0.3;
    show_frames = true;
    frame_color = eng.RandomColor();
}

void Results::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();
    CommonManager& common_manager = CommonManager::GetInstance();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::enter)) {
        if (common_manager.free_spins_mode) eng.OverlayState<FreeSpinsTransitionIn>();
        else if (common_manager.extra_reel_mode) eng.StateChange<ExtraReelTransitionIn>();
        else eng.StateChange<Reeling>();
    }
    if (input_manager.IsReleased(Key::shift)) {
        if (!common_manager.free_spins_mode) eng.StateChange<Betting>();
    }
    if (input_manager.IsReleased(Key::escape)) {
        eng.StateChange<MainMenu>();
    }
}

void Results::Update(single::Engine& eng, double delta_t) {

    CommonManager& common_manager = CommonManager::GetInstance();

    if (win_line_amount > 0) {
        line_display_timer -= delta_t;

        if (line_display_timer <= 0.0) {
            current_win_line++;
            current_win_line %= win_line_amount;
            line_display_timer = 2.0;

            curren_win_line_color = eng.RandomColor();
        }
    }

    if (common_manager.free_spins_mode || common_manager.extra_reel_mode) {
        scatter_frame_timer -= delta_t;
        
        if (scatter_frame_timer <= 0.0) {
            show_frames = !show_frames;
            scatter_frame_timer = 0.3;

            frame_color = eng.RandomColor();
        }
    }

    if (display_win < win_amount) {
        display_win++;
        eng.Delay(10);
    }
}

void Results::Render(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.GetGrid().RenderGrid(eng);
    if (win_line_amount > 0) {
        DrawLine(eng, common_manager.GetAnalyzer().GetWinningLines()[current_win_line], curren_win_line_color);
    }

    win.Update(eng, "Win: " + std::to_string(display_win), 32.0);

    eng.RenderText(credits, 50.0, 610.0);
    eng.RenderText(bet, 50.0, 650.0);
    eng.RenderText(win, 50.0, 690.0);

    eng.RenderText(title, 260.0, 690.0);

    if (common_manager.free_spins_mode) {
        for (const Cell& cell : common_manager.GetGrid().ExportCells()) {
            if (cell.content == CellContent::scatter && show_frames) DrawCellFrame(eng, cell, frame_color);
        }

        eng.RenderText(free_spins, 260.0f, 650.0f);
    }
    else if (common_manager.extra_reel_mode) {
        for (const Cell& cell : common_manager.GetGrid().ExportCells()) {
            if (cell.content == CellContent::scatter && show_frames) DrawCellFrame(eng, cell, frame_color);
        }

        eng.RenderText(extra_reel, 260.0f, 650.0f);
    }
}

void Results::OnExit() {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.credits += win_amount;
}



float Results::CalculateX(int cell_id, GridData grid_data, bool reverse) const {

    if (!reverse) {
        return grid_data.grid_x + (float)(cell_id % grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
    }
    else {
        float grid_x_border = grid_data.grid_x + (float)grid_data.columns * grid_data.cell_size;
        return grid_x_border - (float)(cell_id % grid_data.columns) * grid_data.cell_size - grid_data.cell_size / 2.0f;
    }
}

float Results::CalculateY(int cell_id, GridData grid_data, bool reverse) const {

    return grid_data.grid_y + (float)(cell_id / grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
}



void Results::DrawLine(single::Engine& eng, const Line& ln, single::Color color) const {

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



void Results::DrawCellFrame(single::Engine& eng, const Cell& cell, single::Color color) const {

    float cell_x = (float)cell.location.x;
    float cell_y = (float)cell.location.y;
    float cell_size = (float)cell.location.h;

    eng.RenderLine(cell_x, cell_y, cell_x, cell_y + cell_size, 10.0f, color);
    eng.RenderLine(cell_x, cell_y, cell_x + cell_size, cell_y, 10.0f, color);
    eng.RenderLine(cell_x, cell_y + cell_size, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
    eng.RenderLine(cell_x + cell_size, cell_y, cell_x + cell_size, cell_y + cell_size, 10.0f, color);
}