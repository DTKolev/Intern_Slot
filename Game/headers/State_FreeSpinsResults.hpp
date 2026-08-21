#pragma once

#include "../../Singleton/Singleton.hpp"
#include "Grid.hpp"
#include <vector>

class FreeSpinsResults : public single::GameState {

    using Line = std::vector<int>;

    private:
    single::Text winnings;
    single::Text bet;
    single::Text win;
    single::Text free_spins;

    int win_line_amount;
    int current_win_line;
    double line_display_timer;
    single::Color current_win_line_color;

    int win_amount;
    int display_win;

    bool extra_scatters;
    bool show_frames;
    double scatter_frame_timer;
    single::Color frame_color;

    void DrawLine(single::Engine& eng, const Line& ln, single::Color color) const;
    void DrawCellFrame(single::Engine& eng, const Cell& cell, single::Color color) const;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};