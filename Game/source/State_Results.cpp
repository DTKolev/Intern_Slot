#include "../headers/State_Results.hpp"
#include "../headers/State_Betting.hpp"
#include "../headers/State_MainMenu.hpp"
#include "../headers/State_Reeling.hpp"
#include "../headers/State_TransitionSpins.hpp"
#include "../headers/InputManager.hpp"
#include "../headers/CommonManager.hpp"

void Results::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();
    Grid& grid = common_manager.GetGrid();
    Analyzer& analyzer = common_manager.GetAnalyzer();

    win_amount = common_manager.bet * analyzer.CalculateMultiplier(grid);

    if (grid.ScatterAmount() >= 3) {
        common_manager.free_spins += 10;
        common_manager.free_spins_mode = true;
    }

    credits = eng.CreateText("Credits: " + std::to_string(common_manager.credits), 32.0);
    bet = eng.CreateText("Bet: " + std::to_string(common_manager.bet), 32.0);
    win = eng.CreateText("Win: ", 32.0);
    title = eng.CreateText("RESULTS", 32.0);
    free_spins = eng.CreateText("FREE SPINS MODE ACHEIVED", 32.0f, (single::Color){0, 255, 0, 255});

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
        if (common_manager.free_spins_mode) eng.OverlayState<TransitionSpins>();
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

    if (common_manager.free_spins_mode) {
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
}

void Results::OnExit() {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.credits += win_amount;
}


void Results::DrawLine(single::Engine& eng, const Line& ln, single::Color color) const {

    CommonManager& common_manager = CommonManager::GetInstance();
    GridData grid_data = common_manager.GetGrid().GetGridData();

    float cell_center_x = (ln[0] % grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
    float cell_center_y = (ln[0] / grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
    float next_cell_center_x;
    float next_cell_center_y;

    eng.RenderLine(0.0, cell_center_y, cell_center_x, cell_center_y, 10.0f, color);

    for (int i = 0; i < ln.size() - 1; i++) {

        int cell_id = ln[i];
        int next_cell_id = ln[i + 1];

        cell_center_x = (cell_id % grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
        cell_center_y = (cell_id / grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;

        next_cell_center_x = (next_cell_id % grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;
        next_cell_center_y = (next_cell_id / grid_data.columns) * grid_data.cell_size + grid_data.cell_size / 2.0f;

        eng.RenderLine(cell_center_x, cell_center_y, next_cell_center_x, next_cell_center_y, 10.0f, color);
    }

    eng.RenderLine(next_cell_center_x, next_cell_center_y, 1000.0, next_cell_center_y, 10.0f, color);
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