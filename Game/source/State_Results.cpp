#include "../headers/State_Results.hpp"
#include "../headers/State_Betting.hpp"
#include "../headers/State_MainMenu.hpp"
#include "../headers/State_Reeling.hpp"
#include "../headers/InputManager.hpp"
#include "../headers/CommonManager.hpp"

void Results::OnEntry(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();
    Grid& grid = common_manager.GetGrid();
    Analyzer& analyzer = common_manager.GetAnalyzer();

    win_amount = common_manager.bet * analyzer.CalculateMultiplier(grid);

    credits = eng.CreateText("Credits: " + std::to_string(common_manager.credits), 32.0);
    bet = eng.CreateText("Bet: " + std::to_string(common_manager.bet), 32.0);
    win = eng.CreateText("Win: ", 32.0);
    title = eng.CreateText("RESULTS", 32.0);

    display_win = 0;
}

void Results::HandleInput(single::Engine& eng, SDL_Event& input_event) {

    InputManager& input_manager = InputManager::GetInstance();
    CommonManager& common_manager = CommonManager::GetInstance();

    input_manager.ProcessInput(input_event);

    if (input_manager.IsReleased(Key::enter)) {
        eng.StateChange<Reeling>();
    }
    if (input_manager.IsReleased(Key::shift)) {
        eng.StateChange<Betting>();
    }
    if (input_manager.IsReleased(Key::escape)) {
        eng.StateChange<MainMenu>();
    }
}

void Results::Update(single::Engine& eng, double delta_t) {

    if (display_win < win_amount) {
        display_win++;
        eng.Delay(10);
    }
}

void Results::Render(single::Engine& eng) {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.GetGrid().RenderGrid(eng);

    win.Update(eng, "Win: " + std::to_string(display_win), 32.0);

    eng.RenderText(credits, 50.0, 610.0);
    eng.RenderText(bet, 50.0, 650.0);
    eng.RenderText(win, 50.0, 690.0);

    eng.RenderText(title, 260.0, 690.0);
}

void Results::OnExit() {

    CommonManager& common_manager = CommonManager::GetInstance();

    common_manager.credits += win_amount;
}