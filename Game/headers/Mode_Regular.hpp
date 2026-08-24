#pragma once

#include "../../Singleton/Singleton.hpp"
#include "Grid.hpp"
#include <vector>

class Betting : public single::GameState {

    private:
    single::Text credits;
    single::Text bet;
    single::Text title;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class MainMenu : public single::GameState {

    private:
    single::Text title;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class Reeling : public single::GameState {

    private:
    single::Text credits;
    single::Text title;

    int target_credits;
    double timer;
    bool reeling;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class Results : public single::GameState {

    using Line = std::vector<int>;

    private:
    single::Text credits;
    single::Text bet;
    single::Text win;
    single::Text title;
    single::Text free_spins;
    single::Text extra_reel;

    int win_line_amount;
    int current_win_line;
    double line_display_timer;
    single::Color curren_win_line_color;

    int win_amount;
    int display_win;

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