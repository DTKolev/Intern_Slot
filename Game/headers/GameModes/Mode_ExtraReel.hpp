#pragma once

#include "MasterStates/MasterState_Reeling.hpp"
#include "MasterStates/MasterState_Results.hpp"
#include "Singleton_Visualizer.hpp"

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
    void OnEntry(const single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render() override;
    void OnExit() override;
};

class ExtraReelReeling : public MasterReeling {

    public:
    void OnEntry(const single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
};

class ExtraReelResults : public MasterResults {

    private:
    single::Text outcome;

    public:
    void OnEntry(const single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Render() override;
};

class ExtraReelTransitionOut : public single::GameState {

    private:
    float target_cell_size;
    float target_grid_y;
    float current_cell_size;
    float current_grid_y;

    float size_transition_step;
    float y_transition_step;

    float current_reel_x;
    float x_transition_step;
    double extra_reel_timer;
    bool extra_reel_relocated;

    double transition_timer;
    bool transition_finished;

    public:
    void OnEntry(const single::Engine& eng) override;
    void HandleInput(single::Engine& eng, SDL_Event& input_event) override;
    void Update(single::Engine& eng, double delta_t) override;
    void Render() override;
    void OnExit() override;
};