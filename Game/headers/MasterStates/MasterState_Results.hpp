#pragma once

#include "../GameObjects/CommonManager.hpp"
#include "../GameObjects/InputManager.hpp"

class MasterResults : public single::GameState {

    protected:
    CommonManager& common_manager = CommonManager::GetInstance();
    InputManager& input_manager = InputManager::GetInstance();

    single::Color line_color;
    int winning_lines_amount;
    int display_line_id;
    double line_display_timer;

    single::Color frame_color;
    bool show_frames;
    double frame_display_timer;
    
    single::Text win;
    int win_amount;
    int display_win;
    double win_display_timer;

    single::Text bet;

    bool scatters_found;

    float CalculateX(int cell_id, const GridData& grid_data, bool reverse_lines) const;
    float CalculateY(int cell_id, const GridData& grid_data, bool reverse_lines) const;

    void DrawLine(single::Engine& eng, const Line& ln, const single::Color& color) const;
    void DrawCellFrame(single::Engine& eng, const Cell& cell, const single::Color& color) const;


    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};