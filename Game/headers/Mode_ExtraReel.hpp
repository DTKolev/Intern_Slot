#pragma once

#include "../../Singleton/Singleton.hpp"
#include "Grid.hpp"
#include "Analyzer.hpp"

class ExtraReelTransitionIn : public single::GameState {

    private:
    float target_cell_size;
    float target_grid_y;

    float current_cell_size;
    float current_grid_y;

    float size_transition_step;
    float y_transition_step;

    double transition_timer;
    bool grid_relocation_finished;

    bool extra_reel_created;
    double extra_reel_timer;
    float x_transition_step;
    float extra_reel_x;

    bool transition_finished;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class ExtraReelReeling : public single::GameState {

    private:
    double timer;
    bool reeling;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class ExtraReelResults : public single::GameState {

    private:
    single::Text outcome;
    single::Text bet;
    single::Text win;

    bool scatter_found;
    
    single::Color frame_color;
    double frame_timer;
    bool show_frames;

    int win_amount;

    int winning_lines_amount;
    double line_display_timer;
    int display_line_id;
    single::Color line_color;

    float CalculateX(int cell_id, GridData grid_data, bool reverse) const;
    float CalculateY(int cell_id, GridData grid_data, bool reverse) const;

    void DrawLine(single::Engine& eng, const Line& ln, single::Color color) const;
    void DrawCellFrame(single::Engine& eng, const Cell& cell, single::Color color) const;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class ExtraReelTransitionOut : public single::GameState {

    private:
    float target_cell_size;
    float target_grid_y;
    float current_cell_size;
    float current_grid_y;

    float size_transition_step;
    float y_transition_step;

    double transition_timer;
    bool transition_finished;

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};