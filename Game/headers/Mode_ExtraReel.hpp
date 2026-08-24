#pragma once

#include "../../Singleton/Singleton.hpp"

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

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class ExtraReelReeling : public single::GameState {

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class ExtraReelResults : public single::GameState {

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};

class ExtraReelTransitionOut : public single::GameState {

    public:
    void OnEntry(single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render(single::Engine& eng) override;
    void OnExit() override;
};